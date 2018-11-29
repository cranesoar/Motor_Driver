#ifndef _TASK_CONTROL_H
#define	_TASK_CONTROL_H

#include "board.h"

#define Left_Loosen  GPIO_SetBits(GPIOB,GPIO_Pin_6)//GPIOB->BSRRH=GPIO_Pin_3
#define Left_Clamp GPIO_ResetBits(GPIOB,GPIO_Pin_6)//GPIOB->BSRRL=GPIO_Pin_3

#define Right_Loosen GPIO_SetBits(GPIOB,GPIO_Pin_7)
#define Right_Clamp GPIO_ResetBits(GPIOB,GPIO_Pin_7)

#define Left_valve_open   GPIO_SetBits(GPIOB,GPIO_Pin_8)
#define Left_valve_close  GPIO_ResetBits(GPIOB,GPIO_Pin_8)

#define Right_valve_open   GPIO_SetBits(GPIOB,GPIO_Pin_9)
#define Right_valve_close  GPIO_ResetBits(GPIOB,GPIO_Pin_9)

typedef struct
	{
		 uint8_t   left_val;
	   uint8_t   last_left_val;		
		 uint8_t   right_val;
		 uint8_t   last_right_val;
	} mouse;

extern 	mouse mymouse;
	
void Task_Init(void);
void TsskFlag_getvalue(void);
void Task_control(void);
void Pump_Start(void);

#endif
