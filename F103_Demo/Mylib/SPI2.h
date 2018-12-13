#ifndef _SPI2_H_
#define _SPI2_H_

#include "main.h"

void SPI2_Init(void);
long int SPI2_ReadWriteByte(long int TxData);
void SPI2_Function(int OutputBuff[],int16_t InputBuff[], unsigned int NoOfBytes);

#endif
