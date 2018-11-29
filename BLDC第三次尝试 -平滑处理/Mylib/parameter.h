#ifndef __PARAMETER_H__
#define __PARAMETER_H__
#include "main.h"

#define pidSPEED 		 0
#define PID_POSTION  1


void Load_Parameters(void); 
extern u16 BLDC_HALL_POS[66];
extern u8 BLDC_POS2STEP[66];
void TDT_Get_PIDparameters(pid* pidStruct, u8 pidIndex);


#endif
