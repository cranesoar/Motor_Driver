#include "mlt8530.h"
void TDT_TIM5_PWM_Bell_Init(void)
{	
		GPIO_InitTypeDef         GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef        TIM_OCInitStructure;
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5); 
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOA,&GPIO_InitStructure);
			
		TIM_TimeBaseStructure.TIM_Prescaler=20;
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
		TIM_TimeBaseStructure.TIM_Period=10000;
		TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
		
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		
		TIM_OC1Init(TIM5, &TIM_OCInitStructure);
		
		TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable); 
		TIM_ARRPreloadConfig(TIM5, ENABLE);
		
		TIM_Cmd(TIM5, ENABLE); 

 		TIM5->CCR1=0;
}



void reconfig(int pre)
{
	TIM_Cmd(TIM5, DISABLE); 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Prescaler=pre;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period=10000;
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
	TIM_Cmd(TIM5, ENABLE); 
	TIM5->CCR1=5000;

}

int DJI[18] =      {0 ,22 ,0, 17,0, 13, 0, 22, 0, 17, 0, 22, 0, 17, 0, 22, 0,17 };
int DJI_time[18] = {20,200,5,200,5,400,20,100,20,100,20,200,20,200,20,200,20,200};
void start_task(int len)
{
	static int my_time=0;
	static int step,aim_time,finished_flag,setp_last;
	
	if(finished_flag!=1)
	{
		if(DJI[step]!=0)
		{
			if(setp_last!=step)
				reconfig(DJI[step]);
			setp_last=step;
		}
		else
			TIM5->CCR1=0;

		if(my_time>DJI_time[step])
		{
			step++;
			my_time=0;
			if(step==len)
				finished_flag=1;
		}
		my_time++;
	}
	else
	{
		TIM5->CCR1=0;
	}
	
}
