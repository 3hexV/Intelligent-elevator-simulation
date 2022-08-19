/************************************Info************************************
	+����main����
	+��ע��
				delay_msʹ����SysTick
*************************************************************************/

#include "stm32f10x.h"
#include "main.h"
#include "stdio.h"

extern int RFID_Data, JZ_Key_Data;

int main(void)
{
	byte tmpKey=0;

 	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	
	init1602();
	
	showInitProcess(0);
 	BYJ_48_Init(39,7199);//���������ʼ�����õ�TIM3��
  usart1Init();
	//usart2Init();
  usart3Init();
	
	
	showInitProcess(1);
	MAX7219Init();
	
	showInitProcess(2);
 	smgInit();
	
	showInitProcess(3);
 	fmqInit();
	
	showInitProcess(4);
 	rgbLedInit();
	
	showInitProcess(5);
 	relayInit();
	
	showInitProcess(6);
 	ledGroupInit();
	
	showInitProcess(7);
  SLightInit(2999,7199); //0.1ms*arr TIM4
	
	
  showInformation();
	
	printf("System Successful\n");
	
 	TIM_Cmd(TIM4,ENABLE);
 	TIM_Cmd(TIM5,ENABLE);
	
	DTStart();
	
	while(1)
	{
 		sysRunShow();
		run();
		if(RFID_Data==0)
		{
			DTStop();
			DTStart();
			RFID_Data = 1;		
		}
	}
}

// max:1.8s
void delay_ms(u16 nms)
{
	SysTick->CTRL &= 0xfffffffb;
	SysTick->VAL = 0UL;
	SysTick->LOAD = 8*nms*1000;
	SysTick->CTRL |= (1ul<<0);
	while(!(SysTick->CTRL&(1ul<<16)));
	SysTick->CTRL &= ~(1ul<<0);
	SysTick->VAL = 0ul;
}

void delay_s(u16 ns)
{
	while(ns--) delay_ms(1000);
}


