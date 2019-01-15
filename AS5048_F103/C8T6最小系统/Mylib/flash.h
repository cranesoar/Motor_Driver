#ifndef __FLASH_H__
#define __FLASH_H__
#include "stm32f10x.h"

void Flash_Write_Word(uint32_t Address,uint32_t Data);
void Flash_Write_HalfWold(uint32_t Address,uint16_t Data);
void Flash_Erase_Page(uint32_t Page_Address);
uint32_t Flash_Read_Word(uint32_t Address);

#endif
