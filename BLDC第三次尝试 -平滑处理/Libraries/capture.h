#ifndef __CAPTURE_H__
#define __CAPTURE_H__
#include "main.h"


void TIM3_Cap_Init(u16 arr,u16 psc);


extern u8  TIM3CH3_CAPTURE_STA;	//输入捕获状态		    				
extern u16	TIM3CH3_CAPTURE_VAL;	//输入捕获值

#endif
