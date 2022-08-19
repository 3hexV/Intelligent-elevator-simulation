/************************************Info************************************
	+���򣺶��������������� v1.0
	+�������Ĵ���
	+�ͺţ�/
	+ʹ�ã�
				dlKeyInit();//��ʼ��
	+�ӿڣ�
				VCC <==> 5V/3.3V
				GND <==> GND
				IN <==> PA0
	+��ע��
				EXTI0�ⲿ�жϴ���
*************************************************************************/

#include "dl_key.h"
#include "stm32f10x.h"

void dlKeyInit(void)
{
	RCC->APB2ENR |= (1ul<<0)|(1ul<<2); // AFIO PA
	
	GPIOA->CRL &= ~(0x0000000F);
	GPIOA->CRL |= (0x00000008);
	GPIOA->ODR |= (1ul<<0);
	
	AFIO->EXTICR[0] |= (0ul<<0)<<0; // EXTI0
	
	EXTI->IMR |= (1ul<<0);
	EXTI->FTSR |= (1ul<<0);
	
	NVIC->ISER[0] |= (1ul<<6);
}
