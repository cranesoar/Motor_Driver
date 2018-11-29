#ifndef __MAIN_H__
#define __MAIN_H__

#include "stm32f10x.h"
#include <stdio.h>
#include <stdlib.h>

#include "pwm.h"
#include "encoder.h"
#include "delay.h"
#include "usart2.h"
#include "can.h"
#include "flash.h"
#include "key.h"
#include "SysTick.h"
#include "schedule.h"
#include "usartBroadcast.h"
#include "control.h"
#include "parameter.h"
#include "capture.h"
#include "math.h"
#include "filter.h"



#define abs( x ) ( (x)>0? (x):(-(x)) )
#define limit( x, min, max ) ( (x) < (min)  ? (min) : ( (x) > (max) ? (max) : (x) ) )
#define deathzoom( x, zoom ) ( abs(x)>zoom? (x):(0) )
#define contain( x, min, max ) ( (x<min)?(x+2*max):( (x>max)?(x-2*max):(x) ) )
#define ABS(x) ( (x)>0?(x):-(x) )
#define LIMIT( x,min,max ) ( (x) < (min)  ? (min) : ( (x) > (max) ? (max) : (x) ) )
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define SPEED_FILT_NUM 5


#define MAX_PWM_BLDC   5000
#define CHANGE_POINT     1.7                     //   若为2则线圈磁场超前定子90度，此时力矩最大。但可适当提前，增大该参数换相提前
extern int BLDC_step;
extern int BLDC_POSTION;
extern long int sys_time_ms;
extern _Bool BLDC_DIR_FLG;   
extern int BLDC_pwm;







#endif
