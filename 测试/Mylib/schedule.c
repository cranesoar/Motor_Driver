#include "schedule.h"
float aim=0,zhi_2006=0,zhi_3510=0,zhi_robo=0,speed_robo=0;
float shoot1=0,shoot2=9500;
_Bool DanStart_flag=0,flag_Up=0,flag_Down=0;

void TDT_Loop_1000Hz(void)//1ms执行一次
{
    static int Timer_BLDC=0;
 	float loop_time_1000hz;
	loop_time_1000hz = Get_Cycle_T(1);     /*获取5ms准确时间*/

//   model_ident_sin(5,50,0,1000,1);
//	start_task(6);

}

void TDT_Loop_500Hz(void)	//2ms执行一次
{	
	float zero_speed=2000;
	static _Bool New_init=0;	
	float loop_time_500hz;
	loop_time_500hz = Get_Cycle_T(0);     //获取2ms准确时间

}
 

void TDT_Loop_200Hz(void)	//5ms执行一次
{



}   


void TDT_Loop_100Hz(void)	//10ms执行一次
{
	float loop_time_100hz;
	static u8 timer_10ms = 0;	
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
//			LED_RED_TOGGLE;   //呼吸灯，1s闪烁1次
		  LED_GREEN_TOGGLE; 
         
	 }
   

}


void TDT_Loop(schedule* robotSchdule)
{
	if(robotSchdule->cnt_1ms >= 1)
	{
		TDT_Loop_1000Hz();	
		robotSchdule->cnt_1ms = 0;
	}
	if(robotSchdule->cnt_2ms >= 2)
	{
		TDT_Loop_500Hz();
		robotSchdule->cnt_2ms = 0;
	}		
	if(robotSchdule->cnt_5ms >= 5)
	{	
		TDT_Loop_200Hz();
		robotSchdule->cnt_5ms = 0;
	}
	if(robotSchdule->cnt_10ms >= 10)
	{		
		TDT_Loop_100Hz();
		robotSchdule->cnt_10ms = 0;
	}
	if(robotSchdule->cnt_20ms >= 20)
	{		
		TDT_Loop_50Hz();
		robotSchdule->cnt_20ms = 0;
	}
	if(robotSchdule->cnt_50ms >= 50)
	{		
		TDT_Loop_20Hz();
		robotSchdule->cnt_50ms = 0;
	}
}
