/************************************Info************************************
	+���򣺷������������� v1.0
	+�������Ĵ���
	+�ͺţ�/
	+ʹ�ã�
				fmqInit(); //��ʼ��
				fmgAble(); // ����
				fmgDisable(); // ������
	+�ӿڣ�
				VCC <==> 5V/3.3V
				GND <==> GND
				IN <==> PA3
	+��ע��
				��Ϊ�͵�ƽ/�ߵ�ƽ�������˷�����ʹ�øߵ�ƽ����
*************************************************************************/
#include "stm32f10x.h"
#include "fmq.h"
#include "main.h"

// ������ ��ʼ��
void fmqInit(void)
{
	RCC->APB2ENR |= (1ul<<2); // PA
	
	GPIOA->CRL &= ~0x0000F000;
	GPIOA->CRL |= 0x00001000;
	
	// �������Լ�
	fmqEnable();
	
	delay_s(1);
	
	fmqDisable();
}

// ������ ʹ��
void fmqEnable(void)
{
	GPIOA->ODR |= (1ul<<3);
}

// ������ ʧ��
void fmqDisable(void)
{
	GPIOA->ODR &= ~(1ul<<3);
}

// ������ ����һ��
void fmqLaunch(void)
{
	fmqEnable();
	delay_ms(100);
	fmqDisable();
}


