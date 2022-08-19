#ifndef _JZ_KEY_H
#define _JZ_KEY_H

#include "stm32f10x.h"

#define KEY_SOFT_DELAY 60000
typedef unsigned char byte;

void jzKeyInit(void);
int jzKeyScanf0_3(void);
int jzKeyScanf4_7(void);
void jzKeyMode(int mode);
byte jzKeyScanf(void);

void Tim3_init(u16 arr, u16 psc);

#endif

