#include "Foc.h"

_DRV8305 DRV8305;

void Clarke_Trans(void)
{
   DRV8305.Clack.IAlpha = (int32_t)(DRV8305.Clack.Ia);
   DRV8305.Clack.IBeta  = (int32_t)((DRV8305.Clack.Ia + 2*DRV8305.Clack.Ib)/1.7320508);  
}

void Park_Trans(void)
{
  float Sinthe,Costhe; 
  u16 Point;
  
  Point = DRV8305.Park.Theta ;
  
  Costhe= Point<2700 ? SinTable[Point+900] : SinTable[Point-2700] ;
  Sinthe= SinTable[Point];
   
  DRV8305.Park.ActId = DRV8305.Clack.IAlpha * Costhe + DRV8305.Clack.IBeta  * Sinthe;
  DRV8305.Park.ActIq = DRV8305.Clack.IBeta  * Costhe - DRV8305.Clack.IAlpha * Sinthe;
  
  DRV8305.Park.Id = (int32_t)(LPF2pApply(0,DRV8305.Park.ActId)); 
  DRV8305.Park.Iq = (int32_t)(LPF2pApply(1,DRV8305.Park.ActIq));       
}

float Outmax_Id,Outmax_Iq;
void Id_PI_Controller(void)
{
  float error,SetValue,FbValue;
  float U,Out,Excess,Sum;  //U 无限输出 Out 有限输出 Excess 无限输出与有限输出之差
  
  SetValue = DRV8305.Speed.Id_Result;
  FbValue  = DRV8305.Park.Id;
  error    = SetValue - FbValue;
  U        = Sum + DRV8305.PID.Id.Kp * error;
  Out      = limit( U , -Outmax_Id , Outmax_Id);
  DRV8305.Park.Ud = Out;
  Excess   = U - Out;
  Sum      = Sum + (DRV8305.PID.Id.Ki * error) - (DRV8305.PID.Id.Kc * Excess); 
}

void Iq_PI_Controller(void)
{
  float error,SetValue,FbValue;
  float U,Out,Excess,Sum;  //U 无限输出 Out 有限输出 Excess 无限输出与有限输出之差
  
  SetValue = DRV8305.Speed.Iq_Result;
  FbValue  = DRV8305.Park.Iq;
  error    = SetValue - FbValue;
  U        = Sum + DRV8305.PID.Iq.Kp * error;
  Out      = limit( U , -Outmax_Iq , Outmax_Iq);
  DRV8305.Park.Uq = Out;
  Excess   = U - Out;
  Sum      = Sum + (DRV8305.PID.Iq.Ki * error) - (DRV8305.PID.Iq.Kc * Excess); 
}
