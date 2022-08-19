/************************************Info************************************
	+程序：电梯算法 - 带有空闲停靠的Scanf算法程序 v1.0
	+开发：/
	+型号：/
	+使用：
				run();//启动
	+接口：
				/
	+备注：
				空闲停靠的楼层可以自定义
*************************************************************************/
#include "ele_run.h"
#include "stm32f10x.h"
#include "stdio.h"
#include "main.h"

#define MOTO_RUN_ANGLE 360

extern int ExSmgNum;

int levelFlag[LEVEL + 1];  //1到Level层的外部输入，取值为0,1,2,3，其中0表示目标楼层,1表示有乘客上行，2表示有乘客下行，3表示既有上行又有下行
int levelDest[LEVEL + 1];  //目的楼层的记录，取值为0,1，其中0表示不是目的地，1表示是目的地
int levelMax, levelMin; //levelMax是进出电梯最大楼层，levelMin是进出电梯最小楼层
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
		relayDisable(); // 关门
	
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

	relayEnable(); // 开门中
	
	drawMode(MAX_Space);
	drawMode(MAX_Opening);
	
	delay_s(2);
	
	delay_s(5);   // 开门
}

void upPull(int nowLevel)//上行接客
{
	DTStop();
	levelFlag[nowLevel]--;
	printf ("上行接客\n");
	DTStart();
}

void downPull(int nowLevel)//下行接客
{
	DTStop();
	levelFlag[nowLevel] -= 2;
	printf ("下行接客\n");
	DTStart();
}

void push(int nowLevel)//送客
{	
	DTStop();
	levelDest[nowLevel] = 0;
	printf ("送客\n");
	DTStart();
}

void input(int level, int flag)
{
	if (level > 0 && level <= LEVEL)
	{
		if (flag>0 && flag <= 3) 
			levelFlag[level] = flag; //外部输入
		else 
			levelDest[level] = 1;//内部输入
	}
}

int getMax(void)//进出电梯最大楼层
{
	int nowLevel, levelMax = 0;
	for (nowLevel = 1; nowLevel <= LEVEL; nowLevel++)
	{
		if (levelFlag[nowLevel] || levelDest[nowLevel])levelMax = max(levelMax, nowLevel);
	}
	return levelMax;
}

int getMin(void)//进出电梯最小楼层
{
	int nowLevel=0, levelMin = LEVEL + 1;
	for (nowLevel = 1; nowLevel <= LEVEL; nowLevel++)
	{
		if (levelFlag[nowLevel] || levelDest[nowLevel])levelMin = min(levelMin, nowLevel);
	}
	return levelMin;
}

int isPark(void)//是否为空闲状态
{
	return getMax() <= 0 || getMax() > LEVEL;
}

int getMaxWithPark(void)//进出电梯最大楼层 空闲时停到 固定楼层
{
	if (isPark())return PARK;
	return getMax();
}
int getMinWithPark(void)//进出电梯最小楼层 空闲时停到 固定楼层
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




