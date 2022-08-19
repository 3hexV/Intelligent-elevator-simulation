/************************************Info************************************
	+程序：光感、火焰传感器驱动程序 v1.0
	+开发：寄存器
	+型号：/
	+使用：
				SLightInit(2999,7299); //初始化
	+接口：
				光感DO  <==>  PA11
				火焰DO  <==>  PD3
	+备注：
				全部使用数字量输出，阈值使用可填电阻控制
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

// 光感、火焰初始化
void SLightInit(int ARR, int PSC)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructer;
	NVIC_InitTypeDef NVIC_InitStructer;
	
	RCC->APB2ENR |= (1ul<<2)|(1ul<<5); // PA PD
	
	GPIOA->CRH &= ~0x0000F000; //PA11
	GPIOA->CRH |= 0x00008000;
	
	GPIOD->CRL &= ~0x0000F000; //PD3
	GPIOD->CRL |= 0x00008000;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//使能定时器3时钟
	TIM_DeInit(TIM4);
	TIM_TimeBaseInitStructer.TIM_Period=ARR;//定时周期为ARR
	TIM_TimeBaseInitStructer.TIM_Prescaler=PSC; //分频系数PSC
	TIM_TimeBaseInitStructer.TIM_ClockDivision=TIM_CKD_DIV1;//不分频
	TIM_TimeBaseInitStructer.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructer);

	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);//开启更新中断
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructer.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级
	NVIC_InitStructer.NVIC_IRQChannelSubPriority=1;//响应优先级
	NVIC_InitStructer.NVIC_IRQChannel=TIM4_IRQn;
	NVIC_InitStructer.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructer);
	
	TIM_Cmd(TIM4,DISABLE);
}

// 读取火焰数据
byte readSFireData(void)
{
	return (GPIOD->IDR&(1ul<<3))>>3;
}

// 读取光感数据
byte readSLightData(void)
{
	return (GPIOA->IDR&(1ul<<11))>>11;
}

// 扫描火焰数据 并控制报警灯
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

// 扫描光感数据 并控制光的挡位
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

// TIM4的中断服务函数
void TIM4_IRQHandler(void) //中断
{
	static int i=0;
	byte tmpKey=0;
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)!=RESET)
	{	
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);//清除中断标志
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

