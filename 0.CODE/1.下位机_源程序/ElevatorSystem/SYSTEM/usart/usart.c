/************************************Info************************************
	+程序：USART1，2，3驱动程序 v1.0
	+开发：寄存器
	+型号：/
	+使用：
				usart1Init();
				putCharToU1('1');
				putCharToU1('\n');
				
				usart2Init();
				putCharToU2('2');
				
				putCharToU2('\n');
				usart3Init();
				putCharToU3('3');
				putCharToU3('\n');			
	+接口：
				U1_TX <==> PA9  (无重映射)
				U1_RX <==> PA10
				U2_TX <==> PA2	(无重映射)
				U2_RX <==> PA3
				U3_TX <==> PC10	(部分重映射)
				U3_RX <==> PC11
	+备注：
				U1，U2未使用重映射，U3使用部分重映射
				协议可以支持自定义
*************************************************************************/

#include "stm32f10x.h"
#include "usart.h"
#include "fmq.h"
#include "ele_run.h"
#include <stdio.h>

struct __FILE {int handle;};
FILE __stdout;
FILE __stdin;
FILE __stderr;

byte reBufU1[PACK_MAX_LEN]={0}, reBufU2[PACK_MAX_LEN]={0}, reBufU3[PACK_MAX_LEN]={0};
byte RxLenU1=0, RxLenU2=0, RxLenU3=0;
int RFID_Data=1, JZ_Key_Data=0;
int RFID_Data_Count=0, JZ_Key_Data_Count=0;

/*************************************************** USART 1 begin ***************************************************/
void usart1Init(void)
{
	int i=0x1000;
	// AFIO
	RCC->APB2ENR |= (1UL<<0);
	AFIO->MAPR &=~ (1UL<<2); // TX/PA9， RX/PA10
	
	// GPIOA
	RCC->APB2ENR |= (1UL<<2); // Enable PA
	GPIOA->CRH &= ~(0x0ff0ul); // Clear PA9\10
	GPIOA->CRH |= (0X0B<<4);  // PA9   output push-pull
	GPIOA->CRH |= (0X04<<8);  // PA10  input floating
	
	RCC->APB2ENR |= (1UL << 14);
	// USART1
	USART1->CR1 = 0X002C;
	USART1->CR2 = 0X0000; // 1 stop bit
	USART1->CR3 = 0X0000; // no flow control
	USART1->BRR = 0x0EA0; // 19200
	
	// it
	NVIC->ISER[1] = (1ul<<5);
	
	while(i--) __NOP();
	
	USART1->CR1 |= 0x2000; // enable 
}

// 数据接收
void USART1_IRQHandler(void)
{
	if(USART1->SR & (1UL<<5))
	{
		reBufU1[RxLenU1] = getCharFromU1();
		RxLenU1++;
		
		if(reBufU1[RxLenU1-1] == PACK_END)
		{
			if(reBufU1[0]== PACK_HEAD)
			{
				// check checkFlag
				if((dataXOR(reBufU1, RxLenU1-2)^PACK_END)== reBufU1[RxLenU1-2])
				{
			
					// RFID Data
					if(reBufU1[1] == 0x21)
					{
						RFID_Data_Count = (RFID_Data_Count+1)%10;
						RFID_Data = reBufU1[2];
					}
					
					// JZ_Key Data
					if(reBufU1[1] == 0x22)
					{
						JZ_Key_Data_Count = (JZ_Key_Data_Count+1)%10;
						JZ_Key_Data = reBufU1[2];
						// 0：目标楼层,1：有乘客上行，2：有乘客下行，3：既有上行又有下行
						switch(JZ_Key_Data)
						{
							//case 0x11: input(1, 1); break;
							case 0x12: input(1, 2); break;
							
							case 0x21: input(2, 1); break;
							case 0x22: input(2, 2); break;
							
							case 0x31: input(3, 1); break;
							case 0x32: input(3, 2); break;
							
							case 0x41: input(4, 1); break;
							case 0x42: input(4, 2); break;
							
							//case 0x51: input(5, 1); break;
							case 0x52: input(5, 2); break;

							case 0x01: input(1, 0); break;
							case 0x02: input(2, 0); break;
							case 0x03: input(3, 0); break;
							case 0x04: input(4, 0); break;
							case 0x05: input(5, 0); break;
						}
					}
					
				}			
				RxLenU1 = 0;				
			}
		}
		
		if(RxLenU1>=PACK_END)
		{
			RxLenU1 = 0;
		}
	}
}

/*
	数据发送
	数据格式：
	包头 | 数据类型 | 数据... | 校验位 | 包尾
	
	校验位： 
	校验位 = 包头^数据类型^数据...^包尾
*/

void sendHexMessageToUsart_1(byte *data,byte dataType,unsigned int dataLen)
{
	uint16 len = 3+dataLen+1,i=0,j=0;
	byte readyData[PACK_MAX_LEN] = {0};
	if(PACK_MAX_LEN!=0&&len>PACK_MAX_LEN)
		return ;
	readyData[0] = PACK_HEAD;							// 包头
	readyData[1] = dataType;							// 数据类型
	
	for(i=0;i<(len-2);i++)
	{
		if(i!=0&&i!=1)
		{
			readyData[i] = data[j++];
		}
		readyData[len-2] ^=readyData[i];
	}
	
	readyData[len-1] = PACK_END;          // 包尾
	readyData[len-2] ^= readyData[len-1]; // 校验位
	
	// 发送数据 Usart1
	i=0;
	while(i<len)
	{
		putCharToU1(readyData[i++]);
	}
	
	return ;
}

int putCharToU1 (int c) 
{
  while (!(USART1->SR & USART_SR_TXE));
  USART1->DR = (c & 0x1FF);
  return (c);
}

int getCharFromU1 (void) 
{
//   while (!(USART1->SR & USART_SR_RXNE));
  return (USART1->DR & 0xFF);
}

int checkCharFromU1 (void) 
{
  if (USART1->SR & USART_SR_RXNE)
    return (1);
  else
    return (0);
}

/*************************************************** USART 1 end ***************************************************/



/*************************************************** USART 2 begin ***************************************************/
void usart2Init(void)
{
	int i=0x1000;
	// AFIO
	RCC->APB2ENR |= (1UL<<0);
 	AFIO->MAPR &=~ (1UL<<3);
	
	// GPIOA
	RCC->APB2ENR |= (1UL<<2); 
	GPIOA->CRL &= ~(0xff00ul);  // PA2,PA3
	GPIOA->CRL |= (0X0B<<8);  
	GPIOA->CRL |= (0X04<<12);  
	
	RCC->APB1ENR |= (1UL << 17);
	// USART2
	USART2->CR1 = 0X002C;
	USART2->CR2 = 0X0000; // 1 stop bit
	USART2->CR3 = 0X0000; // no flow control
	USART2->BRR = 0x0EA0; // 19200
	
	// it
	NVIC->ISER[1] = (1ul<<6);
	
	while(i--) __NOP();
	
	USART2->CR1 |= 0x2000; // enable 
}

// 数据接收
void USART2_IRQHandler(void)
{
	if(USART2->SR & (1UL<<5))
	{
		reBufU2[RxLenU2] = getCharFromU2();
		RxLenU2++;
		
		if(reBufU2[RxLenU2-1] == PACK_END)
		{
			if(reBufU2[0]== PACK_HEAD)
			{
				// check checkFlag
				if((dataXOR(reBufU2, RxLenU2-2)^PACK_END)== reBufU2[RxLenU2-2])
				{
					sendHexMessageToUsart_2(&RxLenU2, 0x03, 1);
				}			
				RxLenU2 = 0;				
			}
		}
		
		if(RxLenU2>=PACK_END)
		{
			RxLenU2 = 0;
		}
	}
}

/*
	数据发送
	数据格式：
	包头 | 数据类型 | 数据... | 校验位 | 包尾
	
	校验位： 
	校验位 = 包头^数据类型^数据...^包尾
*/

void sendHexMessageToUsart_2(byte *data,byte dataType,unsigned int dataLen)
{
	uint16 len = 3+dataLen+1,i=0,j=0;
	byte readyData[PACK_MAX_LEN] = {0};
	if(PACK_MAX_LEN!=0&&len>PACK_MAX_LEN)
		return ;
	readyData[0] = PACK_HEAD;							// 包头
	readyData[1] = dataType;							// 数据类型
	
	for(i=0;i<(len-2);i++)
	{
		if(i!=0&&i!=1)
		{
			readyData[i] = data[j++];
		}
		readyData[len-2] ^=readyData[i];
	}
	
	readyData[len-1] = PACK_END;          // 包尾
	readyData[len-2] ^= readyData[len-1]; // 校验位
	
	// 发送数据 Usart1
	i=0;
	while(i<len)
	{
		putCharToU2(readyData[i++]);
	}
	
	return ;
}

int putCharToU2 (int c) 
{
  while (!(USART2->SR & USART_SR_TXE));
  USART2->DR = (c & 0x1FF);
  return (c);
}

int getCharFromU2 (void) 
{
//   while (!(USART1->SR & USART_SR_RXNE));
  return (USART2->DR & 0xFF);
}

int checkCharFromU2 (void) 
{
  if (USART2->SR & USART_SR_RXNE)
    return (1);
  else
    return (0);
}

/*************************************************** USART 2 end ***************************************************/



/*************************************************** USART 3 begin ***************************************************/
void usart3Init(void)
{
	int i=0x1000;
	// AFIO
	RCC->APB2ENR |= (1UL<<0);
 	AFIO->MAPR |= (1UL<<4); // 部分重映像
	
	// GPIOC
	RCC->APB2ENR |= (1UL<<2); 
	GPIOC->CRH &= ~(0xff00ul);  // PC10,PC11
	GPIOC->CRH |= (0X0B<<8);  
	GPIOC->CRH |= (0X04<<12);  
	
	RCC->APB1ENR |= (1UL << 18);
	// USART3
	USART3->CR1 = 0X002C;
	USART3->CR2 = 0X0000; // 1 stop bit
	USART3->CR3 = 0X0000; // no flow control
	USART3->BRR = 0x0EA0; // 19200
	
	// it
	NVIC->ISER[1] = (1ul<<7);
	
	while(i--) __NOP();
	
	USART3->CR1 |= 0x2000; // enable 
}


// 数据接收
void USART3_IRQHandler(void)
{
	if(USART3->SR & (1UL<<5))
	{
		reBufU3[RxLenU3] = getCharFromU3();
		RxLenU3++;
		
		if(reBufU3[RxLenU3-1] == PACK_END)
		{
			if(reBufU3[0]== PACK_HEAD)
			{
				// check checkFlag
				if((dataXOR(reBufU3, RxLenU3-2)^PACK_END)== reBufU3[RxLenU3-2])
				{
					sendHexMessageToUsart_3(&RxLenU3, 0x03, 1);
				}			
				RxLenU3 = 0;				
			}
		}
		
		if(RxLenU3>=PACK_END)
		{
			RxLenU3 = 0;
		}
	}
}

/*
	数据发送
	数据格式：
	包头 | 数据类型 | 数据... | 校验位 | 包尾
	
	校验位： 
	校验位 = 包头^数据类型^数据...^包尾
*/

void sendHexMessageToUsart_3(byte *data,byte dataType,unsigned int dataLen)
{
	uint16 len = 3+dataLen+1,i=0,j=0;
	byte readyData[PACK_MAX_LEN] = {0};
	if(PACK_MAX_LEN!=0&&len>PACK_MAX_LEN)
		return ;
	readyData[0] = PACK_HEAD;							// 包头
	readyData[1] = dataType;							// 数据类型
	
	for(i=0;i<(len-2);i++)
	{
		readyData[len-2] ^=readyData[i];
		if(i!=0&&i!=1)
		{
			readyData[i] = data[j++];
		}
	}
	
	readyData[len-1] = PACK_END;          // 包尾
	readyData[len-2] ^= readyData[len-1]; // 校验位
	
	// 发送数据 Usart1
	i=0;
	while(i<len)
	{
		putCharToU3(readyData[i++]);
	}
	
	return ;
}

int putCharToU3 (int c) 
{
  while (!(USART3->SR & USART_SR_TXE));
  USART3->DR = (c & 0x1FF);
  return (c);
}

int getCharFromU3 (void) 
{
//   while (!(USART1->SR & USART_SR_RXNE));
  return (USART3->DR & 0xFF);
}

int checkCharFromU3 (void) 
{
  if (USART3->SR & USART_SR_RXNE)
    return (1);
  else
    return (0);
}

/*************************************************** USART 3 end ***************************************************/


/*************************************************** Tools ***************************************************/
byte dataXOR(byte *tmpData, byte len)
{
	int i=0;
	byte resData=0;
	for(i=0;i<len;i++)
	{
		resData ^= tmpData[i];
	}
	return resData;
}

/*
	重定向 到U1 用于调试
*/
int fputc(int c, FILE *f) {
  if (c == '\n')  {
    putCharToU1('\r');
  }
  return (putCharToU1(c));
}


int fgetc(FILE *f) {
  return (getCharFromU1());
}


int fclose(FILE* f) {
  return (0);
}


int fseek (FILE *f, long nPos, int nMode)  {
  return (0);
}


int fflush (FILE *f)  {
  return (0);
}

int ferror(FILE *f) {
  /* Your implementation of ferror */
  return (EOF);
}


void _ttywrch(int c) {
  putCharToU1(c);
}


void _sys_exit(int return_code) {
label:  goto label;  /* endless loop */
}

