#ifndef _MLT8530_H_
#define _MLT8530_H_
#include "board.h"

#define  MLT_TOGGLE  GPIOA->ODR ^= GPIO_Pin_0
#define  MLT_OFF     GPIOA->BSRRL = GPIO_Pin_0
#define  MLT_ON      GPIOA->BSRRH = GPIO_Pin_0

void TDT_TIM5_PWM_Bell_Init(void);
void reconfig(int pre);
void start_task(int len);



#endif
