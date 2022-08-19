#ifndef _RGB_LED_H_
#define _RGB_LED_H_

#define T_RUN_STOP_READ 1

enum _DTModePara {DT_ReadyStop=1, DT_Stoped = 2, DT_ReadyRun = 3, DT_Runing = 4};

void rgbLedInit(void);
void rgbLedShow(int R, int G, int B);
void DTRunMode(int mode);

#endif

