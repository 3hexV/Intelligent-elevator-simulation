/************************************Info************************************
	+���򣺼̵����������� v1.0
	+�������Ĵ���
	+�ͺţ�/
	+ʹ�ã�
				relayInit(); //��ʼ��
	+�ӿڣ�
				GND  <==> GND
				VCC  <==> 3.3V
				S		 <==> PA8
				
	+��ע��
				�̵���ʹ�õĸߵ�ƽ����
*************************************************************************/
#include "stm32f10x.h"
#include "relay.h"
#include "main.h"

void relayInit(void)
{
	static int i=3;
	RCC->APB2ENR |= (1ul<<2); // PA
	
	GPIOA->CRH &= ~0x0000000F; //PA8
	GPIOA->CRH |= 0x00000001;
	
	// �̵����Լ�
	while(i--)
	{
		relayEnable();
		delay_ms(500);
		relayDisable();
		delay_ms(500);
	}
}

// ���ϼ̵���
void relayEnable(void)
{
	GPIOA->ODR |= ((1ul<<0)<<8);
}

// �Ͽ��̵���
void relayDisable(void)
{
	GPIOA->ODR &= ~((1ul<<0)<<8);
}
