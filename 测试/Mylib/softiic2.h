#ifndef _SOFT_I2C2_H__
#define _SOFT_I2C2_H__

#include "board.h"

#define SCL2_H         I2C2_PORT->BSRRL = I2C2_Pin_SCL
#define SCL2_L         I2C2_PORT->BSRRH = I2C2_Pin_SCL
#define SDA2_H         I2C2_PORT->BSRRL = I2C2_Pin_SDA
#define SDA2_L         I2C2_PORT->BSRRH = I2C2_Pin_SDA
#define SDA2_read      I2C2_PORT->IDR  & I2C2_Pin_SDA

void I2C2_Soft_Init(void);
void I2C2_Soft_Delay(void);
int I2C2_Soft_Start(void);
void I2C2_Soft_Stop(void);
void I2C2_Soft_Ack(void);
void I2C2_Soft_NoAck(void);
int I2C2_Soft_WaitAck(void); 	 //·µ»Ø:=1ÓÐACK,=0ÎÞACK
void I2C2_Soft_SendByte(u8 SendByte);
u8 I2C2_Soft_ReadByte(void);

int I2C2_Soft_Single_Write(u8 SlaveAddress,u8 REG_Address,u8 REG_data);
int I2C2_Soft_Single_Read(u8 SlaveAddress,u8 REG_Address);
int I2C2_Soft_Mult_Read(u8 SlaveAddress,u8 REG_Address,u8 * ptChar,u8 size);

extern u8 I2C2_FastMode;

#endif
