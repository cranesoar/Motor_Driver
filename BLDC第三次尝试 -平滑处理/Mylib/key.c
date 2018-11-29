#include "main.h"

int last_time=0;
void KEY_Configuration(void)
{
    GPIO_InitTypeDef gpio;  
    EXTI_InitTypeDef exti;
    NVIC_InitTypeDef nvic;

	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    
	  gpio.GPIO_Pin = GPIO_Pin_10;
		gpio.GPIO_Mode = GPIO_Mode_IPD;
		gpio.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_Init(GPIOB, &gpio);
    
    exti.EXTI_Line = EXTI_Line10;
    exti.EXTI_Mode = EXTI_Mode_Interrupt;
    exti.EXTI_Trigger = EXTI_Trigger_Falling;
    exti.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti);
    
    nvic.NVIC_IRQChannel = EXTI15_10_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
    
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource10);
}

void EXTI15_10_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line10)!=RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line10);
//			if(last_time-sys_time_ms>20)
//			{
				BLDC_step++;
			 if(BLDC_step>6)
				 BLDC_step=1;
//			}
			last_time =	sys_time_ms;
    }
}
