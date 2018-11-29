#ifndef __LED_H__
#define __LED_H__

#include "board.h"

 
/***************LED GPIO¶¨Òå******************/

#define  LED_BLUE_ON      LED_PORT->BSRRH = GREEN_LED_Pin
#define  LED_BLUE_OFF     LED_PORT->BSRRL = GREEN_LED_Pin
#define  LED_BLUE_TOGGLE  LED_PORT->ODR ^= GREEN_LED_Pin

#define  LED_RED_ON        LED_PORT->BSRRH = RED_LED_Pin
#define  LED_RED_OFF       LED_PORT->BSRRL = RED_LED_Pin
#define  LED_RED_TOGGLE    LED_PORT->ODR ^= RED_LED_Pin

#define  LED_GREEN_ON        LED_PORT->BSRRH = BLUE_LED_Pin
#define  LED_GREEN_OFF       LED_PORT->BSRRL = BLUE_LED_Pin
#define  LED_GREEN_TOGGLE    LED_PORT->ODR ^= BLUE_LED_Pin

#define  LED_YELLOW_ON      LED_PORT->BSRRH = GREEN_LED_Pin;LED_PORT->BSRRH = RED_LED_Pin
#define  LED_YELLOW_OFF     LED_PORT->BSRRL = GREEN_LED_Pin;LED_PORT->BSRRL = RED_LED_Pin
#define  LED_YELLOW_TOGGLE  LED_PORT->ODR ^= RED_LED_Pin;LED_PORT->ODR ^= GREEN_LED_Pin

#define  LED_WHITE_ON			LED_PORT->BSRRH = GREEN_LED_Pin;LED_PORT->BSRRH = RED_LED_Pin;LED_PORT->BSRRH = BLUE_LED_Pin
#define  LED_ALL_OFF     	LED_PORT->BSRRL = GREEN_LED_Pin;LED_PORT->BSRRL = RED_LED_Pin;LED_PORT->BSRRL = BLUE_LED_Pin



void Led_Init(void);

#endif
