#ifndef _28BYJ_48_H
#define _28BYJ_48_H
#include "sys.h"

#define BYJ_A PAout(4)
#define BYJ_B PAout(5)
#define BYJ_C PAout(6)
#define BYJ_D PAout(7)

void BYJ_48_Init(u16 ARR,u16 PSC);
void BYJ_StepRunCW(void);
void BYJ_StepRunCCW(void);

void BYJ_Stop(void);
void BYJ_Start(u16 mode, u16 angle);
	
#endif
