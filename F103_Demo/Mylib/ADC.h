#ifndef __ADC_H__
#define __ADC_H__

#include "main.h"

//全局变量
//AD采样存放空间
union adcbuff
{
    int16_t ADCConvertedValue[7];
    struct _ad {
        int16_t VSENA;
        int16_t VSENB;
        int16_t VSENC;
        int16_t VSENPVDD;
        int16_t ISENA;
        int16_t ISENB;
        int16_t ISENC;
    } ad;
};

extern union adcbuff ADCBUFF;

void Protect_AdcInit(void);
void ADC1_2_IRQHandler(void);
void Adcdma_Init(void);
#endif



