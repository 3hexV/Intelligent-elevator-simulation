#include"rc522_test.h"
#include <stdbool.h>
#include "user_SysTick.h"
#include "rc522_config.h"
#include "rc522_function.h"
#include "rc522_test.h"
#include "string.h"
#include "usart.h"

#define HF_MODE_WORK					// 工作模式
//#define HF_MODE_WRITE_TURE  // 写入正确的门密钥
//#define HF_MODE_WRITE_FALSE // 写入错误的门密钥

uint8_t KeyValue[]={0xFF ,0xFF, 0xFF, 0xFF, 0xFF, 0xFF};   // 卡A密钥
uint8_t DoorKey[]={0x20 ,0x12,0x20 ,0x03, 0x09, 0x30, 0x87, 0x98};   // Door key
uint8_t T_DoorKey[]={0x20 ,0x12,0x20 ,0x03, 0x09, 0x30, 0x87, 0x98};   //True Door key
uint8_t F_DoorKey[]={0x21 ,0x12,0x20 ,0x03, 0x09, 0x30, 0x87, 0x98};   //False Door key

void HF_Tag_test ( void )
{
	int i=0;
	uint8_t readValue[8];
	byte checkFlag=0;
  uint8_t ucArray_ID [ 4 ];    /*先后存放IC卡的类型和UID(IC卡序列号)*/                                                                                         
	uint8_t ucStatusReturn;      /*返回状态*/                                                                                           
   
    /*寻卡*/
		if ( ( ucStatusReturn = PcdRequest ( PICC_REQIDL, ucArray_ID ) ) != MI_OK )  
       /*若失败再次寻卡*/
			ucStatusReturn = PcdRequest ( PICC_REQIDL, ucArray_ID );		                                                

		if ( ucStatusReturn == MI_OK  )
		{
      /*防冲撞（当有多张卡进入读写器操作范围时，防冲突机制会从其中选择一张进行操作）*/
			if ( PcdAnticoll ( ucArray_ID ) == MI_OK )                                                                   
			{
				PcdSelect(ucArray_ID);			
				
				PcdAuthState( PICC_AUTHENT1A, 0x11, KeyValue, ucArray_ID );//校验密码 
				
				#ifdef HF_MODE_WRITE_TURE
					PcdWrite(0x11, T_DoorKey);
				#else
					#ifdef HF_MODE_WRITE_FALSE
						PcdWrite(0x11, F_DoorKey);
					#else // working
						//...
					#endif
					
				#endif
				//PcdWrite(0x11, DoorKey);
				
				if(PcdRead(0x11, readValue) == MI_OK)	
				{
					//sprintf ( cStr, "The Card ID is: %02X%02X%02X%02X",ucArray_ID [0], ucArray_ID [1], ucArray_ID [2],ucArray_ID [3] );
					//printf ( "%s\r\n",cStr );  //打印卡片ID
					
					for(i=0;i<8;i++)
					{
						if(readValue[i]!=DoorKey[i])
						{
							checkFlag = 1;
							//printf ("Check Error!\r\n");
							sendHexMessageToUsart_1(&checkFlag, 0x21, 1);
							sendHexMessageToUsart_2(&checkFlag, 0x21, 1);

							GPIOA->ODR |= 0x00000001;
							Delay_us(1000*100);
							GPIOA->ODR &= ~0x00000001;
							Delay_us(1000*100);
							
							GPIOA->ODR |= 0x00000001;
							Delay_us(1000*100);
							GPIOA->ODR &= ~0x00000001;
							Delay_us(1000*100);
							goto SKIP_SUC;
						}
					}
					
						//printf ("Check Pass\r\n");
						sendHexMessageToUsart_1(&checkFlag, 0x21, 1);
						sendHexMessageToUsart_2(&checkFlag, 0x21, 1);
						//sendHexMessageToUsart_3(&checkFlag, 0x32, 1);
						GPIOA->ODR |= 0x00000001;
						Delay_us(1000*100);
						GPIOA->ODR &= ~0x00000001;
						Delay_us(1000*100);
						checkFlag = 0;

        	 SKIP_SUC:										 	         
          PcdHalt();
				}
				
			}				
		}		
}
