/************************************Info************************************
	+���򣺹�С����洫������������ v1.0
	+�������Ĵ���
	+�ͺţ�/
	+ʹ�ã�
				SLightInit(2999,7299); //��ʼ��
	+�ӿڣ�
				���DO  <==>  PA11
				����DO  <==>  PD3
	+��ע��
				ȫ��ʹ���������������ֵʹ�ÿ���������
*************************************************************************/
#include "stm32f10x.h"
#include "main.h"
#include "s_light.h"
#include "config.h"
#include "led_group.h"
#include "1602.h" 

byte lastData=0;
byte SLightData=0, SFireData=0;

extern int ExSmgNum,sysRunFlag;

// ��С������ʼ��
void SLightInit(int ARR, int PSC)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructer;
	NVIC_InitTypeDef NVIC_InitStructer;
	
	RCC->APB2ENR |= (1ul<<2)|(1ul<<5); // PA PD
	
	GPIOA->CRH &= ~0x0000F000; //PA11
	GPIOA->CRH |= 0x00008000;
	
	GPIOD->CRL &= ~0x0000F000; //PD3
	GPIOD->CRL |= 0x00008000;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//ʹ�ܶ�ʱ��3ʱ��
	TIM_DeInit(TIM4);
	TIM_TimeBaseInitStructer.TIM_Period=ARR;//��ʱ����ΪARR
	TIM_TimeBaseInitStructer.TIM_Prescaler=PSC; //��Ƶϵ��PSC
	TIM_TimeBaseInitStructer.TIM_ClockDivision=TIM_CKD_DIV1;//����Ƶ
	TIM_TimeBaseInitStructer.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructer);

	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);//���������ж�
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructer.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
	NVIC_InitStructer.NVIC_IRQChannelSubPriority=1;//��Ӧ���ȼ�
	NVIC_InitStructer.NVIC_IRQChannel=TIM4_IRQn;
	NVIC_InitStructer.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructer);
	
	TIM_Cmd(TIM4,DISABLE);
}

// ��ȡ��������
byte readSFireData(void)
{
	return (GPIOD->IDR&(1ul<<3))>>3;
}

// ��ȡ�������
byte readSLightData(void)
{
	return (GPIOA->IDR&(1ul<<11))>>11;
}

// ɨ��������� �����Ʊ�����
void scanfFire(void)
{
	SFireData = readSFireData();
	
	sendHexMessageToUsart_3(&SFireData, 0x03, 1);
	if(SFireData==0x01) {
		ledGroupLightMode(LG_FLASH_1);
	}
	else {
		ledGroupLightMode(LG_FLASH_0);
	}
}

// ɨ�������� �����ƹ�ĵ�λ
void controlLight(void)
{
	SLightData = readSLightData();
	
	sendHexMessageToUsart_3(&SLightData, 0x04, 1);
	
	if(SLightData==0x01) {
		ledGroupLightMode(LG_ON_2);
	}
	else {
		ledGroupLightMode(LG_ON_1);
	}
}

// TIM4���жϷ�����
void TIM4_IRQHandler(void) //�ж�
{
	static int i=0;
	byte tmpKey=0;
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)!=RESET)
	{	
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);//����жϱ�־
		//sysRunShow();
		
		if(i==0) 
		{
			controlLight();
			i++;
		}else if(i==1)
		{
			scanfFire();
			i = 0;
		}
		smgShowNum(ExSmgNum);
		
	}
}

