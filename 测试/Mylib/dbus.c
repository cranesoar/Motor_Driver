#define __DBUS_DRIVER_GLOBALS
#include "dbus.h"

unsigned char sbus_rx_buffer[18];
int16_t sbus_decode_buffer[15];
int16_t keyBoard_jumpkey_code;
int64_t Position_set;
int Dbus_Updata_flag=0;


void Dbus_Config()
{
	  USART_InitTypeDef USART_InitStructure;
	  GPIO_InitTypeDef  GPIO_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;
    DMA_InitTypeDef     DMA_InitStructure;
	
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_DMA1 ,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 ,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);	
	
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource3 ,GPIO_AF_USART2);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 ;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOA,&GPIO_InitStructure);
    
		//USART_DeInit(USART2);
		USART_InitStructure.USART_BaudRate = 100000;   //SBUS 100K baudrate
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No ;
		USART_InitStructure.USART_Mode = USART_Mode_Rx;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		
		USART_Init(USART2,&USART_InitStructure);
    USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);
		USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
    
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    DMA_DeInit(DMA1_Stream5);
    DMA_InitStructure.DMA_Channel= DMA_Channel_4;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)sbus_rx_buffer;
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
    DMA_Init(DMA1_Stream5,&DMA_InitStructure);

    //DMA_ITConfig(DMA1_Stream5,DMA_IT_TC,ENABLE);
    DMA_Cmd(DMA1_Stream5,ENABLE);	
		USART_Cmd(USART2,ENABLE);
}

void USART2_IRQHandler(void)  
{
	uint8_t num;
	uint16_t data_len;
	data_len = 256 - DMA_GetCurrDataCounter(DMA1_Stream5);
  num = USART2->SR;
  num = USART2->DR;
	DMA_Cmd(DMA1_Stream5, DISABLE);   
  DMA_ClearFlag(DMA1_Stream5,DMA_FLAG_TEIF5 | DMA_FLAG_TCIF5 | DMA_FLAG_DMEIF5 | DMA_FLAG_HTIF5 | DMA_FLAG_FEIF5);
	
	Handle_data();
	
	DMA_SetCurrDataCounter(DMA1_Stream5,256);
  USART_ClearITPendingBit(USART2,USART_IT_IDLE);
  DMA_Cmd(DMA1_Stream5, ENABLE);
	Dbus_Updata_flag=1;
}

void Handle_data(void)
{
    //the last mouse state
		RC.CH_LAST[6] = RC.CH[6];
		RC.CH_LAST[7] = RC.CH[7];
	  
	
	
		//”““°∏À∫·œÚ  ∑∂Œß+-660
		sbus_decode_buffer[0] = (sbus_rx_buffer[0]| (sbus_rx_buffer[1] << 8)) & 0x07ff; //!< Channel 0
		RC.CH[0] = my_deathzoom(sbus_decode_buffer[0]-1024, 5);
		//”““°∏À◊›œÚ   ∑∂Œß+-660
		sbus_decode_buffer[1] = ((sbus_rx_buffer[1] >> 3) | (sbus_rx_buffer[2] << 5)) & 0x07ff; //!< Channel 1
		RC.CH[1] = my_deathzoom(sbus_decode_buffer[1]-1024, 10);
		//◊Û“°∏À∫·œÚ   ∑∂Œß+-660
		sbus_decode_buffer[2]= ((sbus_rx_buffer[2] >> 6) | (sbus_rx_buffer[3] << 2) | (sbus_rx_buffer[4] << 10)) & 0x07ff; //!< Channel 2
		RC.CH[2] = my_deathzoom(sbus_decode_buffer[2]-1024, 5);
    if(ABS(RC.CH[2])<=23)RC.CH[2]=0;
		//◊Û“°∏À◊›œÚ   ∑∂Œß+-660
		sbus_decode_buffer[3] = ((sbus_rx_buffer[4] >> 1) | (sbus_rx_buffer[5] << 7)) & 0x07ff; //!< Channel 3
		RC.CH[3] = my_deathzoom(sbus_decode_buffer[3]-1024, 10);
    if(ABS(RC.CH[3])<=10)RC.CH[3]=0; 
		//◊Û±ﬂø™πÿ  132 …œ÷–œ¬
		RC.CH[4] = ((sbus_rx_buffer[5] >> 4)& 0x000C) >> 2; //!< Switch left
		//”“±ﬂø™πÿ  132 …œ÷–œ¬
		RC.CH[5] = ((sbus_rx_buffer[5] >> 4)& 0x0003); //!< Switch right9 / 9 
		/*** Û±ÍX÷µ***/
    RC.CH[6] = 	((sbus_rx_buffer[6]) | (sbus_rx_buffer[7] << 8));//x
		/*** Û±ÍY÷µ***/
		RC.CH[7] = 	-((sbus_rx_buffer[8]) | (sbus_rx_buffer[9] << 8));//y
		/*** Û±Í◊Ûº¸***/
		RC.CH[8] = sbus_rx_buffer[12];//◊Ûº¸
		RC.mouse.left_jump=RC.CH[8];
		/*** Û±Í”“º¸***/
		RC.CH[9] = sbus_rx_buffer[13];//”“º¸
    RC.mouse.Right_jump=RC.CH[9];		/***º¸≈Ã÷µ***/
		RC.key.value = sbus_rx_buffer[14] | (sbus_rx_buffer[15] << 8);
		
		if ((ABS(RC.CH[0]) > 660) || \
      (ABS(RC.CH[1]) > 660) || \
      (ABS(RC.CH[2]) > 660) || \
      (ABS(RC.CH[3]) > 660))
    {
    memset(&RC,0, sizeof(int16_t)*11);
    return ;
    }
		
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

// 		Position_set_zhua += RC.CH[1]*5 ;
//		Position_set_zhua= LIMIT(Position_set_zhua,0,153800);
	
//		Position_set += RC.CH[1]*5 ;
//		Position_set = LIMIT(Position_set,0,545000);

		
}

//∞¥º¸Ã¯±‰ºÏ≤‚
void DBUS_ButtonCheckJump(uint32_t CurrentTick)
{
	  uint8_t index;
    keyBoard_jumpkey_code=0;
   
    static uint32_t LastButtonJumpTick[15] = {0, 0, 0, 0, 0,
                                            0, 0, 0, 0, 0,
                                            0, 0, 0, 0, 0};
    
    for(index = 0; index < 15; index++)
    {
        if(CurrentTick - LastButtonJumpTick[index] >= 50)
        {
            if((RC.CH[10] & (1 << index)) && (!(RC.CH_LAST[10] & (1 << index))))
            {
                keyBoard_jumpkey_code |= 1 << index;
                LastButtonJumpTick[index] = CurrentTick;
            }
        }
        else
        {
            keyBoard_jumpkey_code &= ~(1 << index);
        }
    }
}

/**
  * @brief  Be sure if the key has pressed.
  * @param  Key_value
  * @retval True or False
  */
int Get_Keypress(uint16_t Key)
{
    if(RC.key.value & Key)
       return 1;
    else
			 return 0;
}


	