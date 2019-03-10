#ifndef __ADC_H__
#define __ADC_H__

#include "main.h"

//全局变量
//AD采样存放空间
 
extern u8 filter_flag;

void Protect_AdcInit(void);
void ADC1_2_IRQHandler(void);
void Adcdma_Init(void);
#endif



