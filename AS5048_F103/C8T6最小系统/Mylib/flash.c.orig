#include "main.h"

void Flash_Write_Word(uint32_t Address,uint32_t Data)
{
	FLASH_Unlock();
	FLASH_ProgramWord(Address, (uint32_t)Data);
	FLASH_Lock();
}

void Flash_Write_HalfWold(uint32_t Address,uint16_t Data)
{
	FLASH_Unlock();
	FLASH_ProgramHalfWord(Address, (uint16_t)Data);
	FLASH_Lock();
}

void Flash_Erase_Page(uint32_t Page_Address)
{
	FLASH_Unlock();
	FLASH_ErasePage(Page_Address);
	FLASH_Lock();
}

uint32_t Flash_Read_Word(uint32_t Address)
{
	return *((uint32_t*)Address);
}
