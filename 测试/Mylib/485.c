#include "485.h"
int Position_485Yaw=0,Position_485Whiff_Last,Position_485Whiff=0,Position_485Pitch;
int Speed_485Whiff=0; 
int START_Position_Yaw=0;
int START_Position_Whiff=0;
int START_Position_Pitch=0;
int16_t DM50_POSITION_Pitch=0;
int16_t DM50_POSITION_Whiff=0;
int16_t DM50_POSITION_Yaw=0;
//接收缓存区 	
u8 RS485_RX_BUF[64];  	//接收缓冲,最大64个字节.
//接收到的数据长度
u8 RS485_RX_CNT=0;   

u8 SendBuff[SEND_BUF_SIZE];	//发送数据缓冲区
u8 RxBuff[1000];	//发送数据缓冲区
u8 SendBuff[SEND_BUF_SIZE];	//发送数据缓冲区


void RS485_Init(u32 bound)
{  	 
	
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOB,ENABLE); //使能GPIOA时钟
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);//使能FSMC时钟  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART3时钟
	
  //串口3引脚复用映射
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART2); //GPIOB10复用为USART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART2); //GPIOB11复用为USART3
	
	//USART3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10; //GPIOA2与GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure); //初始化PA2，PA3
	
	//PC2推挽输出，485模式控制  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //GPIOC2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure); //初始化PG8
//	RS485_TX_EN=1;
	USART_OverSampling8Cmd(USART3,ENABLE);
   //USART2 初始化设置
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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

	
	RS485_RX_EN;				//默认为接收模式	
}

int dif_yaw = 0;
/*串口空闲中断使能DMA  DMA1 Stream1 CH4  USART3_RX */  
void USART3_IRQHandler(void)
{
	u16 ii;
  u8 sum_Pitch=0, sum_Yaw=0,sum_Whiff=0;
	static int16_t DM50_POSITION_Yaw_Last,DM50_POSITION_Pitch_Last,DM50_POSITION_Whiff_Last;
	static _Bool zero_Yaw_flag=0,start_Yaw_flag=0;
	static _Bool zero_Whiff_flag=0,start_Whiff_flag=0;
	static _Bool zero_Pitch_flag=0,start_Pitch_flag=0;
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
	{
		DMA_Cmd(DMA1_Stream1,DISABLE);

		ii = USART3->SR;  
		ii = USART3->DR;
    if(RxBuff[0]==0xB4)		
		{ 
			sum_Yaw=RxBuff[0]+RxBuff[1]+RxBuff[2];
			if(RxBuff[3]==sum_Yaw)
      {	
			DM50_POSITION_Yaw_Last=DM50_POSITION_Yaw; 				
			DM50_POSITION_Yaw= (int16_t)((RxBuff[1]<<8)|(RxBuff[2]));
			}
			else
			DM50_POSITION_Yaw=DM50_POSITION_Yaw_Last;
					
			if((DM50_POSITION_Yaw-DM50_POSITION_Yaw_Last)>15000)	
			Position_485Yaw+=DM50_POSITION_Yaw-DM50_POSITION_Yaw_Last-32767;	
			else if((DM50_POSITION_Yaw-DM50_POSITION_Yaw_Last)<-15000)						
			Position_485Yaw+=DM50_POSITION_Yaw-DM50_POSITION_Yaw_Last+32767;	
			else
			Position_485Yaw+=DM50_POSITION_Yaw-DM50_POSITION_Yaw_Last;

      if(Position_485Yaw>=(MID_YAW_ANGLE+16384)) 
      Position_485Yaw-=32767;
      else if(Position_485Yaw<=(MID_YAW_ANGLE-16384)) 
      Position_485Yaw+=32767;        

      
			if(DM50_POSITION_Yaw_Last==0&&zero_Yaw_flag==0)
			Position_485Yaw=DM50_POSITION_Yaw;
		
			DM50_POSITION_Yaw_Last=DM50_POSITION_Yaw;	  //mid   31318
			zero_Yaw_flag=1;		
		}
		
		if(RxBuff [0]==0xB2)		
		{ 
			sum_Pitch=RxBuff[0]+RxBuff[1]+RxBuff[2];
			if(RxBuff[3]==sum_Pitch)
			{
			DM50_POSITION_Pitch_Last=DM50_POSITION_Pitch;				
			DM50_POSITION_Pitch = (int16_t)((RxBuff[1]<<8)|(RxBuff[2]));
			}
		}	

		if(RxBuff [0]==0xB1)		
		{ 
			sum_Whiff=RxBuff[0]+RxBuff[1]+RxBuff[2];
			if(RxBuff[3]==sum_Whiff)
			{
			DM50_POSITION_Whiff_Last=DM50_POSITION_Whiff;				
			DM50_POSITION_Whiff = (int16_t)((RxBuff[1]<<8)|(RxBuff[2]));
			}
			else
			DM50_POSITION_Whiff=DM50_POSITION_Whiff_Last;

			if((DM50_POSITION_Whiff-DM50_POSITION_Whiff_Last)>15000)	
			Position_485Whiff+=DM50_POSITION_Whiff-DM50_POSITION_Whiff_Last-32767;	
			else if((DM50_POSITION_Whiff-DM50_POSITION_Whiff_Last)<-15000)						
			Position_485Whiff+=DM50_POSITION_Whiff-DM50_POSITION_Whiff_Last+32767;	
			else
			Position_485Whiff+=DM50_POSITION_Whiff-DM50_POSITION_Whiff_Last;	

			DM50_POSITION_Whiff_Last=DM50_POSITION_Whiff;	  //mid   31744			
		}			
	
		USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);
		DMA1_Stream1->NDTR = RX_BUF_SIZE;
		USART_ClearITPendingBit(USART3, USART_IT_IDLE);
		DMA_Cmd(DMA1_Stream1,ENABLE);	
	}
	
} 

void RS485_Send_Data(u8 *buf,u8 len)
{
	u8 t;
//	RS485_TX_EN=1;			//设置为发送模式
  	for(t=0;t<len;t++)		//循环发送数据
	{
	  while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //等待发送结束		
    USART_SendData(USART3,buf[t]); //发送数据
	}	 
	RS485_RX_CNT=0;	  
//	RS485_TX_EN=0;				//设置为接收模式	
}



//DMAx的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMA通道选择,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
//par:外设地址
//mar:存储器地址
//ndtr:数据传输量  
void MYDMA_Config(DMA_Stream_TypeDef *DMA_Streamx,u32 chx,u32 par,u32 mar,u16 ndtr)
{ 
 
	DMA_InitTypeDef  DMA_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	if((u32)DMA_Streamx>(u32)DMA2)//得到当前stream是属于DMA2还是DMA1
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2时钟使能 
		
	}else 
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA1时钟使能 
	}
  DMA_DeInit(DMA_Streamx);
	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}//等待DMA可配置 

	/*发送dma NVIC配置*/
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream3_IRQn;                   //发送DMA   Stream3 CH4 USART3_TX
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
		
	/*接收DMA配置*/
	DMA_DeInit(DMA1_Stream1);
	DMA_InitStructure.DMA_Channel= DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART3->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)RxBuff;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = RX_BUF_SIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_Mode_Normal;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream1,&DMA_InitStructure);
	DMA_Cmd(DMA1_Stream1,ENABLE);

		
  /* 配置 DMA Stream */
  DMA_InitStructure.DMA_Channel = chx;  //通道选择
  DMA_InitStructure.DMA_PeripheralBaseAddr = par;//DMA外设地址
  DMA_InitStructure.DMA_Memory0BaseAddr = mar;//DMA 存储器0地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//存储器到外设模式
  DMA_InitStructure.DMA_BufferSize = ndtr;//数据传输量 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度:8位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//存储器数据长度:8位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// 使用普通模式 
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//中等优先级
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
  DMA_Init(DMA_Streamx, &DMA_InitStructure);//初始化DMA Stream
	DMA_ITConfig(DMA_Streamx,DMA_IT_TC,ENABLE);

} 

//开启一次DMA传输
//DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7 
//ndtr:数据传输量  
void MYDMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr)
{
 
	DMA_Cmd(DMA_Streamx, DISABLE);                      //关闭DMA传输 
	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}	//确保DMA可以被设置  
		
	DMA_SetCurrDataCounter(DMA_Streamx,ndtr);          //数据传输量 
		
	DMA_Cmd(DMA_Streamx, ENABLE);                      //开启DMA传输 
	

}	  
int pro=0;

 void DMA1_Stream3_IRQHandler(void)
 {
		if(DMA_GetFlagStatus(DMA1_Stream3,DMA_FLAG_TCIF3)!=RESET)//等待DMA1_Steam3传输完成
		{ 
			DMA_ClearFlag(DMA1_Stream3,DMA_FLAG_TCIF3);//清除DMA1_Steam3传输完成标志
//			LED1=!LED1;//DS1翻转
			pro=DMA_GetCurrDataCounter(DMA1_Stream3);		
			DMA_ClearFlag(DMA1_Stream3, DMA_FLAG_TCIF3);
			DMA_ClearITPendingBit(DMA1_Stream3, DMA_IT_TCIF3);
			DelayUs(20);
			RS485_RX_EN;				//设置为接收模式
		}
	 
 }


