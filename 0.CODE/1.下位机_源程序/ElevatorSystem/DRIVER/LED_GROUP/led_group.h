#ifndef _LED_GROUP_H_
#define _LED_GROUP_H_
#include "stm32f10x.h"
enum _LedGroupLightMode {LG_OFF = 0, LG_ON_1 = 1, LG_ON_2 = 2,LG_FLASH_0=3,LG_FLASH_1=4};

void ledGroupInit(void);
void ledGroupLightMode(u16 mode);

#endif

