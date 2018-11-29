#ifndef __CAN2_H_
#define __CAN2_H_

#include "board.h"
	
typedef struct 
{
	int16_t feedSprocket_speed;
	int16_t feedSprocket_position;
	int16_t feedSprocket_position_last;
	int16_t feedSprocket_position_cnt;
	int32_t feedSprocket_position_sum;

}_can2_feedback;

extern _can2_feedback can2_feedback;  

void CAN2_Configuration(void);
void TDT_feedSprocket_OutUpdate(float* controllerOut);
#endif 
