#ifndef __CAN1_H__
#define __CAN1_H__

#include "board.h"

typedef struct 
{
	int16_t chassisMotor1speed;
	int16_t chassisMotor2speed;
	int16_t chassisMotor3speed;
	int16_t chassisMotor4speed;

}_can1_feedback;

//extern u8 CAN1_TX_Finished_flag;
extern _can1_feedback can1_feedback;
extern unsigned char can_tx_success_flag;
   
void CAN1_Configuration(void);
void TDT_Chassis_OutUpdate(vec4f* controllerOut);//底盘四电机发送函数
	


#endif 
