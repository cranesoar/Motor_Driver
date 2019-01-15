#include "main.h"

void LED_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_LED|RCC_LED_CH3|RCC_LED_CH4|RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
    GPIO_InitStructure.GPIO_Pin = LED_Pin|GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LED_Pin_CH4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    LED_OFF;
    CH3_LED_ON;
    CH4_LED_ON;
}


void LED_State(s16 ch3,s16 ch4,float T)
{
    static float time_ch3,time_ch4;
    static float set_ch3,set_ch4;

    if(ch3 == 0)
        CH3_LED_OFF;
    else
        set_ch3 = 1/(ABS(ch3)/50.0f);
    if(time_ch3>set_ch3)
    {
        CH3_LED_TOGGLE;
        time_ch3 = 0;
    }

    if(ch4 == 0)
        CH4_LED_OFF;
    else
        set_ch4 = 1/(ABS(ch4)/50.0f);
    if(time_ch4>set_ch4)
    {
        CH4_LED_TOGGLE;
        time_ch4 = 0;
    }

    time_ch3 += T;
    time_ch4 += T;
}

