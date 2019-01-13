#include "main.h"

//PA8----EN
//PA9----PWM---TIM1_CH2
//PA10---EN
//PA11---PWM---TIM1_CH4

/*************************************************************************
                              PWM初始化
*************************************************************************/
void PWM_Configuration(int arr)
{
    TIM_TimeBaseInitTypeDef  tim;
    TIM_OCInitTypeDef        oc;
    GPIO_InitTypeDef         gpio;

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM1, ENABLE);

    gpio.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio);

    tim.TIM_Period=arr-1;    //arr             Fpwm=72M/((arr+1)*(psc+1))
    tim.TIM_Prescaler=18-1;   //psc             duty circle= TIMx->CCRx/arr
    tim.TIM_ClockDivision=TIM_CKD_DIV1;
    tim.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &tim);

    oc.TIM_OCMode = TIM_OCMode_PWM1;
    oc.TIM_OutputState = TIM_OutputState_Enable;
    oc.TIM_OutputNState = TIM_OutputNState_Disable;
    oc.TIM_Pulse = 0;
    oc.TIM_OCPolarity = TIM_OCPolarity_High;
    oc.TIM_OCNPolarity = TIM_OCPolarity_High;
    oc.TIM_OCIdleState = TIM_OCIdleState_Reset;
    oc.TIM_OCNIdleState = TIM_OCIdleState_Reset;
    TIM_OC1Init(TIM1, &oc);
    TIM_OC2Init(TIM1, &oc);

    TIM_ARRPreloadConfig(TIM1, ENABLE);

    TIM_CtrlPWMOutputs(TIM1,ENABLE);

    TIM_Cmd(TIM1, ENABLE);

    TIM1->CCR1 = 8000;
    TIM1->CCR2 = 8000;
    delay_ms(1000);
    TIM1->CCR1 = 4000;
    TIM1->CCR2 = 4000;
    delay_ms(2000);
}

/*************************************************************************
                              电压输出
*************************************************************************/
void PWM_OutSet(int value)
{
    int output = limit(value, -3000, 3000);						//给定值限幅

    if(output > 0)																		//根据给定值设置上桥输出和下桥通断
    {
        TIM2->CCR1 = output;
        TIM2->CCR2 = 0;
    }
    else
    {
        TIM2->CCR1 = 0;
        TIM2->CCR2 = -output;
    }
}
/*************************************************************************
															DIR
*************************************************************************/
void DIR_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_6|GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    DIR1_H;
    DIR2_H;
    DIR3_H;
}
