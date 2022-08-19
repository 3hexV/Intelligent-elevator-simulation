/************************************Info************************************
	+���򣺲���������� v1.0
	+�������̼���
	+�ͺţ�28BYJ-48
	+ʹ�ã�
				BYJ_48_Init(39,7199);//���������ʼ�����õ�TIM3��
				BYJ_Start(1, 180); //��ת180��
				BYJ_Start(2, 360); //��ת360��
	+�ӿڣ�
				VCC <==> 5v
				GND <==> GND
				IN1 <==> PA4
				IN2 <==> PA5
				IN3 <==> PA6
				IN4 <==> PA7		
	+��ע��
				TIM3     ��Ϊ������
*************************************************************************/
#include "28BYJ_48.h"
#include "main.h"

int tmpBYJ=0;
int BYJCount=0, BYJMode=1;

//�����ж����ȼ�
void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructer;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	NVIC_InitStructer.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�
	NVIC_InitStructer.NVIC_IRQChannelSubPriority=1;//��Ӧ���ȼ�
	NVIC_InitStructer.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitStructer.NVIC_IRQChannelCmd=ENABLE;

	NVIC_Init(&NVIC_InitStructer);
}
//��ʼ��GPIO���źͶ�ʱ��
void BYJ_48_Init(u16 ARR,u16 PSC)
{
	GPIO_InitTypeDef GPIO_InitStructer;  //�����ṹ�����
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructer;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ�ܶ˿�ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//ʹ�ܶ�ʱ��3ʱ��
	
	GPIO_InitStructer.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;//�򿪶˿�
	GPIO_InitStructer.GPIO_Mode=GPIO_Mode_Out_PP;//���ö˿�ģʽ���������ģʽ��
	GPIO_InitStructer.GPIO_Speed=GPIO_Speed_50MHz;//���ö˿�ʱ��
	GPIO_Init(GPIOA,&GPIO_InitStructer);//ѡ�����ö˿��飬��ʼ���⺯�������������д��
	
	TIM_DeInit(TIM3);
	TIM_TimeBaseInitStructer.TIM_Period=ARR;//��ʱ����ΪARR
	TIM_TimeBaseInitStructer.TIM_Prescaler=PSC; //��Ƶϵ��PSC
	TIM_TimeBaseInitStructer.TIM_ClockDivision=TIM_CKD_DIV1;//����Ƶ
	TIM_TimeBaseInitStructer.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructer);

	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//���������ж�
	NVIC_Config();
	
	TIM_Cmd(TIM3,DISABLE);


	// ������� �Լ�
	BYJ_Start(1, 60);
	delay_ms(10);
	BYJ_Start(2, 60);
	
	BYJ_A=1,BYJ_B=0;BYJ_C=0;BYJ_D=0;
}

// 270 == 180��
void BYJ_Start(u16 mode, u16 angle)
{
	BYJCount = 0;
	BYJMode = mode;
	
	angle = angle*((double)265/180);
	
	TIM_Cmd(TIM3,ENABLE);
	while(BYJCount<angle);
	BYJ_Stop();
}

// ���ͣת
void BYJ_Stop(void)
{
	BYJCount = 0;
	BYJ_A=1,BYJ_B=0;BYJ_C=0;BYJ_D=0;
	TIM_Cmd(TIM3,DISABLE);
}

// ���ת��4����λ
void BYJ_StepRunCW(void)
{
	static int icw;
	switch(icw)
	{
		case 1:BYJ_D=0;BYJ_B=1;icw=2;break;//AB��
		
		case 2:BYJ_A=0;BYJ_C=1;icw=3;break;//BC��
		
		case 3:BYJ_B=0;BYJ_D=1;icw=4;break;//CD��
		
		case 4:BYJ_C=0;BYJ_A=1;icw=1; BYJCount++; break;//DA��
		default :icw=1;  break;
	}
}

// �����ת4����λ
void BYJ_StepRunCCW(void)
{
	static int iccw;
	switch(iccw)
	{
		case 1:BYJ_B=0;BYJ_D=1;iccw=2;break;//AD��  
		
		case 2:BYJ_A=0;BYJ_C=1;iccw=3;break;//DC��
		
		case 3:BYJ_D=0;BYJ_B=1;iccw=4;break;//CB��
		
		case 4:BYJ_C=0;BYJ_A=1;iccw=1; BYJCount++; break;//BA��
		default :iccw=1;  break;
	}
}


// TIM3�жϷ�����
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)
	{	
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);//����жϱ�־
		if(BYJMode==1) BYJ_StepRunCW();
		else if(BYJMode==2) BYJ_StepRunCCW();
	}
}
