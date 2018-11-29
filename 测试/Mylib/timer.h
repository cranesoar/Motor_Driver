#ifndef _TIMER_H
#define _TIMER_H
#include "board.h"

#define MAX_PWM  8000
#define MIN_PWM  4000

void TDT_TIM4_PWM_Init(void);
void TDT_TIM3_PWM_Init(void);
void TIM7_IRQHandler(void);
void TIM7_Key_Configuration(void);


#endif
