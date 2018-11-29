#ifndef __PWM_H__
#define __PWM_H__
#include "main.h"

//#define BLDC_W TIM1->CCR2;
//#define BLDC_U TIM2->CCR1;
//#define BLDC_V TIM2->CCR2;

/***************INH GPIO∂®“Â******************/
#define  INH_RCC           RCC_APB2Periph_GPIOB
#define  INH_PORT          GPIOB
#define  INH_U             GPIO_Pin_8
#define  INH_V             GPIO_Pin_9 
#define  INH_W             GPIO_Pin_10

#define  INH_U_ON      GPIO_SetBits(INH_PORT,INH_U)
#define  INH_U_OFF     GPIO_ResetBits(INH_PORT,INH_U)

#define  INH_V_ON      GPIO_SetBits(INH_PORT,INH_V)
#define  INH_V_OFF     GPIO_ResetBits(INH_PORT,INH_V)

#define  INH_W_ON      GPIO_SetBits(GPIOA,INH_W)
#define  INH_W_OFF     GPIO_ResetBits(GPIOA,INH_W)

void PWM_Configuration(void);
void PWM_OutSet(int value);
void INH_Init(void);
void BLDC_U(int16_t value);
void BLDC_V(int16_t value);
void BLDC_W(int16_t value);
int Commutation(int pos);

#endif
