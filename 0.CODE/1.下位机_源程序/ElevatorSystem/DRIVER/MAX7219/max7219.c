/************************************Info************************************
	+程序：MAX7219点阵屏驱动程序 v1.0
	+开发：寄存器
	+型号：/
	+使用：
				MAX7219Init();
				
				drawMode(MAX_Up);
				delay_s(1);
				drawMode(MAX_Space);
				
				drawMode(MAX_Down);
				delay_s(1);
				drawMode(MAX_Space);
	+接口：
				 DIN  <==>  PB0
				 CS   <==>  PB1
				 CLK  <==>  PB2
	+备注：
				图像由取模软件自由绘制
*************************************************************************/
#include "max7219.h"
#include "stm32f10x.h"
#include "main.h"

// 点阵图片定义
byte space[8] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

byte up[8] = {
	0x00,0x04,0x02,0xFF,0xFF,0x02,0x04,0x00,
};

byte down[8] = {
	0x00,0x20,0x40,0xFF,0xFF,0x40,0x20,0x00,
};

byte opening[8] = {
	0x10,0x28,0x44,0x00,0x00,0x44,0x28,0x10,
};

byte closing[8] = {
	0x44,0x28,0x10,0x00,0x00,0x10,0x28,0x44,
};

byte init[8] = {
	0x04,0x22,0x46,0x40,0x40,0x46,0x22,0x04,
};

byte init_1[8] = {
	0x04,0x22,0x16,0x10,0x10,0x16,0x22,0x04,
};

byte MAXTmp=0;

// 绘制对应的点阵图
void drawMode(u16 type)
{
	int i=0;
	for(i=1;i<9;i++){
		if(type==MAX_Space)
			Write_Max7219(i,space[i-1]);
		else if(type==MAX_INIT)
			Write_Max7219(i,init[i-1]);
		else if(type==MAX_INIT_1)
			Write_Max7219(i,init_1[i-1]);
		else if(type==MAX_Up)
			Write_Max7219(i,up[i-1]);
		else if(type==MAX_Down)
			Write_Max7219(i,down[i-1]);
		else if(type==MAX_Opening)
			Write_Max7219(i,opening[i-1]);
		else if(type==MAX_Closing)
			Write_Max7219(i,closing[i-1]);
	}
}

// MAX7219初始化
void MAX7219Init(void)
{
	static int i=10;
	RCC->APB2ENR |= (1ul<<3);
	
	// 端口初始
	GPIOB->CRL &= ~0x00000FFF;
	GPIOB->CRL |= 0x00000111;
	
	MAX7219InitWorkMode();
	
	while(i--)
	{
		drawMode(MAX_INIT);
		delay_ms(100);
		drawMode(MAX_Space);
		drawMode(MAX_INIT_1);
		delay_ms(100);
	}
	drawMode(MAX_INIT);
}

void MAX_CS_Set(void)
{
	GPIOB->ODR |= (1ul<<1);
}

void MAX_CS_ReSet(void)
{
	GPIOB->ODR &= ~(1ul<<1);
}

void MAX_CLK_Set(void)
{
	GPIOB->ODR |= (1ul<<2);
}

void MAX_CLK_ReSet(void)
{
	GPIOB->ODR &= ~(1ul<<2);
}

//--------------------------------------------
//功能：向MAX7219(U3)写入字节
//入口参数：DATA 
//出口参数：无
//说明：
void Write_Max7219_byte(byte DATA)         
{
    byte i;    
		MAX_CS_ReSet();	
		for(i=8;i>=1;i--)
		{		  
			MAX_CLK_ReSet();
			
			GPIOB->ODR =(DATA&(1<<7))>>7;
			MAXTmp = GPIOA->ODR&0x01;
			
			DATA=DATA<<1;
			MAX_CLK_Set();
		}                                 
}
//-------------------------------------------
//功能：向MAX7219写入数据
//入口参数：address、dat
//出口参数：无
//说明：
void Write_Max7219(byte address,byte dat)
{ 
  MAX_CS_ReSet();	
	Write_Max7219_byte(address);           //写入地址，即数码管编号
  Write_Max7219_byte(dat);               //写入数据，即数码管显示数字 
	MAX_CS_Set();	                       
}

void MAX7219InitWorkMode(void)
{
	 Write_Max7219(0x09, 0x00);       //译码方式：BCD码
	 Write_Max7219(0x0a, 0x03);       //亮度 
	 Write_Max7219(0x0b, 0x07);       //扫描界限；8个数码管显示
	 Write_Max7219(0x0c, 0x01);       //掉电模式：0，普通模式：1
	 Write_Max7219(0x0f, 0x00);       //显示测试：1；测试结束，正常显示：0
}
 

