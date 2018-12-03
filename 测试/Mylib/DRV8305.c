#include "DRV8305.h"
struct Parameter
{
  float Kp;
  float Ki;
};
_DRV8305 DRV8305;
#define   HIGH_LEVEL   800
#define   LOW_LEVEL    0 
#define   FULL_LEVEL   2000
#define MOTOR_POWER   24 
#define Ts            1000
#define Time1_Period  1000
#define MAX_Duty      1000
u8 Step_Flag=0;
int PWMA,PWMB,PWMC,UAA,UBB;
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

  TIM1_Init(Time1_Period-1,8-1);  
}





void Clark_Calc(void)//Clark变换
{
  float sqrt_3=1.7320508075689;
  
  DRV8305.Park.IAlpha = DRV8305.Clack.Ia;
  DRV8305.Park.IBeta  = (DRV8305.Clack.Ia + DRV8305.Clack.Ib*2)*sqrt_3/3; 
}  

void Park_Calc(void)//PARK变换
{
  float Sinthe,Costhe;
  u16   Point;
  
  Point = DRV8305.Park.Theta;  

  Costhe= Point<2700 ? SinTable[Point+900] : SinTable[Point-2700] ;
  Sinthe= SinTable[Point];

  DRV8305.Park.Id = DRV8305.Park.IAlpha * Costhe + DRV8305.Park.IBeta * Sinthe;
  DRV8305.Park.Iq = -DRV8305.Park.IAlpha * Sinthe + DRV8305.Park.IBeta * Costhe;
  
  
  
}

void  FOC_PI_Controller(float setValue, float Feedback, struct Parameter Idref_Pi,float *resualt)
{
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
}  


void Svpwm_Module(void)
{
  u8 Step=0,a,b,c;
  int t1,t2,temp,Sum_OverMod;
  float sqrt_3=1.7320508075689, m=1/*SVPWM调制比*/;
  float Udc=MOTOR_POWER;  
  static float mysin; 
  
  UAA = DRV8305.Svpwm.UAlpha*100;
  UBB = DRV8305.Svpwm.UBeta*100;
  
  //确定扇区
  DRV8305.Svpwm.Ua=DRV8305.Svpwm.UBeta;
  DRV8305.Svpwm.Ub=   sqrt_3 * DRV8305.Svpwm.UAlpha/2 - DRV8305.Svpwm.UBeta/2;
  DRV8305.Svpwm.Uc= - sqrt_3 * DRV8305.Svpwm.UAlpha/2 - DRV8305.Svpwm.UBeta/2;  
  
  a=DRV8305.Svpwm.Ua>0 ? 1 :0;
  b=DRV8305.Svpwm.Ub>0 ? 1 :0;
  c=DRV8305.Svpwm.Uc>0 ? 1 :0;
  
  Step= 4*c + 2*b + a;  //计算所处扇区位置
  
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