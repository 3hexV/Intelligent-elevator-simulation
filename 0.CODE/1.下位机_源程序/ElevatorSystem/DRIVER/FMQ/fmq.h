#ifndef _FMQ_H_
#define _FMQ_H_

// #define DRIVER_LOW // 低电平触发
#define DRIVER_HIGH 	// 高电平触发

void fmqInit(void);
void fmqEnable(void);
void fmqDisable(void);
void fmqLaunch(void);

#endif

