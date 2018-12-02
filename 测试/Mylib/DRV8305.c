#include "DRV8305.h"
_DRV8305 DRV8305;
#define   HIGH_LEVEL   800
#define   LOW_LEVEL    0 
#define   FULL_LEVEL   2000
#define MOTOR_POWER   24 
#define Ts            1000
#define Time1_Period  1000
#define MAX_Duty      1000
u8 Step_Flag=0;
int PWMA,PWMB,PWMC;
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

//  DRV8305_ENGATE_L;
//  
//  DelayMs(200);
  DRV8305_ENGATE_H;
  
  SPI2_Init();
  
  DRV8305_SCS_H;

//  TDT_TIM3_PWM_Init();
//  TDT_TIM4_PWM_Init(); 
  TIM1_Init(Time1_Period-1,2-1);  
}

 int Output[6]={0};
void DRC8305_Driver_Control(float T)
{


  switch(Step_Flag)
  { 
//    case 0:     //ֹͣ
//      DRV8305.Duty.MOTA.H_duty=LOW_LEVEL;
//      DRV8305.Duty.MOTA.L_duty=LOW_LEVEL;
//      DRV8305.Duty.MOTB.H_duty=LOW_LEVEL;
//      DRV8305.Duty.MOTB.L_duty=LOW_LEVEL;
//      DRV8305.Duty.MOTC.H_duty=LOW_LEVEL;
//      DRV8305.Duty.MOTC.L_duty=LOW_LEVEL;     
//    break; 
//    
//    
//    case 1:  //AB  
//      DRV8305.Duty.MOTA.H_duty=HIGH_LEVEL;
//      DRV8305.Duty.MOTA.L_duty=LOW_LEVEL;
//      DRV8305.Duty.MOTB.H_duty=LOW_LEVEL;
//      DRV8305.Duty.MOTB.L_duty=FULL_LEVEL;
//      DRV8305.Duty.MOTC.H_duty=LOW_LEVEL;
//      DRV8305.Duty.MOTC.L_duty=LOW_LEVEL;     
//    break;
//    
//    case 2:  //AC  
//      DRV8305.Duty.MOTA.H_duty=HIGH_LEVEL;
//      DRV8305.Duty.MOTA.L_duty=LOW_LEVEL;
//      DRV8305.Duty.MOTB.H_duty=LOW_LEVEL;
//      DRV8305.Duty.MOTB.L_duty=LOW_LEVEL;
//      DRV8305.Duty.MOTC.H_duty=LOW_LEVEL;
//      DRV8305.Duty.MOTC.L_duty=FULL_LEVEL;     
//    break;
//        
//    case 3:  //BC  
//      DRV8305.Duty.MOTA.H_duty=LOW_LEVEL;
//      DRV8305.Duty.MOTA.L_duty=LOW_LEVEL;
//      DRV8305.Duty.MOTB.H_duty=HIGH_LEVEL;
//      DRV8305.Duty.MOTB.L_duty=LOW_LEVEL;
//      DRV8305.Duty.MOTC.H_duty=LOW_LEVEL;
//      DRV8305.Duty.MOTC.L_duty=FULL_LEVEL;     
//    break;

//    case 4:  //BA  
//      DRV8305.Duty.MOTA.H_duty=LOW_LEVEL;
//      DRV8305.Duty.MOTA.L_duty=FULL_LEVEL;
//      DRV8305.Duty.MOTB.H_duty=HIGH_LEVEL;
//      DRV8305.Duty.MOTB.L_duty=LOW_LEVEL;
//      DRV8305.Duty.MOTC.H_duty=LOW_LEVEL;
//      DRV8305.Duty.MOTC.L_duty=LOW_LEVEL;     
//    break;

//    case 5:  //CA  
//      DRV8305.Duty.MOTA.H_duty=LOW_LEVEL;
//      DRV8305.Duty.MOTA.L_duty=FULL_LEVEL;
//      DRV8305.Duty.MOTB.H_duty=LOW_LEVEL;
//      DRV8305.Duty.MOTB.L_duty=LOW_LEVEL;
//      DRV8305.Duty.MOTC.H_duty=HIGH_LEVEL;
//      DRV8305.Duty.MOTC.L_duty=LOW_LEVEL;     
//    break;

//    case 6:  // CB  
//      DRV8305.Duty.MOTA.H_duty=LOW_LEVEL;
//      DRV8305.Duty.MOTA.L_duty=LOW_LEVEL;
//      DRV8305.Duty.MOTB.H_duty=LOW_LEVEL;
//      DRV8305.Duty.MOTB.L_duty=FULL_LEVEL;
//      DRV8305.Duty.MOTC.H_duty=HIGH_LEVEL;
//      DRV8305.Duty.MOTC.L_duty=LOW_LEVEL;     
//    break;
//   
    default:  break;    
        
  }
  
  memcpy(Output,&DRV8305.Duty,sizeof(DRV8305.Duty));
  DRV8305_Output(Output);
}





static void Anti_Park_Calc(void)
{
 
}  





int UAA;
int UBB;
void Svpwm_Module(void)
{
  u8 Step=0,a,b,c;
  int t1,t2,temp,Sum_OverMod;
  float sqrt_3=1.7320508075689, m=1/*SVPWM调制比*/;
  float Udc=MOTOR_POWER;  
  static float mysin; 
  
//  mysin = model_ident_sin(5,5,0,1000,1);
  
////  DRV8305.Svpwm.UAlpha = 15 * mysin;
////  DRV8305.Svpwm.UBeta = 15 * sqrt(1-mysin*mysin);
  
  UAA = DRV8305.Svpwm.UAlpha*100;
  UBB = DRV8305.Svpwm.UBeta*100;
  
  //确定扇区
  DRV8305.Svpwm.Ua=DRV8305.Svpwm.UBeta;
  DRV8305.Svpwm.Ub=   sqrt_3 * DRV8305.Svpwm.UAlpha/2 - DRV8305.Svpwm.UBeta/2;
  DRV8305.Svpwm.Uc= - sqrt_3 * DRV8305.Svpwm.UAlpha/2 - DRV8305.Svpwm.UBeta/2;  
  
  a=DRV8305.Svpwm.Ua>0 ? 1 :0;
  b=DRV8305.Svpwm.Ub>0 ? 1 :0;
  c=DRV8305.Svpwm.Uc>0 ? 1 :0;
  
  Step= 4*c + 2*b + a;
  
//  DRV8305.Svpwm.Ua=sqrt_3*DRV8305.Svpwm.Ua/Udc*Ts/(0x8000);
//  DRV8305.Svpwm.Ub=sqrt_3*DRV8305.Svpwm.Ub/Udc*Ts/(0x8000);
//  DRV8305.Svpwm.Uc=sqrt_3*DRV8305.Svpwm.Uc/Udc*Ts/(0x8000);
  DRV8305.Svpwm.Ua=sqrt_3*DRV8305.Svpwm.Ua/Udc*Ts;
  DRV8305.Svpwm.Ub=sqrt_3*DRV8305.Svpwm.Ub/Udc*Ts;
  DRV8305.Svpwm.Uc=sqrt_3*DRV8305.Svpwm.Uc/Udc*Ts; 
  
  
  switch(Step)
  {   
    case 0:
             DRV8305.Svpwm.taOn = Time1_Period / 2;
             DRV8305.Svpwm.tbOn = Time1_Period / 2;
             DRV8305.Svpwm.tcOn = Time1_Period / 2;
             break;
    
    case 1:           
             t1=-DRV8305.Svpwm.Ub;
             t2=-DRV8305.Svpwm.Uc;
             Sum_OverMod=t1+t2;
             t1=Sum_OverMod>Ts ? t1*Ts/Sum_OverMod : t1;
             t2=Sum_OverMod>Ts ? t2*Ts/Sum_OverMod : t2;
             DRV8305.Svpwm.tbOn = (Ts-t1-t2)/4;
             DRV8305.Svpwm.taOn = DRV8305.Svpwm.tbOn + t1/2;
             DRV8305.Svpwm.tcOn = DRV8305.Svpwm.taOn + t2/2;
             break;
    
    case 2:           
             t1=-DRV8305.Svpwm.Uc;
             t2=-DRV8305.Svpwm.Ua;
             Sum_OverMod=t1+t2;
             t1=Sum_OverMod>Ts ? t1*Ts/Sum_OverMod : t1;
             t2=Sum_OverMod>Ts ? t2*Ts/Sum_OverMod : t2;
             DRV8305.Svpwm.taOn = (Ts-t1-t2)/4;
             DRV8305.Svpwm.tcOn = DRV8305.Svpwm.taOn + t1/2;
             DRV8305.Svpwm.tbOn = DRV8305.Svpwm.tcOn + t2/2;
             break;

    case 3:           
             t1=DRV8305.Svpwm.Ub;
             t2=DRV8305.Svpwm.Ua;
             Sum_OverMod=t1+t2;
             t1=Sum_OverMod>Ts ? t1*Ts/Sum_OverMod : t1;
             t2=Sum_OverMod>Ts ? t2*Ts/Sum_OverMod : t2;
             DRV8305.Svpwm.taOn = (Ts-t1-t2)/4;
             DRV8305.Svpwm.tbOn = DRV8305.Svpwm.taOn + t1/2;
             DRV8305.Svpwm.tcOn = DRV8305.Svpwm.tbOn + t2/2;
             break;        

    case 4:           
             t1=-DRV8305.Svpwm.Ua;
             t2=-DRV8305.Svpwm.Ub;
             Sum_OverMod=t1+t2;
             t1=Sum_OverMod>Ts ? t1*Ts/Sum_OverMod : t1;
             t2=Sum_OverMod>Ts ? t2*Ts/Sum_OverMod : t2;
             DRV8305.Svpwm.tcOn = (Ts-t1-t2)/4;
             DRV8305.Svpwm.tbOn = DRV8305.Svpwm.tcOn + t1/2;
             DRV8305.Svpwm.taOn = DRV8305.Svpwm.tbOn + t2/2;
             break;    

    case 5:           
             t1=DRV8305.Svpwm.Ua;
             t2=DRV8305.Svpwm.Uc;
             Sum_OverMod=t1+t2;
             t1=Sum_OverMod>Ts ? t1*Ts/Sum_OverMod : t1;
             t2=Sum_OverMod>Ts ? t2*Ts/Sum_OverMod : t2;
             DRV8305.Svpwm.tbOn = (Ts-t1-t2)/4;
             DRV8305.Svpwm.tcOn = DRV8305.Svpwm.tbOn + t1/2;
             DRV8305.Svpwm.taOn = DRV8305.Svpwm.tcOn + t2/2;
             break;    

    case 6:           
             t1=DRV8305.Svpwm.Uc;
             t2=DRV8305.Svpwm.Ub;
             Sum_OverMod=t1+t2;
             t1=Sum_OverMod>Ts ? t1*Ts/Sum_OverMod : t1;
             t2=Sum_OverMod>Ts ? t2*Ts/Sum_OverMod : t2;
             DRV8305.Svpwm.tcOn = (Ts-t1-t2)/4;
             DRV8305.Svpwm.taOn = DRV8305.Svpwm.tcOn + t1/2;
             DRV8305.Svpwm.tbOn = DRV8305.Svpwm.taOn + t2/2;
             break;   

    default:break;    
  } 
  
  //倒三角模式，重新计算占空比
    DRV8305.Duty.MOTA =Time1_Period - DRV8305.Svpwm.taOn;
    DRV8305.Duty.MOTB =Time1_Period - DRV8305.Svpwm.tbOn;  
    DRV8305.Duty.MOTC =Time1_Period - DRV8305.Svpwm.tcOn;  
        
    if(t1==0&&t2==0)
    {
     DRV8305.Duty.MOTA =0;
     DRV8305.Duty.MOTB =0;
     DRV8305.Duty.MOTC =0;      
    }      
  
    DRV8305.Duty.MOTA = DRV8305.Duty.MOTA > MAX_Duty ? MAX_Duty :DRV8305.Duty.MOTA; 
    DRV8305.Duty.MOTB = DRV8305.Duty.MOTB > MAX_Duty ? MAX_Duty :DRV8305.Duty.MOTB; 
    DRV8305.Duty.MOTC = DRV8305.Duty.MOTC > MAX_Duty ? MAX_Duty :DRV8305.Duty.MOTC; 
    
    PWMA= DRV8305.Duty.MOTA;
    PWMB= DRV8305.Duty.MOTB; 
    PWMC= DRV8305.Duty.MOTC;
    
    TIM1->CCR1 = DRV8305.Duty.MOTA ;
    TIM1->CCR2 = DRV8305.Duty.MOTB ;    
    TIM1->CCR3 = DRV8305.Duty.MOTC ;    
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