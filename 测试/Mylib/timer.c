
#include "timer.h"

void TDT_TIM3_PWM_Init(void)
{	
		GPIO_InitTypeDef         GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef        TIM_OCInitStructure;
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
		
		GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM3); 
		GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM3); 
		GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM3); 
		GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM3); 
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOC,&GPIO_InitStructure);
			
		TIM_TimeBaseStructure.TIM_Prescaler=42-1;
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
		TIM_TimeBaseStructure.TIM_Period=10000;
		TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
		
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		
		TIM_OC1Init(TIM3, &TIM_OCInitStructure);
		TIM_OC2Init(TIM3, &TIM_OCInitStructure);
		TIM_OC3Init(TIM3, &TIM_OCInitStructure);
		TIM_OC4Init(TIM3, &TIM_OCInitStructure);
		
   	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable); 
		TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); 
   	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable); 
		TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
		TIM_ARRPreloadConfig(TIM3, ENABLE);
		
		TIM_Cmd(TIM3, ENABLE); 

//		TIM3->CCR1=4000;
////		TIM3->CCR2=MAX_PWM;
////		TIM3->CCR3=MAX_PWM;
////		TIM3->CCR4=MAX_PWM;
//		DelayMs(2000);

// 		TIM3->CCR1=4000;
// 		TIM3->CCR2=MIN_PWM;
////		TIM3->CCR3=MIN_PWM;
//// 		TIM3->CCR4=MIN_PWM;
// 		DelayMs(2000);	
}

void TDT_TIM4_PWM_Init(void)
{	
		GPIO_InitTypeDef         GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef        TIM_OCInitStructure;
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4); 
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_TIM4); 
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_TIM4); 
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_TIM4); 
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOB,&GPIO_InitStructure);
			
		TIM_TimeBaseStructure.TIM_Prescaler=42-1;
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
		TIM_TimeBaseStructure.TIM_Period=10000;
		TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
		
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		
		TIM_OC1Init(TIM4, &TIM_OCInitStructure);
		TIM_OC2Init(TIM4, &TIM_OCInitStructure);
		TIM_OC3Init(TIM4, &TIM_OCInitStructure);
		TIM_OC4Init(TIM4, &TIM_OCInitStructure);
		
   	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable); 
		TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable); 
   	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable); 
		TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
		TIM_ARRPreloadConfig(TIM4, ENABLE);
		
		TIM_Cmd(TIM4, ENABLE); 

//		TIM3->CCR1=4000;
////		TIM3->CCR2=MAX_PWM;
////		TIM3->CCR3=MAX_PWM;
////		TIM3->CCR4=MAX_PWM;
//		DelayMs(2000);

// 		TIM3->CCR1=4000;
// 		TIM3->CCR2=MIN_PWM;
////		TIM3->CCR3=MIN_PWM;
//// 		TIM3->CCR4=MIN_PWM;
// 		DelayMs(2000);	
}


void TIM7_Key_Configuration(void)    //2ms中断
{
  TIM_TimeBaseInitTypeDef  tim;
  NVIC_InitTypeDef         nvic;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);
    
  nvic.NVIC_IRQChannel = TIM7_IRQn;
  nvic.NVIC_IRQChannelPreemptionPriority = 0;
  nvic.NVIC_IRQChannelSubPriority = 3;
  nvic.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvic);

  tim.TIM_Prescaler =840-1;
  tim.TIM_CounterMode = TIM_CounterMode_Up;
  tim.TIM_ClockDivision = TIM_CKD_DIV1;
  tim.TIM_Period = 250-1;
  TIM_TimeBaseInit(TIM7,&tim);
  TIM_Cmd(TIM7, ENABLE);	 
  TIM_ITConfig(TIM7, TIM_IT_Update,ENABLE);
}

void TIM7_IRQHandler(void)  
{
  if (TIM_GetITStatus(TIM7,TIM_IT_Update)!= RESET) 
	{
		if(Dbus_Updata_flag==1)
		{
			/***各按键是否按下,按下为1***/
			RC.key.B = Get_Keypress(KEY_B);
			RC.key.V = Get_Keypress(KEY_V);
			RC.key.C = Get_Keypress(KEY_C);
			RC.key.X = Get_Keypress(KEY_X);
			RC.key.Z = Get_Keypress(KEY_Z);
			RC.key.G = Get_Keypress(KEY_G);
			RC.key.F = Get_Keypress(KEY_F);
			RC.key.R = Get_Keypress(KEY_R);
			RC.key.E = Get_Keypress(KEY_E);
			RC.key.Q = Get_Keypress(KEY_Q);
			RC.key.CTRL = Get_Keypress(KEY_CTRL);
			RC.key.SHIFT = Get_Keypress(KEY_SHIFT);
			RC.key.D = Get_Keypress(KEY_D);
			RC.key.A = Get_Keypress(KEY_A);
			RC.key.S = Get_Keypress(KEY_S);
			RC.key.W = Get_Keypress(KEY_W);
		}

			Dbus_Updata_flag=0;
      TIM_ClearITPendingBit(TIM7,TIM_IT_Update);
      TIM_ClearFlag(TIM7, TIM_FLAG_Update);	
	}		
}
