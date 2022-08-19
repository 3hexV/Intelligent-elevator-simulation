/************************************Info************************************
	+����������������� v1.0
	+�������Ĵ���
	+�ͺţ�/
	+ʹ�ã�
				rgbLedInit(); //��ʼ��
	+�ӿڣ�
				GND   				<==> GND
				Data   Line  	<==> PB8-14
				
	+��ע��
				���������
*************************************************************************/
#include "smg.h"
#include "stm32f10x.h"
#include "main.h"

int ExSmgNum=14;

// ����� ����
int numList[16] = {
			0x3F, // 0
			0x06, // 1
			0x5B, // 2 
			0x4F, // 3 
			0x66, // 4 
			0x6D, // 5 			
			0x7D, // 6 
			0x07, // 7 
			0x7F, // 8 
			0x6F, // 9 
			0x77, // a 
			0x7C, // b 
			0x39, // c 
			0x5E, // d 
			0x79, // e 
			0x71  // f
};


// ����ܳ�ʼ��
void smgInit(void)
{	
	static int i=0;
	RCC->APB2ENR |= (1ul<<3); // PB
	
	GPIOB->CRH &= ~0x0fffffff;
	GPIOB->CRH |= 0x01111111;
	GPIOB->ODR |= 0x7F00; // ȫ����ѡ��
	
	// ������Լ�
	for(i=0;i<16;i++)
	{
		smgShowNum(i);
		delay_ms(100);
	}
	smgShowNum(14);
	
}


// ĳλ ����� ��ʾĳ������
void smgShowNum(int num)
{
	// ������� ����
 	GPIOB->ODR &= ~(0x7F00);
	
	// ���Ͷ� ����
	GPIOB->ODR |= numList[num]<<8;
}

