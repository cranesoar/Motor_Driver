#include "Camera.h"

Position Position_box;

unsigned char camera_rx_buffer[2];
void Usart_Camera_Init(void)
  {
		USART_InitTypeDef USART_InitStructure;
	  GPIO_InitTypeDef  GPIO_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;
    DMA_InitTypeDef     DMA_InitStructure;
	
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_DMA1 ,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5 ,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);	
	
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource2,GPIO_AF_UART5);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 ;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOD,&GPIO_InitStructure);
    
		//USART_DeInit(UART5);
		USART_InitStructure.USART_BaudRate = 19200;   //SBUS 100K baudrate
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No ;
		USART_InitStructure.USART_Mode = USART_Mode_Rx;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		 
		USART_Init(UART5,&USART_InitStructure);
    USART_DMACmd(UART5,USART_DMAReq_Rx,ENABLE);
		USART_ITConfig(UART5, USART_IT_IDLE, ENABLE);
    
    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    DMA_DeInit(DMA1_Stream0);
    DMA_InitStructure.DMA_Channel= DMA_Channel_4;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(UART5->DR);
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)camera_rx_buffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize = 256;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA1_Stream0,&DMA_InitStructure);

    //DMA_ITConfig(DMA1_Stream0,DMA_IT_TC,ENABLE);
    DMA_Cmd(DMA1_Stream0,ENABLE);	
		USART_Cmd(UART5,ENABLE);
		
  }
	
	
	void UART5_IRQHandler(void)  
{
	uint8_t num;
	uint16_t data_len;
	data_len = 256 - DMA_GetCurrDataCounter(DMA1_Stream0);
  num = UART5->SR;
  num = UART5->DR;
	DMA_Cmd(DMA1_Stream0, DISABLE);   
  DMA_ClearFlag(DMA1_Stream0,DMA_FLAG_TEIF0 | DMA_FLAG_TCIF0 | DMA_FLAG_DMEIF0 | DMA_FLAG_HTIF0 | DMA_FLAG_FEIF0);
  
	Position_box.xx=camera_rx_buffer[0];
	Position_box.yy=camera_rx_buffer[1];
	
	DMA_SetCurrDataCounter(DMA1_Stream0,256);
  USART_ClearITPendingBit(UART5,USART_IT_IDLE);
  DMA_Cmd(DMA1_Stream0, ENABLE);
}