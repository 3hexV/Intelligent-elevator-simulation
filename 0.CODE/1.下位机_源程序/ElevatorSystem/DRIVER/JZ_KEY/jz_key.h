#ifndef _JZ_KEY_H
#define _JZ_KEY_H

#include "config.h"

#define KEY_SOFT_DELAY 1

void jzKeyInit(int ARR,int PSC);
int jzKeyScanf0_3(void);
int jzKeyScanf4_7(void);
void jzKeyMode(int mode);
byte jzKeyScanf(void);

#endif

