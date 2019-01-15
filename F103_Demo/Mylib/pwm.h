#ifndef __PWM_H__
#define __PWM_H__
#include "stm32f10x.h"

void TIM3_pwm_init(void);
void TIM2_pwm_init(void);
void TIM4_pwm_init(void);
void PWM_OUT(int A0 ,int A1 ,int A2 , int A3 , int A6 , int A7 , int B0 , int B1 , int B6 , int B7 , int B8 , int B9 );
#endif
