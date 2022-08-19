/**
  * @attention
  *
  * 实验平台:STM32F103ZET6、HSE8MHZ、RC522、ISO14443A、50卡3张；
	* 软件：   MDK Keil4、窗口调试助手
  * 测试结果：识别id、值块读写
	* 硬件连线：STM32<====>RC522
	*						   5V<---->  VCC
  *       			PB0<---->  RST
  *       			GND<---->  GND
  *       			PA6<---->  MISO
  *        			PA7<---->  MOSI
  *       			PA5<---->  SCK
  *       			PA4<---->  SDA
  ******************************************************************************
 */
  
#include "main.h"
#include "usart.h"
#include "stdio.h"
#include "jz_key.h"

int main (void)
{ 
	byte tmpKey=0, sendKeyData=0;
	SysTick_Init();
	jzKeyInit();
	usart1Init();
	usart2Init();
	//usart3Init();
	printf("Slave System Successful\n");
	
	RC522_Init();  /*RC522模块所需外设的初始化配置*/
	
	PcdReset();  
 
	M500PcdConfigISOType('A');/*设置工作方式*/
	
	// RFID Work State
	RCC->APB2ENR |= (1ul<<2);
	GPIOA->CRL &= ~0x0000000F;
	GPIOA->CRL |= 0x00000001;
	
	GPIOA->ODR |= 0x00000001;
	Delay_us(1000*100);
	GPIOA->ODR &= ~0x00000001;
	Delay_us(1000*100);

  while(1){			
    HF_Tag_test (); 
		tmpKey = jzKeyScanf();
		if(tmpKey!=0x00) 
		{
			switch(tmpKey)
			{
				case 0x18: sendKeyData=0x12; break; // 1 DOWN
				case 0x14: sendKeyData=0x12; break; // 1 DOWN
				
				case 0x12: sendKeyData=0x21; break; // 2 UP
				case 0x11: sendKeyData=0x22; break; // 2 DOWN
				
				case 0x28: sendKeyData=0x31; break; // 3 UP
				case 0x24: sendKeyData=0x32; break; // 3 DOWN
				
				case 0x22: sendKeyData=0x41; break; // 4 UP
				case 0x21: sendKeyData=0x42; break; // 4 DOWN
				
				case 0x48: sendKeyData=0x52; break; // 5 DOWN
				case 0x44: sendKeyData=0x52; break; // 5 DOWN
				
				case 0x42: sendKeyData=0x01; break; // F_1 
				case 0x41: sendKeyData=0x02; break; // F_2 
				
				case 0x88: sendKeyData=0x03; break; // F_3 
				case 0x84: sendKeyData=0x04; break; // F_4 
				case 0x82: sendKeyData=0x05; break; // F_5 
				case 0x81: sendKeyData=0x00; break; // / 
				default: sendKeyData = 0xFF;break;
			}
			
			sendHexMessageToUsart_1(&sendKeyData, 0x22, 1);
			sendHexMessageToUsart_2(&sendKeyData, 0x22, 1);
		//	sendHexMessageToUsart_3(&tmpKey, 0x31, 1);
		}		
	}	  
}
