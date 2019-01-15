#ifndef __LED_H__
#define __LED_H__
#include "main.h"
#include "stm32f10x.h"
/***************LED GPIO¶¨Òå******************/
#define  RCC_LED           RCC_APB2Periph_GPIOB
#define  LED_PORT          GPIOB
#define  LED_Pin     GPIO_Pin_12

#define  RCC_LED_CH3           RCC_APB2Periph_GPIOB
#define  LED_PORT_CH3          GPIOB
#define  LED_Pin_CH3     GPIO_Pin_11

#define  RCC_LED_CH4           RCC_APB2Periph_GPIOA
#define  LED_PORT_CH4          GPIOA
#define  LED_Pin_CH4     GPIO_Pin_15


#define  LED_OFF      GPIO_SetBits(LED_PORT,LED_Pin)
#define  LED_ON     GPIO_ResetBits(LED_PORT,LED_Pin)
#define  LED_TOGGLE  LED_PORT->ODR ^= LED_Pin


#define  CH3_LED_OFF      GPIO_SetBits(LED_PORT_CH3,LED_Pin_CH3)
#define  CH3_LED_ON     GPIO_ResetBits(LED_PORT_CH3,LED_Pin_CH3)
#define  CH3_LED_TOGGLE  LED_PORT_CH3->ODR ^= LED_Pin_CH3


#define  CH4_LED_OFF      GPIO_SetBits(LED_PORT_CH4,LED_Pin_CH4)
#define  CH4_LED_ON     GPIO_ResetBits(LED_PORT_CH4,LED_Pin_CH4)
#define  CH4_LED_TOGGLE  LED_PORT_CH4->ODR ^= LED_Pin_CH4

void LED_Configuration(void);
void LED_State(s16 ch3,s16 ch4,float T);

#endif
