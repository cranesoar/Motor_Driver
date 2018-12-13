#include "SPI1.h"

void SPI1_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;  
   
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB|RCC_APB2Periph_SPI1, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE); 
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE); 
  
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_Init(GPIOB,&GPIO_InitStructure);
  
  GPIO_PinRemapConfig(GPIO_Remap_SPI1 , ENABLE);
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, ENABLE);
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, DISABLE);
  
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//串行同步时钟的空闲状态为低电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;		//定义波特率预分频的值:波特率预分频值为8
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI1, ENABLE); //使能SPI外设
  
}

//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
long int SPI1_ReadWriteByte(long int TxData)
{		 			 
 
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}//等待发送区空  
	
	SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个byte  数据
		
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){} //等待接收完一个byte  
 
	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据	
 		    
}

void SPI1_Function(int OutputBuff[],int16_t InputBuff[], unsigned int NoOfBytes)
{	
	 int i;
	
   for(i=0;i<(NoOfBytes);i++)
   {
		 InputBuff[i]=SPI1_ReadWriteByte( OutputBuff[i]);
   }
	 
}

