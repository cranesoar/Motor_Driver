#include "485motor.h"

	u8 Send485buf[10]; 
	u8 rx_rs485buf[10]; 
	u8 SendBuff[SEND_BUF_SIZE];	//发送数据缓冲区
  u8 RxBuff[1000];	//发送数据缓冲区
   	int16_t DM50_POSTION=0;
	int16_t DM50_SPEED=0;
	int16_t speed=0;
	long int DM50_POSTION_SUM=0;
	
	void RS485_Init(u32 bound)
{  	 
	
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC,ENABLE); //使能GPIOA时钟
 // RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);//使能FSMC时钟  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART2时钟
	
  //串口2引脚复用映射
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOB10复用为USART3TX
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOB11复用为USART3RX
	
	//USART2    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOA2与GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure); //初始化PA2，PA3
	
	//PG8推挽输出，485模式控制  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //GPIOC2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure); //初始化PG8
//	RS485_TX_EN=1;
	USART_OverSampling8Cmd(USART3,ENABLE);
   //USART2 初始化设置
	USART_DeInit(USART3); 
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART3, &USART_InitStructure); //初始化串口2
  USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);

	
  USART_Cmd(USART3, ENABLE);  //使能串口 2
	USART_ClearFlag(USART3, USART_FLAG_TC);
	
//	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启接受中断
	USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);      //使能串口空闲中断
	

	//Usart2 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

	
	RS485_TX_EN=0;				//默认为接收模式	
}

//发送函数
void Init_485motor_send(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟

  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM3,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}


//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
	u8 sum=0;
	u16 i;
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
		RS485_TX_EN=1;			//设置为发送模式
		DelayUs(10);
		SendBuff[0]= 0xA0;
		SendBuff[1] = (u8)((int16_t)(speed)>>8);
		SendBuff[2] = (u8)((int16_t)(speed));
		SendBuff[3] = (u8)((int16_t)(speed)>>8);
		SendBuff[4] = (u8)((int16_t)(speed));
		SendBuff[5] = (u8)((int16_t)(speed)>>8);
		SendBuff[6] = (u8)((int16_t)(speed));
		SendBuff[7] = (u8)((int16_t)(speed)>>8);
		SendBuff[8] = (u8)((int16_t)(speed));
		for(i=0;i<9;i++)
		{
			sum+=SendBuff[i];
		}
		SendBuff[9]=sum;

		USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);  //使能串口1的DMA发送     
    MYDMA_Enable(DMA1_Stream3,SEND_BUF_SIZE);     //开始一次DMA传输！	
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
}
//接收缓存区 	
u8 RS485_RX_BUF[64];  	//接收缓冲,最大64个字节.
//接收到的数据长度
u8 RS485_RX_CNT=0;   
void USART3_IRQHandler(void)
{
	static long int DM50_POSTION_SUM_LAST=0;
	static int16_t DM50_POSTION_LAST=0;
	static long int count=0;
	u16 ii;

	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
	{
		DMA_Cmd(DMA1_Stream1,DISABLE);
		ii = USART3->SR;  
		ii = USART3->DR; 
		DM50_POSTION = (int16_t)((RxBuff[1]<<8)|(RxBuff[2]));
		if(DM50_POSTION - DM50_POSTION_LAST<-16384)
			count++;
		if(DM50_POSTION - DM50_POSTION_LAST>16384)
			count--;
		
		DM50_POSTION_SUM = count*32768 + DM50_POSTION;
	
		DM50_SPEED = DM50_POSTION_SUM - DM50_POSTION_SUM_LAST;
		

		DM50_POSTION_LAST = DM50_POSTION;
		DM50_POSTION_SUM_LAST = DM50_POSTION_SUM;
		
		USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);
		DMA1_Stream1->NDTR = RX_BUF_SIZE;
		USART_ClearITPendingBit(USART3, USART_IT_IDLE);
		DMA_Cmd(DMA1_Stream1,ENABLE);
		
	}
	
} 
//RS485发送len个字节.
//buf:发送区首地址
//len:发送的字节数(为了和本代码的接收匹配,这里建议不要超过64个字节)
void RS485_Send_Data(u8 *buf,u8 len)
{
	u8 t;
//	RS485_TX_EN=1;			//设置为发送模式
  	for(t=0;t<len;t++)		//循环发送数据
	{
	  while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //等待发送结束		
    USART_SendData(USART3,buf[t]); //发送数据
	}	 
	//while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //等待发送结束		
	RS485_RX_CNT=0;	  
//	RS485_TX_EN=0;				//设置为接收模式	
}
//RS485查询接收到的数据
//buf:接收缓存首地址
//len:读到的数据长度
void RS485_Receive_Data(u8 *buf,u8 *len)
{
	u8 rxlen=RS485_RX_CNT;
	u8 i=0;
	*len=0;				//默认为0
	if(rxlen==RS485_RX_CNT&&rxlen)//接收到了数据,且接收完成了
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=RS485_RX_BUF[i];	
		}		
		*len=RS485_RX_CNT;	//记录本次数据长度
		RS485_RX_CNT=0;		//清零
	}
}