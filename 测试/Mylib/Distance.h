#ifndef _DISTANCE_H__
#define _DISTANCE_H__

#include "board.h"

void GY_53_Distance_Init(void);
void TIM4_IRQHandler(void); 
void Collide_Switch_Init(void);

extern vec4f distance;
extern GY53 GY_53;;
#endif
