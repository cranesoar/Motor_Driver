#ifndef _TIM1_H_
#define _TIM1_H_

#include "main.h"

#define  PWM_PERIOD_CYCLES  4500
#define  TIM_CLOCK_DIVIDER  1

void TIM1_Init(u16  arr,  u16 psc);

#endif
