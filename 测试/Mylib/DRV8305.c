#include "DRV8305.h"


void DRV8305_Init(void)
{
 
  GPIO_InitTypeDef  GPIO_InitStructure; 
  
  RCC_AHB1PeriphClockCmd(RCC_DRV8305 , ENABLE );
  GPIO_InitStructure.GPIO_Pin =  DRV8305_Pin_SCS|DRV8305_Pin_ENGATE;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(DRV8305_PORT, &GPIO_InitStructure);

  DRV8305_ENGATE_H;
  
  SPI2_Init();
  
  DRV8305_SCS_H;
  

//  TDT_TIM3_PWM_Init();
//  TDT_TIM4_PWM_Init();  
}



