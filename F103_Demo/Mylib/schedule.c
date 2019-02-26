#include "schedule.h"
long Time_1ms=0;
int Feedback_Theta=0,SetAdvance_Theta=200;
void TDT_Loop_10000Hz(void)//100us执行一次
{
 	float loop_time_10000hz;
	loop_time_10000hz = Get_Cycle_T(2);     /*获取100us准确时间*/
  
  as5048_singelread_angle();  
  
  Feedback_Theta=((int)(as5048_A.reg/(16384.0/14.0)*360.0+42))%360;  //机械角度换电角度
        
  DRV8305.Park.Theta=Feedback_Theta*10+SetAdvance_Theta;
    
  if(DRV8305.Park.Theta>3599)
  DRV8305.Park.Theta=0; 
  else if(DRV8305.Park.Theta<0)
  DRV8305.Park.Theta=3599;  

  Anti_Park_Calc();     
  Svpwm_Module();          
  

}

void TDT_Loop_1000Hz(void)//1ms执行一次
{
 	float loop_time_1000hz;
	loop_time_1000hz = Get_Cycle_T(1);     /*获取1ms准确时间*/


}

void TDT_Loop_500Hz(void)	//2ms执行一次
{	
	float loop_time_500hz;
	loop_time_500hz = Get_Cycle_T(0);     //获取2ms准确时间

}
 

void TDT_Loop_200Hz(void)	//5ms执行一次
{

  

}   


void TDT_Loop_100Hz(void)	//10ms执行一次
{
	float loop_time_100hz;
	loop_time_100hz = Get_Cycle_T(2);     //获取10ms准确时间  
}


void TDT_Loop_50Hz(void)	//20ms执行一次
{

}

void TDT_Loop_20Hz(void)	//50ms执行一次
{
   static u8 timer_50ms = 0;//记录50ms次数
	 if(++timer_50ms > 10)
	 {
		  timer_50ms = 0;
  		LED_TOGGLE;  //呼吸灯，1s闪烁1次
//		  LED_GREEN_TOGGLE; 
         
	 }
}


void TDT_Loop(schedule* robotSchdule)
{
  	if(robotSchdule->cnt_100us >= 1)
	{
		TDT_Loop_10000Hz();	
		robotSchdule->cnt_100us = 0;
	}
	if(robotSchdule->cnt_1ms >= 10)
	{
		TDT_Loop_1000Hz();	
		robotSchdule->cnt_1ms = 0;
	}
	if(robotSchdule->cnt_2ms >= 20)
	{
		TDT_Loop_500Hz();
		robotSchdule->cnt_2ms = 0;
	}		
	if(robotSchdule->cnt_5ms >= 50)
	{	
		TDT_Loop_200Hz();
		robotSchdule->cnt_5ms = 0;
	}
	if(robotSchdule->cnt_10ms >= 100)
	{		
		TDT_Loop_100Hz();
		robotSchdule->cnt_10ms = 0;
	}
	if(robotSchdule->cnt_20ms >= 200)
	{		
		TDT_Loop_50Hz();
		robotSchdule->cnt_20ms = 0;
	}
	if(robotSchdule->cnt_50ms >= 500)
	{		
		TDT_Loop_20Hz();
		robotSchdule->cnt_50ms = 0;
	}
}
