/************************************Info************************************
	+���򣺵����㷨 - ���п���ͣ����Scanf�㷨���� v1.0
	+������/
	+�ͺţ�/
	+ʹ�ã�
				run();//����
	+�ӿڣ�
				/
	+��ע��
				����ͣ����¥������Զ���
*************************************************************************/
#include "ele_run.h"
#include "stm32f10x.h"
#include "stdio.h"
#include "main.h"

#define MOTO_RUN_ANGLE 360

extern int ExSmgNum;

int levelFlag[LEVEL + 1];  //1��Level����ⲿ���룬ȡֵΪ0,1,2,3������0��ʾĿ��¥��,1��ʾ�г˿����У�2��ʾ�г˿����У�3��ʾ����������������
int levelDest[LEVEL + 1];  //Ŀ��¥��ļ�¼��ȡֵΪ0,1������0��ʾ����Ŀ�ĵأ�1��ʾ��Ŀ�ĵ�
int levelMax, levelMin; //levelMax�ǽ����������¥�㣬levelMin�ǽ���������С¥��
int nowLevel = 1;
 
void run(void)
{
	printf ("nowLevel:%d\n", nowLevel);
	if(nowLevel==1)
	{
		drawMode(MAX_Space);
		drawMode(MAX_Closing);
	}
	while (nowLevel <= getMaxWithPark())
	{
		ExSmgNum = nowLevel;
		if (levelFlag[nowLevel] == 1 || levelFlag[nowLevel] == 3) 
		{
			fmqLaunch();
			upPull(nowLevel);
		}
		if (levelDest[nowLevel]) push(nowLevel);
		// delay
		
		if (nowLevel<getMaxWithPark()){
			drawMode(MAX_Space);
			drawMode(MAX_Up);
			
			BYJ_Start(1, MOTO_RUN_ANGLE);
			
			nowLevel++;
			ExSmgNum = nowLevel;
			printf("nowLevel:%d\n", nowLevel);
			smgShowNum(nowLevel);
		}
		else break;
	}
	while (nowLevel >= getMinWithPark())
	{
		ExSmgNum = nowLevel;
		if (levelFlag[nowLevel] == 2 || levelFlag[nowLevel] == 3)downPull(nowLevel);
		if (levelDest[nowLevel]) push(nowLevel);
		// delay
		
		if (nowLevel > getMinWithPark()){
			drawMode(MAX_Space);
			drawMode(MAX_Down);
			
			BYJ_Start(2, MOTO_RUN_ANGLE);
				
			nowLevel--;
			ExSmgNum = nowLevel;
			printf ("nowLevel:%d\n", nowLevel);
			smgShowNum(nowLevel);
		}
		else break;
	}
	printf("nowLevel:%d\n", nowLevel);
}

void DTStart(void)
{	
	static int firstFlag=0;
	
	if(firstFlag==0)
	{
		firstFlag = 1;
	}else{
		relayDisable(); // ����
	
		drawMode(MAX_Space);
		drawMode(MAX_Closing);
	}
	delay_s(2);	
	fmqLaunch();
	DTRunMode(DT_ReadyRun);

	DTRunMode(DT_Runing);
}

void DTStop(void)
{
	fmqLaunch();
	DTRunMode(DT_ReadyStop);
	delay_ms(500);
	
	DTRunMode(DT_Stoped);

	relayEnable(); // ������
	
	drawMode(MAX_Space);
	drawMode(MAX_Opening);
	
	delay_s(2);
	
	delay_s(5);   // ����
}

void upPull(int nowLevel)//���нӿ�
{
	DTStop();
	levelFlag[nowLevel]--;
	printf ("���нӿ�\n");
	DTStart();
}

void downPull(int nowLevel)//���нӿ�
{
	DTStop();
	levelFlag[nowLevel] -= 2;
	printf ("���нӿ�\n");
	DTStart();
}

void push(int nowLevel)//�Ϳ�
{	
	DTStop();
	levelDest[nowLevel] = 0;
	printf ("�Ϳ�\n");
	DTStart();
}

void input(int level, int flag)
{
	if (level > 0 && level <= LEVEL)
	{
		if (flag>0 && flag <= 3) 
			levelFlag[level] = flag; //�ⲿ����
		else 
			levelDest[level] = 1;//�ڲ�����
	}
}

int getMax(void)//�����������¥��
{
	int nowLevel, levelMax = 0;
	for (nowLevel = 1; nowLevel <= LEVEL; nowLevel++)
	{
		if (levelFlag[nowLevel] || levelDest[nowLevel])levelMax = max(levelMax, nowLevel);
	}
	return levelMax;
}

int getMin(void)//����������С¥��
{
	int nowLevel=0, levelMin = LEVEL + 1;
	for (nowLevel = 1; nowLevel <= LEVEL; nowLevel++)
	{
		if (levelFlag[nowLevel] || levelDest[nowLevel])levelMin = min(levelMin, nowLevel);
	}
	return levelMin;
}

int isPark(void)//�Ƿ�Ϊ����״̬
{
	return getMax() <= 0 || getMax() > LEVEL;
}

int getMaxWithPark(void)//�����������¥�� ����ʱͣ�� �̶�¥��
{
	if (isPark())return PARK;
	return getMax();
}
int getMinWithPark(void)//����������С¥�� ����ʱͣ�� �̶�¥��
{
	if (isPark())return PARK;
	return getMin();
}

int max(int src0, int src1)
{
	return (src0>src1?(src0):(src1));
}

int min(int src0, int src1)
{
	return (src0<src1?(src0):(src1));
}




