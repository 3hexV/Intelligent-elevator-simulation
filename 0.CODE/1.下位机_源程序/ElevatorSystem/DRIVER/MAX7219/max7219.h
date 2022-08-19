#ifndef _MAX_7219_H_
#define _MAX_7219_H_

#include "stm32f10x.h"
#include "config.h"

extern byte up[8], down[8], space[8], opening[8], closing[8];
enum _MAX7219_Type {MAX_INIT=5,MAX_INIT_1=6,
										MAX_Space=0, 
										MAX_Up=1, 
										MAX_Down=2, 
										MAX_Closing=3, 
										MAX_Opening=4};

void MAX7219Init(void);
void MAX7219InitWorkMode(void);

void drawMode(u16 type);

void MAX_CS_Set(void);
void MAX_CS_ReSet(void);
void MAX_CLK_Set(void);
void MAX_CLK_ReSet(void);

void Write_Max7219_byte(byte DATA);
void Write_Max7219(byte address,byte dat);

#endif

