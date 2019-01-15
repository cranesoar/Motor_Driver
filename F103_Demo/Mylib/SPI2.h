#ifndef _SPI2_H_
#define _SPI2_H_

#include "main.h"

void SPI2_Init(void);
uint16_t SPI2_ReadWrite16Bit(uint16_t Txdata);
void SPI2_Function(int OutputBuff[],int16_t InputBuff[], unsigned int NoOfBytes);

#endif
