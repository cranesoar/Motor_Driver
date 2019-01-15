#include "main.h"

u16 DRV=0,Check=0xBFFF;
int FOC_T=500;
int main(void)
{

    Board_ALL_Init();

    while(1)
    { 
      
//  DRV8305_SCS_L;        
//  DRV=SPI1_ReadWriteByte(Check); 
//  DelayMs(100);      
//  DRV8305_SCS_H;
//  DelayUs(1);
     
//    DelayUs(100);
    DRV8305.Park.Theta=DRV8305.Park.Theta+10;
    if(DRV8305.Park.Theta>3599)
    DRV8305.Park.Theta=0;    
  
    Anti_Park_Calc();     
    Svpwm_Module();      
    }
}     

