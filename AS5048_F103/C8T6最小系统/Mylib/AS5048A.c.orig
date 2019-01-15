#include "AS5048A.h"

//#define AS5048A_DEBUG

const int AS5048A_CLEAR_ERROR_FLAG              = 0x0001;
const int AS5048A_PROGRAMMING_CONTROL           = 0x0003;
const int AS5048A_OTP_REGISTER_ZERO_POS_HIGH    = 0x0016;
const int AS5048A_OTP_REGISTER_ZERO_POS_LOW     = 0x0017;
const int AS5048A_DIAG_AGC                      = 0x3FFD;
const int AS5048A_MAGNITUDE                     = 0x3FFE;
const int AS5048A_ANGLE                         = 0x3FFF;

// 定义好附加偶校验位的各个寄存器读取指令
#define CMD_ANGLE            0xffff
#define CMD_AGC              0x7ffd
#define CMD_MAG              0x7ffe
#define CMD_CLAER            0x4001
#define CMD_NOP              0xc000

#define AS5048A_CS_Port GPIOC
#define AS5048A_CS_Pin GPIO_Pin_13

uint8_t errorFlag = 0;
uint16_t position = 0;

//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI2_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		retry++;
		if(retry>200)return 0;
	}			  
	SPI_I2S_SendData(SPI2, TxData); //通过外设SPIx发送一个数据
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	return SPI_I2S_ReceiveData(SPI2); //返回通过SPIx最近接收的数据					    
}

uint16_t SPI2_RW16bit(uint16_t TxData)
{
	uint8_t retry = 0;
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
	{
		retry++;
		if(retry>200)return 0;
	}			  
	SPI_I2S_SendData(SPI2, TxData); //通过外设SPIx发送一个数据
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	return SPI_I2S_ReceiveData(SPI2);
}

void SPI2_Init_16bit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTB时钟使能 
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );//SPI2时钟使能 	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB

	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //PB13/14/15上拉

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 15;	//CRC值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

	SPI_Cmd(SPI2, ENABLE); //使能SPI外设

	SPI2_RW16bit(0xFFFF);//启动传输
}

/* PB13,14,15 SPI2 */
/* SPI CLK div 2, 36MHz->18MHz */
void SPI2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTB时钟使能 
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );//SPI2时钟使能 	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB

	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //PB13/14/15上拉

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

	SPI_Cmd(SPI2, ENABLE); //使能SPI外设

	SPI2_ReadWriteByte(0xff);//启动传输
}   
//SPI 速度设置函数
//SpeedSet:
//SPI_BaudRatePrescaler_2
//SPI_BaudRatePrescaler_8
//SPI_BaudRatePrescaler_16
//SPI_BaudRatePrescaler_256
  
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI2->CR1&=0XFFC7;
	SPI2->CR1|=SPI_BaudRatePrescaler;	//设置SPI2速度 
	SPI_Cmd(SPI2,ENABLE); 
}

void spiCSInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
}

void AS5048A_Init(void)
{
	// SPI2_Init();
	// SPI2_SetSpeed(SPI_BaudRatePrescaler_16);
	SPI2_Init_16bit();
	spiCSInit();
}

uint8_t AS5048A_spiCalcEvenParity(uint16_t value)
{
	uint8_t cnt = 0;
	uint8_t i;

	for (i = 0; i < 16; i++)
	{
		if (value & 0x1)
		{
			cnt++;
		}
		value >>= 1;
	}
	return cnt & 0x1;
}

uint16_t AS5048A_read_always()
{
	// 这里其实每次读取的是上一次写命令（0xFF）时的返回数据。
	uint16_t cnt=0;
	uint8_t right_byte;
	uint8_t left_byte;
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	cnt = 50;while(cnt--);
	left_byte = SPI2_ReadWriteByte(0xFF);
	right_byte = SPI2_ReadWriteByte(0xFF);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);

	return (( ( left_byte & 0xFF ) << 8 ) | ( right_byte & 0xFF )) & ~0xC000;
}

uint16_t AS5048A_Read_Angle_once()
{
	// read angle once, need two 0xFF commands...
	// write a nop command 0x00 may cause error, so I use these:
	uint16_t cnt=0;
	uint8_t right_byte;
	uint8_t left_byte;
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	cnt = 50;while(cnt--);
	left_byte = SPI2_ReadWriteByte(0xFF);
	right_byte = SPI2_ReadWriteByte(0xFF);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	cnt = 50;while(cnt--);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	cnt = 50;while(cnt--);
	left_byte = SPI2_ReadWriteByte(0xFF);
	right_byte = SPI2_ReadWriteByte(0xFF);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);

	return (( ( left_byte & 0xFF ) << 8 ) | ( right_byte & 0xFF )) & ~0xC000;
}

uint16_t AS5048A_Read_Angle_once_16bit()
{
	uint16_t cnt=0;
	uint16_t data=0;
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	cnt = 50;while(cnt--);
	data = SPI2_RW16bit(0xFFFF);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	cnt = 50;while(cnt--);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	cnt = 50;while(cnt--);
	data = SPI2_RW16bit(0xFFFF);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);

	return data & ~0xC000;
}

/*
 * Read a register from the sensor
 * Takes the address of the register as a 16 bit uint16_t
 * Returns the value of the register
 */
uint16_t AS5048A_read(uint16_t registerAddress)
{
	uint16_t cnt = 50;
	uint8_t right_byte;
	uint8_t left_byte;
	uint16_t command = 0x4000; // PAR=0 R/W=R
	command = command | registerAddress;

	//Add a parity bit on the the MSB
	command |= ((uint16_t)AS5048A_spiCalcEvenParity(command)<<15);

	//Split the command into two bytes
	right_byte = command & 0xFF;
	left_byte = ( command >> 8 ) & 0xFF;

	//SPI - begin transaction
	// SPI.beginTransaction(settings);

	//Send the command
	while(cnt--);
	cnt = 50;
	GPIO_ResetBits(AS5048A_CS_Port, AS5048A_CS_Pin);
	while(cnt--);
	cnt = 50;
	SPI2_ReadWriteByte(left_byte);
	SPI2_ReadWriteByte(right_byte);
	GPIO_SetBits(AS5048A_CS_Port, AS5048A_CS_Pin);

	while(cnt--);
	cnt = 50;
	//Now read the response
	GPIO_ResetBits(AS5048A_CS_Port, AS5048A_CS_Pin);
	left_byte = SPI2_ReadWriteByte(0x00);
	right_byte = SPI2_ReadWriteByte(0x00);
	GPIO_SetBits(AS5048A_CS_Port, AS5048A_CS_Pin);
	while(cnt--);
	cnt = 50;
	//SPI - end transaction
	// SPI.endTransaction();

	//Check if the error bit is set
	if (left_byte & 0x40) {
		errorFlag = 1;
		printf("Read Error Error:\n");
		printf("%d\n", right_byte);
	}
	else {
		errorFlag = 0;
		printf("Read Error OK:\n");
		printf("%d\n", right_byte);
	}

	//Return the data, stripping the parity and error bits
	return (( ( left_byte & 0xFF ) << 8 ) | ( right_byte & 0xFF )) & ~0xC000;
}

/**
 * Returns the raw angle directly from the sensor
 */
uint16_t AS5048A_getRawRotation(void)
{
	return AS5048A_read(AS5048A_ANGLE);
}
 
uint16_t AS5048A_getRotation()
{
	uint16_t data;
	uint16_t rotation;

	data = AS5048A_getRawRotation();
	rotation = (uint16_t)data - (uint16_t)position;
	if(rotation > 8191) rotation = -((0x3FFF)-rotation); //more than -180
	//if(rotation < -0x1FFF) rotation = rotation+0x3FFF;

	return rotation;
}

/*
 * Get and clear the error register by reading it
 */
uint16_t AS5048A_getErrors(){
	return AS5048A_read(AS5048A_CLEAR_ERROR_FLAG);
}


/*
 * Write to a register
 * Takes the 16-bit  address of the target register and the 16 bit uint16_t of data
 * to be written to that register
 * Returns the value of the register after the write has been performed. This
 * is read back from the sensor to ensure a sucessful write.
 */
uint16_t AS5048A_write_test(uint16_t registerAddress, uint16_t data)
{
	uint8_t right_byte;
	uint8_t left_byte;
	uint16_t command = 0x4000; // PAR=0 R/W=W
	uint16_t dataToSend = 0x0000;

	command |= registerAddress;

	//Add a parity bit on the the MSB
	command |= ((uint16_t)AS5048A_spiCalcEvenParity(command)<<15);

	//Split the command into two bytes
	right_byte = command & 0xFF;
	left_byte = ( command >> 8 ) & 0xFF;

	// //Start the write command with the target address
	GPIO_ResetBits(AS5048A_CS_Port, AS5048A_CS_Pin);
	SPI2_ReadWriteByte(left_byte);
	SPI2_ReadWriteByte(right_byte);
	GPIO_SetBits(AS5048A_CS_Port, AS5048A_CS_Pin);
	
	dataToSend |= data;

	//Craft another packet including the data and parity
	dataToSend |= ((uint16_t)AS5048A_spiCalcEvenParity(dataToSend)<<15);
	right_byte = dataToSend & 0xFF;
	left_byte = ( dataToSend >> 8 ) & 0xFF;

	// //Now send the data packet
	GPIO_ResetBits(AS5048A_CS_Port, AS5048A_CS_Pin);
	SPI2_ReadWriteByte(left_byte);
	SPI2_ReadWriteByte(right_byte);
	GPIO_SetBits(AS5048A_CS_Port, AS5048A_CS_Pin);
	
	// //Send a NOP to get the new data in the register
	GPIO_ResetBits(AS5048A_CS_Port, AS5048A_CS_Pin);
	left_byte = SPI2_ReadWriteByte(0x00);
	right_byte = SPI2_ReadWriteByte(0x00);
	GPIO_SetBits(AS5048A_CS_Port, AS5048A_CS_Pin);

	//Return the data, stripping the parity and error bits
	return (( ( left_byte & 0xFF ) << 8 ) | ( right_byte & 0xFF )) & ~0xC000;
}

void AS5048A_clear_error()
{
	uint16_t cnt = 20;

	GPIO_ResetBits(AS5048A_CS_Port, AS5048A_CS_Pin);
	SPI2_ReadWriteByte(0x40);
	SPI2_ReadWriteByte(0x01);
	GPIO_SetBits(AS5048A_CS_Port, AS5048A_CS_Pin);

	while(cnt--);

	GPIO_ResetBits(AS5048A_CS_Port, AS5048A_CS_Pin);
	SPI2_ReadWriteByte(0xC0);
	SPI2_ReadWriteByte(0x00);
	GPIO_SetBits(AS5048A_CS_Port, AS5048A_CS_Pin);
}

// /**
//  * returns the value of the state register
//  * @return 16 bit uint16_t containing flags
//  */
// uint16_t AS5048A_getState()
// {
// 	return AS5048A_read(AS5048A_DIAG_AGC);
// }

// // Arduino Ser Com, need fix!!!
// /**
//  * Print the diagnostic register of the sensor
//  */
// void AS5048A_printState(){
// 	uint16_t data;

// 	data = AS5048A_getState();
// 	if(AS5048A_error()){
// 		Serial.print("Error bit was set!");
// 	}
// 	Serial.println(data, BIN);
// }

// /**
//  * Returns the value used for Automatic Gain Control (Part of diagnostic
//  * register)
//  */
// byte AS5048A_getGain(){
// 	uint16_t data = AS5048A_getState();
// 	return (byte) data & 0xFF;
// }


// /*
//  * Set the zero position
//  */
// void AS5048A_setZeroPosition(uint16_t arg_position){
// 	position = arg_position % 0x3FFF;
// }

// /*
//  * Returns the current zero position
//  */
// uint16_t AS5048A_getZeroPosition(){
// 	return position;
// }

// /*
//  * Check if an error has been encountered.
//  */
// bool AS5048A_error(){
// 	return errorFlag;
// }

