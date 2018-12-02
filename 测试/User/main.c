#include "board.h"
int unm11=1000;
unsigned char dat[6]={0};
u16 DRV=0,Check=0xBFFF;
int main(void)
{
Board_ALL_Init();
	
	while(1)
	{
  
//  DRV8305_SCS_L;   
//  
//  DelayMs(100);    
//  DRV=SPI2_ReadWriteByte(Check); 
//    
//  DRV8305_SCS_H;
//  
//  DelayUs(1);   
  Svpwm_Module();
   
  }

}
