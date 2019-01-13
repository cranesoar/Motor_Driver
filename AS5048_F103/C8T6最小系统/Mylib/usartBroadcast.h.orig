#ifndef __USART_BROADCAST_H__
#define __USART_BROADCAST_H__

#include "stm32f10x.h"

#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

typedef struct _dataPrepare{
															int data[14];
													 }_dataPrepare;

void USART_Broadcast(_dataPrepare* dataStruct);
void USART_Send(void);
													 
#endif
