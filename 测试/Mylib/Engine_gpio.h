#ifndef _ENGINE_GPIO_H_
#define	_ENGINE_GPIO_H_

#include "board.h"

#define Ascend_Loosen  GPIO_SetBits(GPIOB,GPIO_Pin_3)
#define Ascend_Clamp   GPIO_ResetBits(GPIOB,GPIO_Pin_3)
#define UP_POSITION   545000
#define GET_SPEED     1000
#define Island_Loosen  	  GPIO_SetBits(GPIOC,GPIO_Pin_15);GPIO_ResetBits(GPIOC,GPIO_Pin_14)
#define Island_Clamp      GPIO_SetBits(GPIOC,GPIO_Pin_14);GPIO_ResetBits(GPIOC,GPIO_Pin_15)
#define Bullet_Clamp      GPIO_SetBits(GPIOC,GPIO_Pin_3)
#define Bullet_Loosen      GPIO_ResetBits(GPIOC,GPIO_Pin_3)

extern float get_speed;

void Ascend_Task_Init(void);
void TsskFlag_getvalue(void);
void Ascend_Task_Control(void);

#endif