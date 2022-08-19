/************************************Info************************************
	+程序：蜂鸣器驱动程序 v1.0
	+开发：寄存器
	+型号：/
	+使用：
				fmqInit(); //初始化
				fmgAble(); // 发声
				fmgDisable(); // 不发声
	+接口：
				VCC <==> 5V/3.3V
				GND <==> GND
				IN <==> PA3
	+备注：
				分为低电平/高电平触发，此蜂鸣器使用高电平驱动
*************************************************************************/
#include "stm32f10x.h"
#include "fmq.h"
#include "main.h"

// 蜂鸣器 初始化
void fmqInit(void)
{
	RCC->APB2ENR |= (1ul<<2); // PA
	
	GPIOA->CRL &= ~0x0000F000;
	GPIOA->CRL |= 0x00001000;
	
	// 蜂鸣器自检
	fmqEnable();
	
	delay_s(1);
	
	fmqDisable();
}

// 蜂鸣器 使能
void fmqEnable(void)
{
	GPIOA->ODR |= (1ul<<3);
}

// 蜂鸣器 失能
void fmqDisable(void)
{
	GPIOA->ODR &= ~(1ul<<3);
}

// 蜂鸣器 鸣叫一声
void fmqLaunch(void)
{
	fmqEnable();
	delay_ms(100);
	fmqDisable();
}


