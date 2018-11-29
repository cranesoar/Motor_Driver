#ifndef __USART_H
#define __USART_H
#include <stdio.h>
#include "board.h"
void Hall_init(void);

void USART_SendChar(char b);

int fputc(int ch, FILE *f);
#endif


