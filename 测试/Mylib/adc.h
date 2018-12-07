#ifndef __ADC_H
#define __ADC_H	
#include "board.h" 

void Adc_Init(void); 				//ADC通道初始化
void Get_Adc(void); 				//获得某个通道值 
// u16 Get_Adc(u8 ch)   ;
// u16 Get_Adc_Average(u8 ch,u8 times);//得到某个通道给定次数采样的平均值  

struct GP2Y
		{
			float forward;
			float backward;
			float left;
			float right;
		};

#endif 















