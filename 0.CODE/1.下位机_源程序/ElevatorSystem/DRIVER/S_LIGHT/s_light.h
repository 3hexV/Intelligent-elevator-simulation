#ifndef _S_LIGHT_H_
#define _S_LIGHT_H_

#include "config.h"

void SLightInit(int ARR, int PSC);
byte readSLightData(void);
void controlLight(void);

void scanfFire(void);
byte readSFireData(void);

#endif

