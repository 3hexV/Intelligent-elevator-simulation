/************************************Info************************************
	+程序：LED1600液晶显示屏驱动程序 v1.0
	+开发：寄存器
	+型号：/
	+使用：
				init1602();
	+接口：
				 RS  		<==>  PC8
				 RW  		<==>  PC7
				 EN  		<==>  PC6
				 DB0-7  <==>  PF0-7
	+备注：
				
*************************************************************************/
#include <stm32f10x.h>
#include "1602.h" 
#include "main.h" 
#include "string.h"

#define rd PCout(8) 
#define wr PCout(7)
#define en PCout(6)
  
char table[]={
0x02,0x04,0x0F,0x12,0x0F,0x0A,0x1F,0x02,//年
0x0F,0x09,0x0F,0x09,0x0F,0x09,0x09,0x11,//月
0x1F,0x11,0x11,0x1F,0x11,0x11,0x1F,0x00,//日
};



char T0[][16]={	"|+-------|", //10%
								"|_+------|", //20%
								"|__+-----|", //30%
								"|___+----|", //40%
								"|____+---|", //60%
								"|_____+--|", //80%
								"|______+-|", //90%
								"|_______+|"};  //100%
char T0_0[][16]={	"10%",
								"20%", 
								"30%", 
								"40%", 
								"60%", 
								"80%", 
								"90%", 
								"100%"}; 

char T1[][16]={"1.Integrated",
						   "design of SCM",
						   "and RFID course"};

char T2[][16]={"2.Intelligent",
						   "elevator control",
						   "system"};
char T3[][16]={"3.chen hu(06)",
						   "hu jian(30)",
						   "wu jin ping(87)",
							 "yan zi heng(98)"};
char T4[][16]={"4.2020 Internet",
								"of things",
						    "Engineering(ZSB)",};
char T5[][16]={"5.School of",
								"computer and",
						    "information"};
char T6[][16]={"6.Anhui Normal",
								"University(AHNU)"};  
char T7[16]={" >Sys init Suc<"}; 
char T8[16]={"           :)"}; 
char T9[][16]={"Runing","Runing.","Runing..","Runing...","Runing ...","Runing ..."}; 

char T10[][16]={"RFID: "," JZ: "}; 

u8 table1[]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07};  //自定义字符数据地址

extern int RFID_Data_Count, JZ_Key_Data_Count;
	

// LCD1602初始化
void init1602() 
{ 
	AFIO->MAPR&=0XF8FFFFFF;   			//清除MAPR的［26：24］
  AFIO->MAPR|=0X04000000;

	RCC->APB2ENR|=1<<7;     			//使能PORTF时钟 
	RCC->APB2ENR|=1<<4;     			//使能PORTC时钟

	//pc6-pc9推挽输出
	GPIOC->CRH&=0XFFFFFF00;
	GPIOC->CRL&=0X00FFFFFF;

	GPIOC->CRH|=0X00000033;
	GPIOC->CRL|=0X33000000;
	GPIOC->ODR|=0X03C0; 
	//Pf推挽输出
	GPIOF->CRL&=0X00000000; 			 
	GPIOF->CRL|=0X33333333; 
	GPIOF->ODR|=0XFF; 
	 
	delay_ms(5);
	write_com(0x38);
  write_com(0x38);
  write_com(0x08);
	write_com(0x01);
	write_com(0x06);
	write_com(0x0c);

}
 
/*******************写指令函数************************/
void write_com(char com)	 			  
{ 
	rd=0; 
	wr=0; 
	en=0;
	 
	delay_ms(2); 
	GPIOF->ODR=0X00FF&com; 					  
	delay_ms(5); 
	en=1; 
	delay_ms(5); 
	en=0; 
	delay_ms(5); 
} 
/*******************写数据函数************************/

void write_dat(char date)	     	  
{ 
	rd=1;
	wr=0; 
	en=0;
	  
	delay_ms(2); 
	GPIOF->ODR=0X00FF&date; 					 
	delay_ms(5); 
	en=1;  
	delay_ms(5); 
	en=0; 
}
// 显示运行状态
void sysRunShow(void)
{
	static int lcd1602_index=0;
	static int lcd1602_indexMode=1;
	int i=0, len=0;
	
	write_com(0x01);
		
	// run state
	write_com(0x80);    
	len = strlen(T9[lcd1602_index]);
	for(i=0;i<len;i++)
	{
		write_dat(T9[lcd1602_index][i]);
	}
	
	if(lcd1602_indexMode==1)
	{
		lcd1602_index++;
		if(lcd1602_index==6)
		{
			lcd1602_index = 5;
			lcd1602_indexMode = 0;
		}
	}else if(lcd1602_indexMode==0)
	{
		lcd1602_index--;
		if(lcd1602_index==-1)
		{
			lcd1602_index = 0;
			lcd1602_indexMode = 1;
		}
	}
		
	write_com(0x80+0x40);
	// rfid and jz reC
	len = strlen(T10[0]);
	for(i=0;i<len;i++)
	{
		write_dat(T10[0][i]);
	}
	write_dat(RFID_Data_Count+'0');
	
	len = strlen(T10[1]);
	for(i=0;i<len;i++)
	{
		write_dat(T10[1][i]);
	}
	write_dat(JZ_Key_Data_Count+'0');
	
	delay_s(1);
	
}
// 显示自检进度
void showInitProcess(int nowP)
{
	int i=0, len=0;
	
	write_com(0x01);
	write_com(0x80+0x03);
	
	len = strlen(T0[nowP]);
	for(i=0;i<len;i++)
	{
		write_dat(T0[nowP][i]);
	}

	write_com(0x80+0x40+0x07);
	len = strlen(T0_0[nowP]);
	for(i=0;i<len;i++)
	{
		write_dat(T0_0[nowP][i]);
	}
	
	delay_ms(100);
}

// 按要求显示信息
void showInformation(void)
{
	int i=0 , len=0;

	// 1.课程
	write_com(0x80);    //从第一行第一列开始显示
	len = strlen(T1[0]);
	for(i=0;i<len;i++)
	{
		write_dat(T1[0][i]);
	}
	delay_ms(SHOW_TIME);
	
	write_com(0x80+0x40);
	len = strlen(T1[1]);
	for(i=0;i<len;i++)
	{
		write_dat(T1[1][i]);
	}
	delay_ms(SHOW_TIME);
	
	write_com(0x01);
	
	write_com(0x80);    
	len = strlen(T1[2]);
	for(i=0;i<len;i++)
	{
		write_dat(T1[2][i]);
	}
	delay_ms(SHOW_TIME);
	
	write_com(0x01);
	
	//2.作品名称
	write_com(0x80);    //从第一行第一列开始显示
	len = strlen(T2[0]);
	for(i=0;i<len;i++)
	{
		write_dat(T2[0][i]);
	}
	delay_ms(SHOW_TIME);
	
	write_com(0x80+0x40);
	len = strlen(T2[1]);
	for(i=0;i<len;i++)
	{
		write_dat(T2[1][i]);
	}
	delay_ms(SHOW_TIME);
	
	write_com(0x01);
	
	write_com(0x80);    
	len = strlen(T2[2]);
	for(i=0;i<len;i++)
	{
		write_dat(T2[2][i]);
	}
	delay_ms(SHOW_TIME);
	
	write_com(0x01);
	
	//3.学生
	write_com(0x80);    //从第一行第一列开始显示
	len = strlen(T3[0]);
	for(i=0;i<len;i++)
	{
		write_dat(T3[0][i]);
	}
	delay_ms(SHOW_TIME);
	
	write_com(0x80+0x40);
	len = strlen(T3[1]);
	for(i=0;i<len;i++)
	{
		write_dat(T3[1][i]);
	}
	delay_ms(SHOW_TIME);
	
	write_com(0x01);
	
	write_com(0x80);    
	len = strlen(T3[2]);
	for(i=0;i<len;i++)
	{
		write_dat(T3[2][i]);
	}
	delay_ms(SHOW_TIME);
	
	write_com(0x80+0x40);
	len = strlen(T3[3]);
	for(i=0;i<len;i++)
	{
		write_dat(T3[3][i]);
	}
	delay_ms(SHOW_TIME);
	
	write_com(0x01);
	
	//4. 专业
	write_com(0x80);    //从第一行第一列开始显示
	len = strlen(T4[0]);
	for(i=0;i<len;i++)
	{
		write_dat(T4[0][i]);
	}
	delay_ms(SHOW_TIME);
	
	write_com(0x80+0x40);
	len = strlen(T4[1]);
	for(i=0;i<len;i++)
	{
		write_dat(T4[1][i]);
	}
	delay_ms(SHOW_TIME);
	
	write_com(0x01);
	
	write_com(0x80);    //从第一行第一列开始显示
	len = strlen(T4[2]);
	for(i=0;i<len;i++)
	{
		write_dat(T4[2][i]);
	}
	delay_ms(SHOW_TIME);
	
	write_com(0x01);
	
	// 5. 学院
	write_com(0x80);    //从第一行第一列开始显示
	len = strlen(T5[0]);
	for(i=0;i<len;i++)
	{
		write_dat(T5[0][i]);
	}
	delay_ms(SHOW_TIME);
	
	write_com(0x80+0x40);
	len = strlen(T5[1]);
	for(i=0;i<len;i++)
	{
		write_dat(T5[1][i]);
	}
	delay_ms(SHOW_TIME);
	
	write_com(0x01);
	
	write_com(0x80);    //从第一行第一列开始显示
	len = strlen(T5[2]);
	for(i=0;i<len;i++)
	{
		write_dat(T5[2][i]);
	}
	delay_ms(SHOW_TIME);
	
	write_com(0x01);
	
	// 6. 学校
	write_com(0x80);    //从第一行第一列开始显示
	len = strlen(T6[0]);
	for(i=0;i<len;i++)
	{
		write_dat(T6[0][i]);
	}
	delay_ms(SHOW_TIME);
	
	write_com(0x80+0x40);
	len = strlen(T6[1]);
	for(i=0;i<len;i++)
	{
		write_dat(T6[1][i]);
	}
	delay_ms(SHOW_TIME);
	
	write_com(0x01);
	
	
	// 7. 时间
	write_com(0x40);    //开始写入你要显示的自定义字符、汉字代码
	for(i=0;i<24;i++)
	{
		write_dat(table[i]);
	}
	delay_ms(10);
	write_com(0x80);
	write_dat('7');write_dat('.');
	write_dat('2');write_dat('0'); write_dat('2');write_dat('0'); delay_ms(5);
	write_dat(table1[0]);  delay_ms(5);
	
	write_dat('1');write_dat('2'); delay_ms(5);
	write_dat(table1[1]); delay_ms(5); 
	
	write_dat('1');write_dat('2'); delay_ms(5); 
	write_dat(table1[2]);delay_ms(5);  
	
	delay_ms(SHOW_TIME);
	
	write_com(0x01);
	
	// 8. 
	write_com(0x80);    //从第一行第一列开始显示
	len = strlen(T7);
	for(i=0;i<len;i++)
	{
		write_dat(T7[i]);
	}
	delay_ms(SHOW_TIME);
	
	write_com(0x80+0x40+0x07);    //从第一行第一列开始显示
	len = strlen(T8);
	for(i=0;i<len;i++)
	{
		write_dat(T8[i]);
	}
	delay_ms(100);
}

