#ifndef _CAMERA_H__
#define _CAMERA_H__

#include "board.h"


typedef struct _Position
   {
		 u8 xx;
		 u8 yy;	 
   }Position;

extern Position	Position_box; 

void UART5_IRQHandler(void);
void Usart_Camera_Init(void);
#endif