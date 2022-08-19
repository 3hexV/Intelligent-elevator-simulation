#ifndef _ELE_RUN_H_
#define _ELE_RUN_H_


#define LEVEL 5 // ×î¸ßÂ¥²ã
#define PARK 1  // ¿ÕÏÐÍ£¿¿Â¥²ã

void input(int level, int flag);
void upPull(int loc);
void downPull(int loc);
void push(int loc);
int  getMax(void);
int  getMin(void);
int  getMaxWithPark(void);
int  getMinWithPark(void);
int  isPark(void);
int  max(int src0, int src1);
int  min(int src0, int src1);
void run(void);

void DTStop(void);
void DTStart(void);

#endif


