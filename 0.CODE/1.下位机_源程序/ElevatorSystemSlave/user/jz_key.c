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
#include "user_SysTick.h"
#include "usart.h"
/*
	
*/

// DATA Line: 

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


void jzKeyInit(void)
{
	RCC->APB2ENR |= (1ul<<4); //PC
	jzKeyMode(1);

//	Tim3_init(999, 7199); 
}

void Tim3_init(u16 arr, u16 psc)
{
	RCC->APB1ENR |= (1ul<<1); // TIIM3
	// TIM3 init
	TIM3->ARR = arr;
	TIM3->PSC = psc;
	TIM3->CR1 &= ~(1ul<<4);
	TIM3->DIER |= (1ul<<0);

	NVIC->ISER[0] |= (1ul<<29); // TIM3 
//	TIM3->CR1 |= (1ul<<0);
}


void TIM3_IRQHandler(void)
{
	if(TIM3->SR&(1ul<<0)){
		TIM3->SR &= ~(1ul<<0);
	}
}


byte jzKeyScanf(void)
{
	int tmp = jzKeyScanf0_3() | jzKeyScanf4_7();

	if(((tmp&0x0f)==0x00)||((tmp&0xf0)==0x00)) return (byte)0x00;

	return (byte)tmp;
}

int jzKeyScanf0_3(void)
{
	int tmp=0;

	jzKeyMode(2);

	if(GPIOC->IDR & (0x0ful)){
		Delay_us(KEY_SOFT_DELAY);
		if(GPIOC->IDR & (0x0ful)){
			tmp = GPIOC->IDR & (0x0ful);		
		}
	}

	return tmp;
}

int jzKeyScanf4_7(void)
{
	int tmp=0;

	jzKeyMode(1);

	if(GPIOC->IDR & (0xf0ul)){
		Delay_us(KEY_SOFT_DELAY);
		if(GPIOC->IDR & (0xf0ul)){
			tmp = GPIOC->IDR & (0xf0ul);	
		}
	}
	return tmp;
}

// 1: 	0-3: output 4-7: input
// 2: 	0-3: input  4-7: output
void jzKeyMode(int mode)
{
		if(mode==1)
		{
			GPIOC->CRL = 0x88881111;
			GPIOC->ODR |= 0x000F;
			GPIOC->ODR &= ~0x00F0;
		}else if(mode==2)
		{
			GPIOC->CRL = 0x11118888;
			GPIOC->ODR |= 0x00F0;
			GPIOC->ODR &= ~0x000F;
		}
}



