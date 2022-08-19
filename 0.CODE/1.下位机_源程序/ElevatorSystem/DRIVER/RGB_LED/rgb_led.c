/************************************Info************************************
	+程序：多色RGB灯驱动程序 v1.0
	+开发：寄存器
	+型号：/
	+使用：
				rgbLedInit(); //初始化
				rgbLedShow(1, 1, 1);
				rgbLedShow(1, 0, 0);
	+接口：
				VCC   <==> 3.3V
				GND   <==> GND
				Red   <==> PA0 
				Green <==> PA1
				Bule  <==> PA2
	+备注：
				3种基本色，4种合成色
*************************************************************************/

#include "stm32f10x.h"
#include "rgb_led.h"
#include "main.h"


// RGB初始化
void rgbLedInit(void)
{
	RCC->APB2ENR |= (1ul<<2); // PA
	
	GPIOA->CRL &= ~(0x00000FFF);
	GPIOA->CRL |= (0x00000111);
	
	rgbLedShow(0,0,0);
	RCC->APB2ENR |= (1ul<<2); // PA
	
	GPIOA->CRH &= ~(0xFFF00000);
	GPIOA->CRH |= (0x11100000);
	
	// RGB自检
	rgbLedShow(1,0,0);
	delay_ms(500);
	rgbLedShow(0,1,0);
	delay_ms(500);
	rgbLedShow(0,0,1);
	delay_ms(500);

	rgbLedShow(1,1,0);
	delay_ms(500);
	rgbLedShow(1,0,1);
	delay_ms(500);
	rgbLedShow(0,1,1);
	delay_ms(500);
	rgbLedShow(0,0,0);
}

void rgbLedShow(int R, int G, int B)
{
	GPIOA->ODR &= ~(0x0007);
	if(R==1)
	{
		GPIOA->ODR |= (1ul<<0);
	}
	if(G==1)
	{
		GPIOA->ODR |= (1ul<<1);
	}
	if(B==1)
	{
		GPIOA->ODR |= (1ul<<2);
	}
}

// Red Green Blue
void DTRunMode(int mode)
{
	if((mode == DT_ReadyStop)|(mode == DT_ReadyRun))
	{
		rgbLedShow(0, 0, 1);
		delay_s(T_RUN_STOP_READ);
		//rgbLedShow(0, 0, 0);
	}else if(mode == DT_Stoped)
	{
		rgbLedShow(0, 1, 0);
		//rgbLedShow(0, 0, 0);
	}else if(mode == DT_Runing)
	{
		rgbLedShow(1, 0, 0);
	}
}

