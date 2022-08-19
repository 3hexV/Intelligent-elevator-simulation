/************************************Info************************************
	+程序：数码管驱动程序 v1.0
	+开发：寄存器
	+型号：/
	+使用：
				rgbLedInit(); //初始化
	+接口：
				GND   				<==> GND
				Data   Line  	<==> PB8-14
				
	+备注：
				共阴数码管
*************************************************************************/
#include "smg.h"
#include "stm32f10x.h"
#include "main.h"

int ExSmgNum=14;

// 断码表 共阴
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


// 数码管初始化
void smgInit(void)
{	
	static int i=0;
	RCC->APB2ENR |= (1ul<<3); // PB
	
	GPIOB->CRH &= ~0x0fffffff;
	GPIOB->CRH |= 0x01111111;
	GPIOB->ODR |= 0x7F00; // 全部不选中
	
	// 数码管自检
	for(i=0;i<16;i++)
	{
		smgShowNum(i);
		delay_ms(100);
	}
	smgShowNum(14);
	
}


// 某位 数码管 显示某个数字
void smgShowNum(int num)
{
	// 清空整体 数据
 	GPIOB->ODR &= ~(0x7F00);
	
	// 发送段 数据
	GPIOB->ODR |= numList[num]<<8;
}

