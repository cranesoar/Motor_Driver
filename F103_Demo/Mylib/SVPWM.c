#include "SVPWM.h"

u8 Step_Flag=0;
int PWMA,PWMB,PWMC,UAA,UBB;
int Section=0;

void Anti_Clark_Calc(void)//反CLARK变换
{
  
  float Sinthe,Costhe; 
  u16 Point;
  
  Point = DRV8305.Park.Theta;
  
  Costhe= Point<2700 ? SinTable[Point+900] : SinTable[Point-2700] ;
  Sinthe= SinTable[Point];
  
  DRV8305.Svpwm.UAlpha=DRV8305.Park.Ud * Costhe - DRV8305.Park.Uq * Sinthe;
  DRV8305.Svpwm.UBeta =DRV8305.Park.Ud * Sinthe + DRV8305.Park.Uq * Costhe;
  
//  DRV8305.Svpwm.UAlpha=13 * Costhe - 0 * Sinthe;
//  DRV8305.Svpwm.UBeta =13 * Sinthe + 0 * Costhe;    
} 



void Anti_Park_Calc(void)//反PARK变换
{
  
  float Sinthe,Costhe; 
  u16 Point;
  
  Point = DRV8305.Park.Theta;
  
  Costhe= Point<2700 ? SinTable[Point+900] : SinTable[Point-2700] ;
  Sinthe= SinTable[Point];
  
  DRV8305.Svpwm.UAlpha=DRV8305.Park.Ud * Costhe - DRV8305.Park.Uq * Sinthe;
  DRV8305.Svpwm.UBeta =DRV8305.Park.Ud * Sinthe + DRV8305.Park.Uq * Costhe;
  
//  DRV8305.Svpwm.UAlpha=13 * Costhe - 0 * Sinthe;
//  DRV8305.Svpwm.UBeta =13 * Sinthe + 0 * Costhe;    
}  

 int Sum_OverMod=0;
void Svpwm_Module(void)
{
  u8 Step=0,a,b,c;
  int t1,t2,temp;
  float sqrt_3=1.7320508; // m=sqrt(3)*Uref*Udc/*SVPWM调制比*/;
  float Udc=MOTOR_POWER;  
  static float mysin; 
  
  UAA = DRV8305.Svpwm.UAlpha*100;
  UBB = DRV8305.Svpwm.UBeta*100;
  
  //确定扇区
  DRV8305.Svpwm.Ua=   DRV8305.Svpwm.UBeta;
  DRV8305.Svpwm.Ub=   sqrt_3 * DRV8305.Svpwm.UAlpha/2 - DRV8305.Svpwm.UBeta/2;
  DRV8305.Svpwm.Uc= - sqrt_3 * DRV8305.Svpwm.UAlpha/2 - DRV8305.Svpwm.UBeta/2;  
  
  a=DRV8305.Svpwm.Ua>0 ? 1 :0;
  b=DRV8305.Svpwm.Ub>0 ? 1 :0;
  c=DRV8305.Svpwm.Uc>0 ? 1 :0;
  
  Step= 4*c + 2*b + a;  //计算所处扇区位置
  
  DRV8305.Svpwm.Ua=sqrt_3*DRV8305.Svpwm.Ua/Udc*Ts;
  DRV8305.Svpwm.Ub=sqrt_3*DRV8305.Svpwm.Ub/Udc*Ts;
  DRV8305.Svpwm.Uc=sqrt_3*DRV8305.Svpwm.Uc/Udc*Ts; 
  
  switch(Step)     //3 一扇区 1 二扇区 5 三扇区 4 四扇区 6 五扇区 2 六扇区      
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
             Section=2;
             R3LM1_SetADCSampPointSect2();    
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
             Section=6;
             R3LM1_SetADCSampPointSect6();    
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
             Section=1;
             R3LM1_SetADCSampPointSect1();    
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
             Section=4;
             R3LM1_SetADCSampPointSect4();    
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
             Section=3;
             R3LM1_SetADCSampPointSect3();    
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
             Section=5;
             R3LM1_SetADCSampPointSect5();
             break;   

    default:break;    
  } 
    
    
  //倒三角模式，重新计算占空比
    DRV8305.Duty.MOTA =Time1_Period - 2*DRV8305.Svpwm.taOn;
    DRV8305.Duty.MOTB =Time1_Period - 2*DRV8305.Svpwm.tbOn;  
    DRV8305.Duty.MOTC =Time1_Period - 2*DRV8305.Svpwm.tcOn;  
        
    if(t1==0&&t2==0)
    {
     DRV8305.Duty.MOTA =MAX_Duty;
     DRV8305.Duty.MOTB =MAX_Duty;
     DRV8305.Duty.MOTC =MAX_Duty;      
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

//    TIM1->CCR1 = 0 ;
//    TIM1->CCR2 = 0 ;    
//    TIM1->CCR3 = 0 ;       
}
