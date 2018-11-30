#include "DRV8305.h"
_DRV8305 DRV8305;
#define   HIGH_LEVEL   800
#define   LOW_LEVEL    0 
#define   FULL_LEVEL   2000
u8 Step_Flag=0;
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

  TDT_TIM3_PWM_Init();
  TDT_TIM4_PWM_Init();  
}

 int Output[6]={0};
void DRC8305_Driver_Control(float T)
{


  switch(Step_Flag)
  { 
    case 0:     //ֹͣ
      DRV8305.Duty.MOTA.H_duty=LOW_LEVEL;
      DRV8305.Duty.MOTA.L_duty=LOW_LEVEL;
      DRV8305.Duty.MOTB.H_duty=LOW_LEVEL;
      DRV8305.Duty.MOTB.L_duty=LOW_LEVEL;
      DRV8305.Duty.MOTC.H_duty=LOW_LEVEL;
      DRV8305.Duty.MOTC.L_duty=LOW_LEVEL;     
    break; 
    
    
    case 1:  //AB  
      DRV8305.Duty.MOTA.H_duty=HIGH_LEVEL;
      DRV8305.Duty.MOTA.L_duty=LOW_LEVEL;
      DRV8305.Duty.MOTB.H_duty=LOW_LEVEL;
      DRV8305.Duty.MOTB.L_duty=FULL_LEVEL;
      DRV8305.Duty.MOTC.H_duty=LOW_LEVEL;
      DRV8305.Duty.MOTC.L_duty=LOW_LEVEL;     
    break;
    
    case 2:  //AC  
      DRV8305.Duty.MOTA.H_duty=HIGH_LEVEL;
      DRV8305.Duty.MOTA.L_duty=LOW_LEVEL;
      DRV8305.Duty.MOTB.H_duty=LOW_LEVEL;
      DRV8305.Duty.MOTB.L_duty=LOW_LEVEL;
      DRV8305.Duty.MOTC.H_duty=LOW_LEVEL;
      DRV8305.Duty.MOTC.L_duty=FULL_LEVEL;     
    break;
        
    case 3:  //BC  
      DRV8305.Duty.MOTA.H_duty=LOW_LEVEL;
      DRV8305.Duty.MOTA.L_duty=LOW_LEVEL;
      DRV8305.Duty.MOTB.H_duty=HIGH_LEVEL;
      DRV8305.Duty.MOTB.L_duty=LOW_LEVEL;
      DRV8305.Duty.MOTC.H_duty=LOW_LEVEL;
      DRV8305.Duty.MOTC.L_duty=FULL_LEVEL;     
    break;

    case 4:  //BA  
      DRV8305.Duty.MOTA.H_duty=LOW_LEVEL;
      DRV8305.Duty.MOTA.L_duty=FULL_LEVEL;
      DRV8305.Duty.MOTB.H_duty=HIGH_LEVEL;
      DRV8305.Duty.MOTB.L_duty=LOW_LEVEL;
      DRV8305.Duty.MOTC.H_duty=LOW_LEVEL;
      DRV8305.Duty.MOTC.L_duty=LOW_LEVEL;     
    break;

    case 5:  //CA  
      DRV8305.Duty.MOTA.H_duty=LOW_LEVEL;
      DRV8305.Duty.MOTA.L_duty=FULL_LEVEL;
      DRV8305.Duty.MOTB.H_duty=LOW_LEVEL;
      DRV8305.Duty.MOTB.L_duty=LOW_LEVEL;
      DRV8305.Duty.MOTC.H_duty=HIGH_LEVEL;
      DRV8305.Duty.MOTC.L_duty=LOW_LEVEL;     
    break;

    case 6:  // CB  
      DRV8305.Duty.MOTA.H_duty=LOW_LEVEL;
      DRV8305.Duty.MOTA.L_duty=LOW_LEVEL;
      DRV8305.Duty.MOTB.H_duty=LOW_LEVEL;
      DRV8305.Duty.MOTB.L_duty=FULL_LEVEL;
      DRV8305.Duty.MOTC.H_duty=HIGH_LEVEL;
      DRV8305.Duty.MOTC.L_duty=LOW_LEVEL;     
    break;
   
    default:  break;    
        
  }
  
  memcpy(Output,&DRV8305.Duty,sizeof(DRV8305.Duty));
  DRV8305_Output(Output);
}



void DRV8305_Output(int Duty[6])
{
  
    TIM4->CCR1=Duty[0];  //AH   PB6
		TIM4->CCR2=Duty[1];  //AL   PB7
		TIM4->CCR3=Duty[2];  //BH   PB8
		TIM4->CCR4=Duty[3];  //BL   PB9
		TIM3->CCR2=Duty[4];  //CH   PC7
		TIM3->CCR3=Duty[5];  //CL   PC8
}