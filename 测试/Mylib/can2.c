#include "can2.h"
CanRxMsg can2_rx_message;
CanTxMsg can2_tx_message;

/*----CAN2_TX-----PB13----*/
/*----CAN2_RX-----PB12----*/

_can2_feedback can2_feedback;
CanRxMsg CAN2_RX_MESSAGE;
CanTxMsg CAN2_TX_MESSAGE;

void CAN2_Configuration(void)
{
    CAN_InitTypeDef        can;
    CAN_FilterInitTypeDef  can_filter;
    GPIO_InitTypeDef       gpio;
    NVIC_InitTypeDef       nvic;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_CAN2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_CAN2); 

    gpio.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_12 ;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    GPIO_Init(GPIOB, &gpio);

    nvic.NVIC_IRQChannel = CAN2_RX0_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
		
    nvic.NVIC_IRQChannel = CAN2_TX_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 2;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);    

    CAN_DeInit(CAN2);
    CAN_StructInit(&can);

    can.CAN_TTCM = DISABLE;
    can.CAN_ABOM = DISABLE;    
    can.CAN_AWUM = DISABLE;    
    can.CAN_NART = DISABLE;    
    can.CAN_RFLM = DISABLE;    
    can.CAN_TXFP = ENABLE;     
    can.CAN_Mode = CAN_Mode_Normal; 
    can.CAN_SJW  = CAN_SJW_1tq;
    can.CAN_BS1 = CAN_BS1_9tq;
    can.CAN_BS2 = CAN_BS2_4tq;
    can.CAN_Prescaler = 12;   //CAN BaudRate 42/(1+9+4)/3=1Mbps
    CAN_Init(CAN2, &can);
    
    can_filter.CAN_FilterNumber=14;
    can_filter.CAN_FilterMode=CAN_FilterMode_IdMask;
    can_filter.CAN_FilterScale=CAN_FilterScale_32bit;
    can_filter.CAN_FilterIdHigh=0x0000;
    can_filter.CAN_FilterIdLow=0x0000;
    can_filter.CAN_FilterMaskIdHigh=0x0000;
    can_filter.CAN_FilterMaskIdLow=0x0000;
    can_filter.CAN_FilterFIFOAssignment=0;//the message which pass the filter save in fifo0
    can_filter.CAN_FilterActivation=ENABLE;
    CAN_FilterInit(&can_filter);
    
    CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);
		CAN_ITConfig(CAN2,CAN_IT_TME,ENABLE); 
}

void CAN2_RX0_IRQHandler(void)
{
    if (CAN_GetITStatus(CAN2,CAN_IT_FMP0)!= RESET)
	  {
        CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);
        CAN_Receive(CAN2, CAN_FIFO0, &CAN2_RX_MESSAGE);
    }
		switch(CAN2_RX_MESSAGE.StdId)
		{
				case 0x201:																		//拨弹轮速度、位置由can2返回
				{
					can2_feedback.feedSprocket_speed = (int16_t)((CAN2_RX_MESSAGE.Data[2]<<8)|(CAN2_RX_MESSAGE.Data[3]));
						if(ABS(can2_feedback.feedSprocket_speed) <= 10)
							can2_feedback.feedSprocket_speed=0;
						break;
					can2_feedback.feedSprocket_position = (int16_t)((CAN2_RX_MESSAGE.Data[0]<<8)|(CAN2_RX_MESSAGE.Data[1]));
						if(can2_feedback.feedSprocket_position - can2_feedback.feedSprocket_position_last > 6000)				//过圈处理
							can2_feedback.feedSprocket_position_cnt--;
						if(can2_feedback.feedSprocket_position - can2_feedback.feedSprocket_position_last < -6000)
							can2_feedback.feedSprocket_position_cnt++;
						can2_feedback.feedSprocket_position_sum = can2_feedback.feedSprocket_position + can2_feedback.feedSprocket_position_cnt * 8192;			
						can2_feedback.feedSprocket_position_last = can2_feedback.feedSprocket_position;
				}
								
				default:break;			
		} 
		
}

void CAN2_TX_IRQHandler(void)
{
  if (CAN_GetITStatus(CAN2,CAN_IT_TME)!= RESET) 
	{	   
		CAN_ClearITPendingBit(CAN2,CAN_IT_TME);
	  can_tx_success_flag=1;
  }
	CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);	
}

void TDT_feedSprocket_OutUpdate(float* controllerOut)
{
		CanTxMsg Can2TxMsg;
	
		Can2TxMsg.IDE = 0;             //标准帧
		Can2TxMsg.RTR = 0;             //数据帧
		Can2TxMsg.DLC = 8;             //帧长度
		Can2TxMsg.StdId =  0x200;      //ID:0x200
		Can2TxMsg.Data[0] = (u8)((int16_t)(*controllerOut)>>8);
		Can2TxMsg.Data[1] = (u8)((int16_t)(*controllerOut));

		CAN_Transmit(CAN2,&Can2TxMsg);
		can_tx_success_flag = 0;
}
