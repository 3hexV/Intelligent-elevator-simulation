#ifndef _MAIN_H_
#define _MAIN_H_

// driver lib
#include "smg.h"
#include "dl_key.h"
#include "jz_key.h"
#include "fmq.h"
#include "rgb_led.h"
#include "ele_run.h"
#include "relay.h"
#include "28BYJ_48.h"
#include "s_light.h"
#include "led_group.h"
#include "max7219.h"
#include "1602.h"
#include "matrix_key.h"

// sys
#include "usart.h"

void PeripheralsInit(void);
void SysInit(void);

void delay_ms(u16 nms);
void delay_s(u16 ns);

#endif


