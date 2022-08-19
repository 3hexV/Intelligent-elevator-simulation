/************************************Info************************************
	+���򣺶�ɫRGB���������� v1.0
	+�������Ĵ���
	+�ͺţ�/
	+ʹ�ã�
				rgbLedInit(); //��ʼ��
				rgbLedShow(1, 1, 1);
				rgbLedShow(1, 0, 0);
	+�ӿڣ�
				VCC   <==> 3.3V
				GND   <==> GND
				Red   <==> PA0 
				Green <==> PA1
				Bule  <==> PA2
	+��ע��
				3�ֻ���ɫ��4�ֺϳ�ɫ
*************************************************************************/

#include "stm32f10x.h"
#include "rgb_led.h"
#include "main.h"


// RGB��ʼ��
void rgbLedInit(void)
{
	RCC->APB2ENR |= (1ul<<2); // PA
	
	GPIOA->CRL &= ~(0x00000FFF);
	GPIOA->CRL |= (0x00000111);
	
	rgbLedShow(0,0,0);
	RCC->APB2ENR |= (1ul<<2); // PA
	
	GPIOA->CRH &= ~(0xFFF00000);
	GPIOA->CRH |= (0x11100000);
	
	// RGB�Լ�
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

