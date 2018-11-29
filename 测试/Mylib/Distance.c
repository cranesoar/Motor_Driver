#include "distance.h"
vec4f distance;
GY53 GY_53;
int DDD,RRR,FFF,BBB;
void Collide_Switch_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE );  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 |GPIO_Pin_4; 
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void GY_53_Distance_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;
  TIM_ICInitTypeDef  TIM_ICInitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

/*---------------------定时器4-----------------------------*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	 
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_TIM4);
  
	TIM4->PSC = 18-1;
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_ICInit(TIM4, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_ICInit(TIM4, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_ICInit(TIM4, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_ICInit(TIM4, &TIM_ICInitStructure);
  
  /* TIM enable counter */
  TIM_Cmd(TIM4, ENABLE);

  /* Enable the CC2 Interrupt Request */
  TIM_ITConfig(TIM4, TIM_IT_CC1, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_CC2, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_CC3, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_CC4, ENABLE);
}
double RC_PPM_CH[4];
void TIM4_IRQHandler(void)		
{
	static u16 temp_cnt[4][2]={0};//    [x通道][0上升沿，1时间长]

	if(TIM4->SR & TIM_IT_CC1) 
	{
		TIM4->SR = ~TIM_IT_CC1;//TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
		TIM4->SR = ~TIM_FLAG_CC1OF;
		if(GPIOB->IDR & GPIO_Pin_6)
		{
			temp_cnt[0][0] = TIM4->CCR1;			
		}
		else 
		{
      temp_cnt[0][1] = TIM4->CCR1 - temp_cnt[0][0];
			RC_PPM_CH[0] = 0.3683 * temp_cnt[0][1];
			distance.data[0]=RC_PPM_CH[0]*0.1;
			GY_53.Right=distance.data[0];
//			RRR=distance.data[0];
		}
	}
	
	if(TIM4->SR & TIM_IT_CC2) 
	{
		TIM4->SR = ~TIM_IT_CC2;//TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
		TIM4->SR = ~TIM_FLAG_CC1OF;
		if(GPIOB->IDR & GPIO_Pin_7)
		{
			temp_cnt[1][0] = TIM4->CCR2;			
		}
		else
		{
      temp_cnt[1][1] = TIM4->CCR2 - temp_cnt[1][0];
			RC_PPM_CH[1] = 0.3683 * temp_cnt[1][1];
			distance.data[1]=RC_PPM_CH[1]*0.1;
			GY_53.Down=distance.data[1];
//	      DDD=distance.data[1];
		}
	}
	
	if(TIM4->SR & TIM_IT_CC3) 
	{
		TIM4->SR = ~TIM_IT_CC3;//TIM_ClearITPendingBit(TIM4, TIM_IT_CC3);
		TIM4->SR = ~TIM_FLAG_CC1OF;
		if(GPIOB->IDR & GPIO_Pin_8)
		{
			temp_cnt[2][0] = TIM4->CCR3;			
		}
		else
		{
      temp_cnt[2][1] = TIM4->CCR3 - temp_cnt[2][0];
			RC_PPM_CH[2] = 0.3683 * temp_cnt[2][1]*0.1;
			distance.data[2]=RC_PPM_CH[2]-20;
			GY_53.Forward=(int)(LPF2pApply_1(distance.data[2]));
//			FFF=distance.data[2];
		}
	}
	
	if(TIM4->SR & TIM_IT_CC4) 
	{
		TIM4->SR = ~TIM_IT_CC4;//TIM_ClearITPendingBit(TIM4, TIM_IT_CC4);
		TIM4->SR = ~TIM_FLAG_CC1OF;
		if(GPIOB->IDR & GPIO_Pin_9)
		{
			temp_cnt[3][0] = TIM4->CCR4;			
		}
		else
		{
      temp_cnt[3][1] = TIM4->CCR4 - temp_cnt[3][0];
			RC_PPM_CH[3] = 0.3683 * temp_cnt[3][1]*0.1;
			distance.data[3]=RC_PPM_CH[3];
			GY_53.Back=(int)(LPF2pApply_2(distance.data[3]));
//			BBB=distance.data[3];
		}
	}
}
