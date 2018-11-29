#ifndef __OLED_I2C_H__
#define	__OLED_I2C_H__

#include "stm32f4xx.h"
#include "board.h"

#define OLED_ADDRESS	0x78 //通过调整0R电阻,屏可以0x78和0x7A两个地址 -- 默认0x78

void I2C_Configuration(void);
void I2C_WriteByte(uint8_t addr,uint8_t data);
void WriteCmd(unsigned char I2C_Command);
void WriteDat(unsigned char I2C_Data);
void OLED_Init(void);
void OLED_SetPos(unsigned char ax, unsigned char ay);
void OLED_Fill(unsigned char fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowStr(unsigned char ax, unsigned char ay, unsigned char ch[], unsigned char TextSize);
void OLED_ShowCN(unsigned char ax, unsigned char ay, unsigned char N);
void OLED_DrawBMP(unsigned char ax0,unsigned char ay0,unsigned char ax1,unsigned char ay1,unsigned char BMP[]);
void OLED_value(_Bool oled_cls ,float value,unsigned char x0 ,unsigned char y0, unsigned char textsize);
#endif
