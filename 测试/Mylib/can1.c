#include "can1.h"

/*----CAN1_TX-----PA12----*/
/*----CAN1_RX-----PA11----*/
CanRxMsg CAN1_RX_MESSAGE;
CanTxMsg CAN1_TX_MESSAGE;

_can1_feedback can1_feedback;

void CAN1_Configuration(void)
{
    CAN_InitTypeDef        can;
    CAN_FilterInitTypeDef  can_filter;
    GPIO_InitTypeDef       gpio;
    NVIC_InitTypeDef       nvic;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1);

    gpio.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_11;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    GPIO_Init(GPIOA, &gpio);
    
    nvic.NVIC_IRQChannel = CAN1_RX0_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
    
    nvic.NVIC_IRQChannel = CAN1_TX_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 2;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);    
    
    CAN_DeInit(CAN1);
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
    CAN_Init(CAN1, &can);

		can_filter.CAN_FilterNumber=0;
		can_filter.CAN_FilterMode=CAN_FilterMode_IdMask;
		can_filter.CAN_FilterScale=CAN_FilterScale_32bit;
		can_filter.CAN_FilterIdHigh=0x0000;
		can_filter.CAN_FilterIdLow=0x0000;
		can_filter.CAN_FilterMaskIdHigh=0x0000;
		can_filter.CAN_FilterMaskIdLow=0x0000;
		can_filter.CAN_FilterFIFOAssignment=0;
		can_filter.CAN_FilterActivation=ENABLE;
		CAN_FilterInit(&can_filter);
			
		CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
		CAN_ITConfig(CAN1,CAN_IT_TME,ENABLE); 
}

unsigned char can_tx_success_flag = 0;

void CAN1_TX_IRQHandler(void)
{
  if (CAN_GetITStatus(CAN1,CAN_IT_TME)!= RESET) 
	{
	   CAN_ClearITPendingBit(CAN1,CAN_IT_TME);
		 can_tx_success_flag = 1;
  }
}
   
void CAN1_RX0_IRQHandler(void)
{
	
    if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET)
	  {
        CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
        CAN_Receive(CAN1, CAN_FIFO0, &CAN1_RX_MESSAGE);
    }
		switch(CAN1_RX_MESSAGE.StdId)
		{
				case 0x201:
				{ 
					can1_feedback.chassisMotor1speed = (int16_t)((CAN1_RX_MESSAGE.Data[2]<<8)|(CAN1_RX_MESSAGE.Data[3]));
					if(ABS(can1_feedback.chassisMotor1speed) <= 10)
						can1_feedback.chassisMotor1speed=0;
					break;
				}
				case 0x202:
				{
					can1_feedback.chassisMotor2speed = (int16_t)((CAN1_RX_MESSAGE.Data[2]<<8)|(CAN1_RX_MESSAGE.Data[3]));
					if(ABS(can1_feedback.chassisMotor1speed) <= 10)
						can1_feedback.chassisMotor2speed = 0;
					break;
				}
				case 0x203:
				{
					can1_feedback.chassisMotor3speed = (int16_t)((CAN1_RX_MESSAGE.Data[2]<<8)|(CAN1_RX_MESSAGE.Data[3]));
					if(ABS(can1_feedback.chassisMotor3speed) <= 10)	
						can1_feedback.chassisMotor3speed = 0;
					break;
				}
				case 0x204:
				{
					can1_feedback.chassisMotor4speed = (int16_t)((CAN1_RX_MESSAGE.Data[2]<<8)|(CAN1_RX_MESSAGE.Data[3]));
					if(ABS(can1_feedback.chassisMotor4speed) <= 10)
						can1_feedback.chassisMotor4speed = 0;
				}
				default:break;			
		} 

}

/*底盘四电机发送函数*/
void TDT_Chassis_OutUpdate(vec4f* controllerOut)
{
		CanTxMsg Can1TxMsg;
	
		Can1TxMsg.IDE = 0;             //标准帧
		Can1TxMsg.RTR = 0;             //数据帧
		Can1TxMsg.DLC = 8;             //帧长度

		Can1TxMsg.StdId =  0x200;      //ID:0x200
		Can1TxMsg.Data[0] = (u8)((int16_t)controllerOut->data[0]>>8);
		Can1TxMsg.Data[1] = (u8)(controllerOut->data[0]);
		Can1TxMsg.Data[2] = (u8)((int16_t)controllerOut->data[1]>>8);
		Can1TxMsg.Data[3] = (u8)(controllerOut->data[1]);
		Can1TxMsg.Data[4] = (u8)((int16_t)controllerOut->data[2]>>8);
		Can1TxMsg.Data[5] = (u8)(controllerOut->data[2]);
		Can1TxMsg.Data[6] = (u8)((int16_t)controllerOut->data[3]>>8);
		Can1TxMsg.Data[7] = (u8)(controllerOut->data[3]);
  
    if(RC.CH[5]==2)
    {
    Can1TxMsg.Data[0] = 0;
		Can1TxMsg.Data[1] = 0;
		Can1TxMsg.Data[2] = 0;
		Can1TxMsg.Data[3] = 0;
		Can1TxMsg.Data[4] = 0;
		Can1TxMsg.Data[5] = 0;
		Can1TxMsg.Data[6] = 0;
		Can1TxMsg.Data[7] = 0;  
      
    }

		CAN_Transmit(CAN1,&Can1TxMsg);
		can_tx_success_flag = 0;
}

