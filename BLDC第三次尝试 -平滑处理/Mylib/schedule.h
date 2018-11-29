#ifndef _SCHEDULE_H
#define _SCHEDULE_H

#include "stm32f10x.h"


typedef struct _schedule{
													 uint16_t   cnt_1ms;
													 uint16_t   cnt_2ms;
													 uint16_t   cnt_5ms;
													 uint16_t   cnt_10ms;
													 uint16_t   cnt_20ms;
													 uint16_t   cnt_50ms;
													 uint16_t   cnt_100ms;
													 uint64_t   sysTime_ms;
													 uint64_t   beatStart_ms;
												}_schedule;

void Loop_1000Hz(void); //1ms执行一次
void Loop_500Hz(void);	//2ms执行一次
void Loop_200Hz(void);	//5ms执行一次
void Loop_100Hz(void);	//10ms执行一次
void Loop_50Hz(void);	  //20ms执行一次
void Loop_20Hz(void);	  //50ms执行一次
void Loop_10Hz(void);	  //100ms执行一次

void Loop(void);

extern struct _schedule schedule;
extern float BLDC_SPEED_FILT;


#endif
