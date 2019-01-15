#ifndef _SPI1_H_
#define _SPI1_H_

#include "main.h"

void SPI1_Init(void);
long int SPI1_ReadWriteByte(long int TxData);
void SPI1_Function(int OutputBuff[],int16_t InputBuff[], unsigned int NoOfBytes);
#endif
