#include "usart1.h"

u8 USART1_RX_BUF[1000];	//发送数据缓冲区
int Voice_distance=0,Voice_distance_last=0,VoiceStart_Flag=0;


void Usart1_Distance_Init(u32 bound)
{  	 
	
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
//  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);//使能FSMC时钟  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART3时钟
	
  //串口1引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOB10复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9 ,GPIO_AF_USART1); //GPIOB9复用为USART1
	
	//USART1
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA1与GPIOA1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA2，PA3
	
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART1, &USART_InitStructure); //初始化串口2
  USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);

  USART_Cmd(USART1, ENABLE);  //使能串口 1
	USART_ClearFlag(USART1, USART_FLAG_TC);	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启接受中断
//	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);      //使能串口空闲中断
	
	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

}

u8 j=0;
/*串口空闲中断使能DMA  DMA1 Stream1 CH4  USART3_RX */  
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
    USART1_RX_BUF[j] =USART_ReceiveData(USART1); 
		j++;
		if(j==2)
		{ Voice_distance=USART1_RX_BUF[0]<<8|USART1_RX_BUF[1];
			Voice_distance=(int)(LPF2pApply_3(Voice_distance));
//			if(ABS(Voice_distance-Voice_distance_last)>700)
//			Voice_distance=Voice_distance_last;
//      else			
//			Voice_distance_last=Voice_distance;
		  j=0;
    }	
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);	
	}

} 

void Usart1_Send_Data()
{
		USART_SendData(USART1, 0xe8);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);//循环发送,直到发送完毕
		DelayUs(40);
		USART_SendData(USART1, 0x02);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);//循环发送,直到发送完毕
		DelayUs(40);
		USART_SendData(USART1, 0xb4);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);//循环发送,直到发送完毕	
	
}



