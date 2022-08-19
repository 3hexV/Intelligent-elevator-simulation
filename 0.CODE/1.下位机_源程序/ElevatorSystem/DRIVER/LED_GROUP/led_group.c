/************************************Info************************************
	+����LED������������ v1.0
	+�������Ĵ���
	+�ͺţ�/
	+ʹ�ã�
				ledGroupInit(); //��ʼ��
	+�ӿڣ�
				LED1  		<==>  PD0
				LED2  		<==>  PD1
				Fire_LED  <==>  PD2
				RFID_LED  <==>  PA0(�ӻ�)
	+��ע��
				RFID_LEDΪ�ӻ����ƣ���������������
*************************************************************************/
#include "stm32f10x.h"
#include "led_group.h"
#include "main.h"

// LED��ʼ��
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

// LED�����л�ģʽ
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

