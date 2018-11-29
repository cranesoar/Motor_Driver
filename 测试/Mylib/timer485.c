#include "timer485.h"

int DM50_SPEED_Yaw=0,DM50_SPEED_Pitch1=0,DM50_SPEED_Pitch2= 0;
void Gimbal_TIM2_Init(void)   
{
  TIM_TimeBaseInitTypeDef  tim;
  NVIC_InitTypeDef         nvic;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    
  nvic.NVIC_IRQChannel = TIM2_IRQn;
  nvic.NVIC_IRQChannelPreemptionPriority = 1;
  nvic.NVIC_IRQChannelSubPriority = 3;
  nvic.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvic);

  tim.TIM_Prescaler =672-1;
  tim.TIM_CounterMode = TIM_CounterMode_Up;
  tim.TIM_ClockDivision = TIM_CKD_DIV1;
  tim.TIM_Period = 125-1;
  TIM_TimeBaseInit(TIM2,&tim);
  TIM_Cmd(TIM2, ENABLE);	 
  TIM_ITConfig(TIM2, TIM_IT_Update,ENABLE);
}



void TIM2_IRQHandler(void)  
{
	u8 i;
	u8 sum=0;
  if (TIM_GetITStatus(TIM2,TIM_IT_Update)!= RESET) 
	{  
			SendBuff[0]= 0xA0;
			SendBuff[1] = (u8)((int16_t)(DM50_SPEED_Pitch1)>>8);
			SendBuff[2] = (u8)((int16_t)(DM50_SPEED_Pitch1));		
			SendBuff[3] = (u8)((int16_t)(DM50_SPEED_Pitch2)>>8);
			SendBuff[4] = (u8)((int16_t)(DM50_SPEED_Pitch2));      //	
  		SendBuff[5] = (u8)((int16_t)(0)>>8);
			SendBuff[6] = (u8)((int16_t)(0));		
			SendBuff[7] = (u8)((int16_t)(DM50_SPEED_Yaw)>>8);
			SendBuff[8] = (u8)((int16_t)(DM50_SPEED_Yaw));
       
      if(RC.CH[5]==2)
      {
			SendBuff[0]= 0xA0;
			SendBuff[1] = (u8)((int16_t)(0)>>8);
			SendBuff[2] = (u8)((int16_t)(0));		
			SendBuff[3] = (u8)((int16_t)(0)>>8);
			SendBuff[4] = (u8)((int16_t)(0));      //	
  		SendBuff[5] = (u8)((int16_t)(0)>>8);
			SendBuff[6] = (u8)((int16_t)(0));		
			SendBuff[7] = (u8)((int16_t)(0)>>8);
			SendBuff[8] = (u8)((int16_t)(0));               
      }
              
			for(i=0;i<9;i++)
			{
				sum+=SendBuff[i];
			}
			SendBuff[9]=sum;
			RS485_TX_EN;			//设置为发送模式
			DelayUs(20);
			USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);  //使能串口3的DMA发送     
			MYDMA_Enable(DMA1_Stream3,SEND_BUF_SIZE);     //开始一次DMA传输！	
		
      TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
      TIM_ClearFlag(TIM2, TIM_FLAG_Update);	
	}		
}

