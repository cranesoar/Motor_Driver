#ifndef __PWM_H__
#define __PWM_H__

void PWM_Configuration(int arr);
void PWM_OutSet(int value);
void DIR_Configuration(void);
/***************DIR GPIO¶¨Òå******************/

#define  DIR1_H      GPIO_SetBits(GPIOA,GPIO_Pin_7)
#define  DIR1_L      GPIO_ResetBits(GPIOA,GPIO_Pin_7)
#define  DIR1_TOGGLE  GPIOA->ODR ^= GPIO_Pin_7

#define  DIR2_H      GPIO_SetBits(GPIOA,GPIO_Pin_6)
#define  DIR2_L      GPIO_ResetBits(GPIOA,GPIO_Pin_6)
#define  DIR2_TOGGLE  GPIOA->ODR ^= GPIO_Pin_6

#define  DIR3_H      GPIO_SetBits(GPIOA,GPIO_Pin_4)
#define  DIR3_L      GPIO_ResetBits(GPIOA,GPIO_Pin_4)
#define  DIR3_TOGGLE  GPIOA->ODR ^= GPIO_Pin_4

#endif
