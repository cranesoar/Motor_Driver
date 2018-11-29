#ifndef _PARAMETER_H
#define	_PARAMETER_H

#include "board.h"

#define pidChassisMove 		0
#define pidChassisRotate  1
#define pidPosition_3510Inner     2
#define pidPosition_3510Outer			3
#define pidPitchInner     4
#define pidPitchOuter			5
#define pidYawInner     6
#define pidYawOuter			7
#define pidWhiffInner 8
#define pidWhiffOuter 9
#define pidPitchInnerMech 10
#define pidPitchOuterMech 11
#define pidfeedSprocket_Inner 12
#define pidfeedSprocket_Outer 13


void TDT_Get_PIDparameters(pid* pidStruct, u8 pidIndex);
void Get_Parameters(void);
void Struct_Init(void);


#endif
