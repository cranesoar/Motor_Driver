#ifndef __USART1_H
#define __USART1_H
#include "board.h" 

void Usart1_Distance_Init(u32 bound);
void USART1_IRQHandler(void);
void Usart1_Send_Data(void);
extern int Voice_distance;


#endif