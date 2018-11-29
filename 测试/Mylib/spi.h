#ifndef __SPI_H
#define __SPI_H
#include "board.h"

 	    													  
void SPI2_Init(void);			 //初始化SPI1口
void SPI2_SetSpeed(u8 SpeedSet); //设置SPI1速度   
long int SPI2_ReadWriteByte(long int TxData);//SPI1总线读写一个字节
void Spi2_Function(int OutputBuff[],int16_t InputBuff[], unsigned int NoOfBytes);
		 
#endif
