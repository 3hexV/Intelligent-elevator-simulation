/************************************Info************************************
	+程序：步进电机驱动 v1.0
	+开发：固件库
	+型号：28BYJ-48
	+使用：
				BYJ_48_Init(39,7199);//步进电机初始化（用到TIM3）
				BYJ_Start(1, 180); //正转180°
				BYJ_Start(2, 360); //反转360°
	+接口：
				VCC <==> 5v
				GND <==> GND
				IN1 <==> PA4
				IN2 <==> PA5
				IN3 <==> PA6
				IN4 <==> PA7		
	+备注：
				TIM3     作为节拍器
*************************************************************************/
#include "28BYJ_48.h"
#include "main.h"

int tmpBYJ=0;
int BYJCount=0, BYJMode=1;

//设置中断优先级
void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructer;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	NVIC_InitStructer.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级
	NVIC_InitStructer.NVIC_IRQChannelSubPriority=1;//响应优先级
	NVIC_InitStructer.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitStructer.NVIC_IRQChannelCmd=ENABLE;

	NVIC_Init(&NVIC_InitStructer);
}
//初始化GPIO引脚和定时器
void BYJ_48_Init(u16 ARR,u16 PSC)
{
	GPIO_InitTypeDef GPIO_InitStructer;  //申明结构体变量
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructer;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能端口时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//使能定时器3时钟
	
	GPIO_InitStructer.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;//打开端口
	GPIO_InitStructer.GPIO_Mode=GPIO_Mode_Out_PP;//设置端口模式（推挽输出模式）
	GPIO_InitStructer.GPIO_Speed=GPIO_Speed_50MHz;//设置端口时钟
	GPIO_Init(GPIOA,&GPIO_InitStructer);//选择设置端口组，初始化库函数将上面的内容写入
	
	TIM_DeInit(TIM3);
	TIM_TimeBaseInitStructer.TIM_Period=ARR;//定时周期为ARR
	TIM_TimeBaseInitStructer.TIM_Prescaler=PSC; //分频系数PSC
	TIM_TimeBaseInitStructer.TIM_ClockDivision=TIM_CKD_DIV1;//不分频
	TIM_TimeBaseInitStructer.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructer);

	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//开启更新中断
	NVIC_Config();
	
	TIM_Cmd(TIM3,DISABLE);


	// 步进电机 自检
	BYJ_Start(1, 60);
	delay_ms(10);
	BYJ_Start(2, 60);
	
	BYJ_A=1,BYJ_B=0;BYJ_C=0;BYJ_D=0;
}

// 270 == 180°
void BYJ_Start(u16 mode, u16 angle)
{
	BYJCount = 0;
	BYJMode = mode;
	
	angle = angle*((double)265/180);
	
	TIM_Cmd(TIM3,ENABLE);
	while(BYJCount<angle);
	BYJ_Stop();
}

// 电机停转
void BYJ_Stop(void)
{
	BYJCount = 0;
	BYJ_A=1,BYJ_B=0;BYJ_C=0;BYJ_D=0;
	TIM_Cmd(TIM3,DISABLE);
}

// 电机转动4个相位
void BYJ_StepRunCW(void)
{
	static int icw;
	switch(icw)
	{
		case 1:BYJ_D=0;BYJ_B=1;icw=2;break;//AB相
		
		case 2:BYJ_A=0;BYJ_C=1;icw=3;break;//BC相
		
		case 3:BYJ_B=0;BYJ_D=1;icw=4;break;//CD相
		
		case 4:BYJ_C=0;BYJ_A=1;icw=1; BYJCount++; break;//DA相
		default :icw=1;  break;
	}
}

// 电机逆转4个相位
void BYJ_StepRunCCW(void)
{
	static int iccw;
	switch(iccw)
	{
		case 1:BYJ_B=0;BYJ_D=1;iccw=2;break;//AD相  
		
		case 2:BYJ_A=0;BYJ_C=1;iccw=3;break;//DC相
		
		case 3:BYJ_D=0;BYJ_B=1;iccw=4;break;//CB相
		
		case 4:BYJ_C=0;BYJ_A=1;iccw=1; BYJCount++; break;//BA相
		default :iccw=1;  break;
	}
}


// TIM3中断服务函数
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)
	{	
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);//清除中断标志
		if(BYJMode==1) BYJ_StepRunCW();
		else if(BYJMode==2) BYJ_StepRunCCW();
	}
}
