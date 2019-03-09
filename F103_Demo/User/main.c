#include "main.h"

//测得磁编码器对应的机械角0度，对应电角度=>165度（正负1度），机械角度是电角度的14倍。
//则机械角度与电角度的换算关系为：电角度=[（机械角度/（16384/14））*360+165]%360
//在while（1）中一次循环96us，移至schedule的100us中断里
int FOC_T=500;
long Change=0;
int main(void)
{

  Board_ALL_Init(); 
  
    while(1)
    {    
 
//  as5048_singelread_angle();  
//  
//  Feedback_Theta=((int)(as5048_A.reg/(16384.0/14.0)*360.0+42))%360;  //机械角度换电角度
//        
//  DRV8305.Park.Theta=Feedback_Theta*10+SetAdvance_Theta;
//      
//  if(DRV8305.Park.Theta>3599)
//  DRV8305.Park.Theta=0; 
//  else if(DRV8305.Park.Theta<0)
//  DRV8305.Park.Theta=3599;  

//  Clarke_Trans();
// // Park_Trans();     
//  Anti_Park_Calc();     
//  Svpwm_Module(); 

    
    }
}     

