#ifndef __INPUT_H__
#define __INPUT_H__
#include "stm32f10x.h"

#define READ_B3    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)//读取B3电平 
#define READ_B4    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)//读取B4电平
#define READ_B5    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)//读取B5电平   
#define READ_B13    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)//读取B13电平 
#define READ_B14    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)//读取B14电平
#define READ_B15    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)//读取B15电平

#define OUT_ON_A4    GPIO_SetBits(GPIOA,GPIO_Pin_4);//A4输出高电平
#define OUT_OFF_A4   GPIO_ResetBits(GPIOA,GPIO_Pin_4);//A4输出低电平
#define OUT_ON_A5    GPIO_SetBits(GPIOA,GPIO_Pin_5);//A5输出高电平
#define OUT_OFF_A5   GPIO_ResetBits(GPIOA,GPIO_Pin_5);//A5输出低电平 
#define OUT_ON_A8    GPIO_SetBits(GPIOA,GPIO_Pin_8);//A8输出高电平
#define OUT_OFF_A8   GPIO_ResetBits(GPIOA,GPIO_Pin_8);//A8输出低电平
#define OUT_ON_A11    GPIO_SetBits(GPIOA,GPIO_Pin_11);//A11输出高电平
#define OUT_OFF_A11   GPIO_ResetBits(GPIOA,GPIO_Pin_11);//A11输出低电平
#define OUT_ON_A12    GPIO_SetBits(GPIOA,GPIO_Pin_12);//A12输出高电平
#define OUT_OFF_A12   GPIO_ResetBits(GPIOA,GPIO_Pin_12);//A12输出低电平 
#define OUT_ON_A15    GPIO_SetBits(GPIOA,GPIO_Pin_15);//A15输出高电平
#define OUT_OFF_A15   GPIO_ResetBits(GPIOA,GPIO_Pin_15);//A15输出低电平
void Input_init(void);
void Output_init(void);

#endif
