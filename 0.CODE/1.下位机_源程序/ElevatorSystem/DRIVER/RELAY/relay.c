/************************************Info************************************
	+程序：继电器驱动程序 v1.0
	+开发：寄存器
	+型号：/
	+使用：
				relayInit(); //初始化
	+接口：
				GND  <==> GND
				VCC  <==> 3.3V
				S		 <==> PA8
				
	+备注：
				继电器使用的高电平驱动
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
	
	// 继电器自检
	while(i--)
	{
		relayEnable();
		delay_ms(500);
		relayDisable();
		delay_ms(500);
	}
}

// 吸合继电器
void relayEnable(void)
{
	GPIOA->ODR |= ((1ul<<0)<<8);
}

// 断开继电器
void relayDisable(void)
{
	GPIOA->ODR &= ~((1ul<<0)<<8);
}
