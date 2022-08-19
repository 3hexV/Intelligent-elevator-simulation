#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "ele_run.h"
#include "stm32f10x.h"

typedef unsigned char byte;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;

// smg.h
extern int _smg_count, smgExChangeCount, numList[16];

// ele_run.h
extern int levelFlag[LEVEL + 1];
extern int levelDest[LEVEL + 1];
extern int levelMax, levelMin; 
extern int nowLevel ;

#endif

