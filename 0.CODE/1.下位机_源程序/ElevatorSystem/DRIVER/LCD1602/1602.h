#ifndef __1602_H 
#define __1602_H 

#include "sys.h" 

#define SHOW_TIME 500

void init1602(void); 
void write_com(char com); 
void write_dat(char date); 
void showInformation(void);
void sysRunShow(void);
void showInitProcess(int nowP);

#endif

