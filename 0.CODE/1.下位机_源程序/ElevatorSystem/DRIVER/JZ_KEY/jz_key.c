/************************************Info************************************
	+程序：矩阵键盘驱动 v1.0
	+开发：寄存器
	+型号：/
	+使用：
				jzKeyInit(); //初始化
				tmpKey = jzKeyScanf();
				if(tmpKey!=0x00)
				{
					putCharToU1(tmpKey);
				}
	+接口：
				Data Line <==> PC0-7
	+备注：
				用在While(1)中
*************************************************************************/
#include "jz_key.h"
#include "stm32f10x.h"
#include "main.h"

// 按键映射表
byte key_num_list[17] = {
	0x00, // null
	0x18, // S1
	0x14, // S2
	0x12, // S3
	0x11, // S4
	
	0x28, // S5
	0x24, // S6
	0x22, // S7
	0x21, // S8
	
	0x48, // S9
	0x44, // S10
	0x42, // S11
	0x41, // S12
	
	0x88, // S13
	0x84, // S14
	0x82, // S15
	0x81, // S16
};

// 矩阵键盘初始化
void jzKeyInit(int ARR,int PSC)
{
	RCC->APB2ENR |= (1ul<<4); //PC
	jzKeyMode(1);
}

// 矩阵键盘扫描
byte jzKeyScanf(void)
{
	int tmp = jzKeyScanf0_3() | jzKeyScanf4_7();
	if(((tmp&0x0f)==0x00)||((tmp&0xf0)==0x00)) return (byte)0x00;
	jzKeyMode(3);
	return (byte)tmp;
}

// 矩阵键盘扫描低位 
int jzKeyScanf0_3(void)
{
	int tmp=0;
	
	jzKeyMode(2);
	
	if(GPIOE->IDR & (0x0ful)){
		delay_ms(KEY_SOFT_DELAY);
		if(GPIOE->IDR & (0x0ful)){
			tmp = GPIOE->IDR & (0x0ful);		
		}
	}
	return tmp;
}

// 矩阵键盘扫描高位
int jzKeyScanf4_7(void)
{
	int tmp=0;
	
	jzKeyMode(1);
	
	if(GPIOE->IDR & (0xf0ul)){
		delay_ms(KEY_SOFT_DELAY);
		if(GPIOE->IDR & (0xf0ul)){
			tmp = GPIOE->IDR & (0xf0ul);		
		}
	}
	return tmp;
}

// 1: 	0-3: output 4-7: input
// 2: 	0-3: input  4-7: output
// 矩阵键盘 行列的输入输出模式切换
void jzKeyMode(int mode)
{
		if(mode==1)
		{
			GPIOE->CRL = 0x88881111;
			GPIOE->ODR |= 0x000F;
			GPIOE->ODR &= ~0x00F0;
		}else if(mode==2)
		{
			GPIOE->CRL = 0x11118888;
			GPIOE->ODR |= 0x00F0;
			GPIOE->ODR &= ~0x000F;
		}else if(mode==3)
		{
			GPIOE->CRL = 0x88881111;
			GPIOE->ODR |= 0x00FF;
		}
}



