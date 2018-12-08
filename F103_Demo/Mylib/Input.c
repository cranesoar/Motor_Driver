#include "main.h"

void Input_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE); 
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE); 
  GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;      //设置成上拉输入 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);       
  
}

void Output_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
  GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_15; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
}

