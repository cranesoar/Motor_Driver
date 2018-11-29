#define __DRIVER_GLOBALS
#include "judgement.h"
receive_judge_t judge_rece_mesg;

void Judge_Uart5_Config(void)
{
	  USART_InitTypeDef USART_InitStructure;
	  GPIO_InitTypeDef  GPIO_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;
    DMA_InitTypeDef   DMA_InitStructure;
	
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_DMA1,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
		
		GPIO_PinAFConfig(GPIOC,GPIO_PinSource12 ,GPIO_AF_UART5);
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource2 ,GPIO_AF_UART5);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOC,&GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOD,&GPIO_InitStructure);
		
		USART_DeInit(UART5);
		USART_InitStructure.USART_BaudRate = 115200;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_Even;
		USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_Init(UART5,&USART_InitStructure);			
		USART_Cmd(UART5,ENABLE);
				
    USART_DMACmd(UART5,USART_DMAReq_Rx,ENABLE);
					
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
 
    DMA_DeInit(DMA1_Stream0);
    DMA_InitStructure.DMA_Channel= DMA_Channel_4;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(UART5->DR);
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)JudgeDataBuffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize = JudgeBufferLength;
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
    DMA_Init(DMA1_Stream0,&DMA_InitStructure);

    DMA_ITConfig(DMA1_Stream0,DMA_IT_TC,ENABLE);
    DMA_Cmd(DMA1_Stream0,ENABLE);
		
}

/**
  * @brief  DMA传输完成中断，用于计算队列入队指针
  * @input  void
  * @output void
  * @note   
  */
void DMA1_Stream0_IRQHandler(void)
{
	
	if(DMA_GetITStatus(DMA1_Stream0, DMA_IT_TCIF0))
	{
		full_count++;

		DMA_ClearFlag(DMA1_Stream0, DMA_FLAG_TCIF0);
		DMA_ClearITPendingBit(DMA1_Stream0, DMA_IT_TCIF0);
		
		
  }
}

/**
  * @brief  解析数据包
  * @input  独立完整的一帧数据，起始地址
  * @output 0或1
  * @note   
  */
uint8_t get_judge_data(uint8_t *buff,uint16_t sof_arr)
{
	if(buff[sof_arr] == 0xA5)
	{
		judge_rece_mesg.judge_frame_header.data_length = (int16_t)(buff[2 + sof_arr]<<8 | buff[1 + sof_arr]);
		judge_rece_mesg.judge_frame_header.seq         = buff[3 + sof_arr];
		judge_rece_mesg.judge_frame_header.crc8        = buff[4 + sof_arr];
		judge_rece_mesg.judge_rxcmd_id                 = (int16_t)(buff[6 + sof_arr]<<8 | buff[5 + sof_arr]);	
		
		
		switch(judge_rece_mesg.judge_rxcmd_id)
		{
			case GAME_INFO_ID:
				judge_rece_mesg.game_information.stageRemianTime = (int16_t)(buff[1 + 7 + sof_arr]<<8 | buff[0 + 7 + sof_arr]);	
				judge_rece_mesg.game_information.gameProgress    = buff[2 + 7 + sof_arr];
				judge_rece_mesg.game_information.robotLevel      = buff[3 + 7 + sof_arr];
				judge_rece_mesg.game_information.remainHP        = (int16_t)(buff[5 + 7 + sof_arr]<<8 | buff[4 + 7 + sof_arr]);	
				judge_rece_mesg.game_information.maxHP           = (int16_t)(buff[7 + 7 + sof_arr]<<8 | buff[6 + 7 + sof_arr]);	
				judge_rece_mesg.crc16 = (int16_t)(buff[16 + sof_arr]<<8 | buff[15 + sof_arr]);	
				break;
			case REAL_HURT_DATA_ID:
				judge_rece_mesg.real_hurt_data.armorType = (uint8_t)(buff[7 + sof_arr] & 0x0F);
				judge_rece_mesg.real_hurt_data.hurtType  = (uint8_t)(buff[7 + sof_arr] >> 4);
				judge_rece_mesg.crc16 = (int16_t)(buff[2 + 7 + sof_arr]<<8 | buff[1 + 7 + sof_arr]);	

				break;
			case REAL_SHOOT_DATA_ID:
				judge_rece_mesg.real_shoot_data.bulletType 	= buff[7 + sof_arr];
				judge_rece_mesg.real_shoot_data.bulletFreq  = buff[1 + 7 + sof_arr];
				judge_rece_mesg.real_shoot_data.bulletSpeed = u8toflaot(buff[5 + 7 + sof_arr],buff[4 + 7 + sof_arr],buff[3 + 7 + sof_arr],buff[2 + 7 + sof_arr]);
				judge_rece_mesg.crc16 = (int16_t)(buff[14 + sof_arr]<<8 | buff[13 + sof_arr]);	
			
				break;
			case REAL_POWER_HEAT_DATA_ID:
				judge_rece_mesg.real_powerheat_data.chassisVolt        = u8toflaot(buff[3 + 0 + 7 + sof_arr],buff[2 + 0 + 7 + sof_arr],buff[1 + 0 + 7 + sof_arr],buff[0 + 0 + 7 + sof_arr]);
				judge_rece_mesg.real_powerheat_data.chassisCurrent     = u8toflaot(buff[3 + 4 + 7 + sof_arr],buff[2 + 4 + 7 + sof_arr],buff[1 + 4 + 7 + sof_arr],buff[0 + 4 + 7 + sof_arr]);
				judge_rece_mesg.real_powerheat_data.chassisPower   		 = u8toflaot(buff[3 + 8 + 7 + sof_arr],buff[2 + 8 + 7 + sof_arr],buff[1 + 8 + 7 + sof_arr],buff[0 + 8 + 7 + sof_arr]);
				judge_rece_mesg.real_powerheat_data.chassisPowerBuffer = u8toflaot(buff[3 + 12 + 7 + sof_arr],buff[2 + 12 + 7 + sof_arr],buff[1 + 12 + 7 + sof_arr],buff[0 + 12 + 7 + sof_arr]);
				judge_rece_mesg.real_powerheat_data.shooterHeat0       = (int16_t)(buff[1 + 16 + 7 + sof_arr]<<8 | buff[0 + 16 + 7 + sof_arr]);	
				judge_rece_mesg.real_powerheat_data.shooterHeat1       = (int16_t)(buff[1 + 18 + 7 + sof_arr]<<8 | buff[0 + 18 + 7 + sof_arr]);	
				judge_rece_mesg.crc16 = (int16_t)(buff[28 + sof_arr]<<8 | buff[27 + sof_arr]);	
			
				break;
			case GAME_RFID_DATA_ID:
				judge_rece_mesg.rfid_data.cardType = buff[7 + sof_arr];
				judge_rece_mesg.rfid_data.cardIdx  = buff[8 + sof_arr];
				judge_rece_mesg.crc16 = (int16_t)(buff[10 + sof_arr]<<8 | buff[9 + sof_arr]);	
				break;
			case GAME_RESULT_ID:
				judge_rece_mesg.game_result_data.winner = buff[7 + sof_arr];
				judge_rece_mesg.crc16 = (int16_t)(buff[9 + sof_arr]<<8 | buff[8 + sof_arr]);	
				break;
			case GAIN_BUFF_ID:
				judge_rece_mesg.get_buff_data.buffType     = buff[7 + sof_arr];
				judge_rece_mesg.get_buff_data.buffAddition = buff[8 + sof_arr];
				judge_rece_mesg.crc16 = (int16_t)(buff[10 + sof_arr]<<8 | buff[9 + sof_arr]);	
				break;
			case REAL_POSITION_ID:
				judge_rece_mesg.real_robot_pos.xx  = u8toflaot(buff[3 + 0 + 7 + sof_arr],buff[2 + 0 + 7 + sof_arr],buff[1 + 0 + 7 + sof_arr],buff[0 + 0 + 7 + sof_arr]);
				judge_rece_mesg.real_robot_pos.yy  = u8toflaot(buff[3 + 4 + 7 + sof_arr],buff[2 + 4 + 7 + sof_arr],buff[1 + 4 + 7 + sof_arr],buff[0 + 4 + 7 + sof_arr]);
				judge_rece_mesg.real_robot_pos.zz  = u8toflaot(buff[3 + 8 + 7 + sof_arr],buff[2 + 8 + 7 + sof_arr],buff[1 + 8 + 7 + sof_arr],buff[0 + 8 + 7 + sof_arr]);
				judge_rece_mesg.real_robot_pos.yaw = u8toflaot(buff[3 + 12 + 7 + sof_arr],buff[2 + 12 + 7 + sof_arr],buff[1 + 12 + 7 + sof_arr],buff[0 + 12 + 7 + sof_arr]);
				judge_rece_mesg.crc16 = (int16_t)(buff[24 + sof_arr]<<8 | buff[23 + sof_arr]);	
				break;
			default:break;			
		}
		return 1;
	}else
	return 0;
}


/**
  * @brief  变量转换4个U8合成1个float
  * @input  u8*4
  * @output float
  * @note   
  */
float u8toflaot(uint8_t u4,uint8_t u3,uint8_t u2,uint8_t u1)
{
	FormatTrans FT;
	FT.U[3] = u4;
	FT.U[2] = u3;
	FT.U[1] = u2;
	FT.U[0] = u1;
	return FT.F;
}



/**
  * @brief  变量转换4个U8合成1个float
  * @input  u8*4
  * @output float
  * @note   
  */
_vec4u floattou8(float f1)
{
	FormatTrans FT;
	_vec4u u;
	FT.F = f1;
	u.data[0] = FT.U[0];
	u.data[1] = FT.U[1];
	u.data[2] = FT.U[2];
	u.data[3] = FT.U[3];
	return u;
}


/**
  * @brief  获取数据包长度
  * @input  CMD_ID
  * @output length
  * @note   
  */
uint8_t get_length(u8 cmdid)
{
	u8 len;
		switch(cmdid)
		{
			case GAME_INFO_ID:
				len = 17;
				break;
			case REAL_HURT_DATA_ID:
				len = 10;
				break;
			case REAL_SHOOT_DATA_ID:
				len = 15;
				break;
			case REAL_POWER_HEAT_DATA_ID:
				len = 29;
				break;
			case GAME_RFID_DATA_ID:
				len = 11;
				break;
			case GAME_RESULT_ID:
				len = 11;
				break;
			case GAIN_BUFF_ID:
				len = 12;
				break;
			case REAL_POSITION_ID:
				len = 25;
				break;
			default:break;			
		}	
		return len;
}
/**
  * @brief  裁判系统数据解析
  * @input  void
  * @output void
  * @note   将JudgeDataBuffer当作环形队列，从中分拣出每个数据包放到dji_temp中，解析每一帧
  * @note   在while(1)中调用，while(1)中不能用延时函数
  */

int judge_Ring_queue(void)
{
	static int arrtemp = 0,rx_arr = 0;
	static long int err,rx_sum = 0,read_arr = 0,read_arr_sum = 0;
	static uint8_t dji_temp[80];
	u8 cmdid,err_flag = 1;
	rx_arr = JudgeBufferLength - DMA_GetCurrDataCounter(DMA1_Stream0);
	rx_sum = full_count * 200 + rx_arr;
	err = rx_sum - read_arr_sum;
	if(err<5)                               //确保队列不为空
	{
		return ERROR_BUFFER_EMPTY;            //队列长度小于5，直接返回，直到缓冲区有一定数据量再继续解析
	}
	read_arr = read_arr_sum % 200;
	if(JudgeDataBuffer[read_arr] == 0xA5)
	{
		cmdid= (int16_t)(dji_temp[6]<<8 | dji_temp[5]);	
		if(Verify_CRC16_Check_Sum(dji_temp,get_length(cmdid)) == 1)
			get_judge_data(dji_temp,0);
		else
			err_flag = ERROR_CRC16_WRONG;
		arrtemp = 0;
		dji_temp[arrtemp] = JudgeDataBuffer[read_arr];
	}
	read_arr_sum++;
	read_arr = read_arr_sum % 200;
	arrtemp++;
	dji_temp[arrtemp] = JudgeDataBuffer[read_arr];
	
	return err_flag;
	
}


void Uart5_SendChar(unsigned char b)
{
   while (USART_GetFlagStatus(UART5,USART_FLAG_TC) == RESET);
	 USART_SendData(UART5,b);
}


/**
  * @brief  向裁判系统发送数据
  * @param  infor---发送的数据
  * @retval void
  */
void Judge_sendbuffer(float data1,float data2,float data3,uint8_t mask) //4*3+9=21
{
	int i=0;
	_vec4u u;
	unsigned char buffer[20];
	buffer[0] = 0XA5; buffer[1] = 0X15; buffer[2] = 0X00; buffer[3] = 0X00; buffer[4] = 0X00;
	Append_CRC8_Check_Sum(buffer,5);
  buffer[5] = 0X00; buffer[6] = 0X01;
	u = floattou8(data1);
	buffer[6] = u.data[0];
	buffer[7] = u.data[1];
	buffer[8] = u.data[2]; 
	buffer[9] = u.data[3]; 
	u = floattou8(data2);
	buffer[10] = u.data[0]; 
	buffer[11] = u.data[1]; 
	buffer[12] = u.data[2]; 
	buffer[13] = u.data[3]; 
	u = floattou8(data3);
	buffer[14] = u.data[0];
  buffer[15] = u.data[1]; 
	buffer[16] = u.data[2]; 
	buffer[17] = u.data[3];	
	buffer[18] = mask;
  Append_CRC16_Check_Sum(buffer,21);		
	while(i<21)
	{
		Uart5_SendChar(buffer[i]);
		i++;
	}	
}

