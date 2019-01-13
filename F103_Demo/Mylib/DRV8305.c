#include "DRV8305.h"

void DRV8305_Init(void)
{
 
  GPIO_InitTypeDef  GPIO_InitStructure; 
  
  RCC_APB2PeriphClockCmd(RCC_DRV8305 , ENABLE );
  GPIO_InitStructure.GPIO_Pin =  DRV8305_Pin_SCS|DRV8305_Pin_ENGATE;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//ÍÆÍìÊä³ö
  GPIO_Init(DRV8305_PORT, &GPIO_InitStructure);

  DRV8305_ENGATE_H;
  
  SPI1_Init();
  
  DRV8305_SCS_H;
  
}
