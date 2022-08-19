/************************************Info************************************
	+程序：LED灯组驱动程序 v1.0
	+开发：寄存器
	+型号：/
	+使用：
				ledGroupInit(); //初始化
	+接口：
				LED1  		<==>  PD0
				LED2  		<==>  PD1
				Fire_LED  <==>  PD2
				RFID_LED  <==>  PA0(从机)
	+备注：
				RFID_LED为从机控制，其他由主机控制
*************************************************************************/
#include "stm32f10x.h"
#include "led_group.h"
#include "main.h"

// LED初始化
void ledGroupInit(void)
{
	int i=0,j=3*3;
	RCC->APB2ENR |= (1ul<<5); // PD
	
	GPIOD->CRL &= ~0x00000FFF; //PD0,1,2
	GPIOD->CRL |= 0x00000111;

	while(j--)
	{
		GPIOD->ODR &= ~(1ul<<i);
		delay_ms(100);
		GPIOD->ODR |= (1ul<<i);
		delay_ms(100);
		i = (i+1)%3;
	}

	GPIOD->ODR |= (1ul<<2); 
	ledGroupLightMode(LG_ON_1);
}

// LED灯组切换模式
void ledGroupLightMode(u16 mode)
{
	if(mode==LG_OFF)
	{
		GPIOD->ODR |= ((1ul<<0)|(1ul<<1));
	}else if(mode==LG_ON_1)
	{
		GPIOD->ODR |= ((1ul<<0)|(1ul<<1));
		GPIOD->ODR &= ~(1ul<<0); 
	}else if(mode==LG_ON_2)
	{	
		GPIOD->ODR &= ~((1ul<<0)|(1ul<<1)); 
	}else if(mode==LG_FLASH_0)
	{	
		GPIOD->ODR &= ~(1ul<<2);
	}else if(mode==LG_FLASH_1)
	{	
		GPIOD->ODR |= (1ul<<2);
	}
}

