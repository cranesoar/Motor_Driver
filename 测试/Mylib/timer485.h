#ifndef _TIMER485_H
#define _TIMER485_H
#include "board.h"

#define MAX_PWM  8000
#define MIN_PWM  4000


void TIM2_IRQHandler(void);
void Gimbal_TIM2_Init(void);
extern int DM50_SPEED_Yaw,DM50_SPEED_Pitch1,DM50_SPEED_Pitch2;


#endif
