#include "main.h"

struct _schedule schedule;

void Loop_1000Hz(void)	//1ms执行一次
{
//	as5048_read_angle();
    as5048_singelread_angle();
    as5048_data_prepare();
    LED_State(as5048_A.speed_ef,as5048_B.speed_ef,0.001);
}

void Loop_500Hz(void)	 //2ms执行一次
{
	
}
int16_t set_speed1 = 4000;
int16_t set_speed2 = 4000;

int16_t speed_error=0;
void Loop_200Hz(void)	 //5ms执行一次
{
	
    vec2int16 set_val,fb_val,result;
    set_val.data[0] = canFeedback.Set_speed_A;
    set_val.data[1] = canFeedback.Set_speed_B;	
    fb_val.data[0] = as5048_B.speed_ef;
    fb_val.data[1] = -as5048_A.speed_ef;
		speed_error  = fb_val.data[0] - fb_val.data[1];
    TDT_Friction_PidControl(&set_val,&fb_val,&result,0.05,2);
    TIM1->CCR1 = result.data[1];
    TIM1->CCR2 = result.data[0];
		if(canFeedback.Set_speed_A == 0 &&canFeedback.Set_speed_B == 0)
		{
			TIM1->CCR1 = 3900;
			TIM1->CCR2 = 3900;
		}
//    TIM1->CCR1 = set_speed1;   
//    TIM1->CCR2 = set_speed2;

}

void Loop_100Hz(void)  //10ms执行一次
{
	
	
}

void Loop_50Hz(void)	//20ms执行一次
{
	
	
}

void Loop_20Hz(void)	//50ms执行一次
{
    static u8 timer_50ms = 0;//记录50ms次数
    if(++timer_50ms > 10)
    {
        timer_50ms = 0;
//		 LED_OFF;
        LED_TOGGLE;
    }
}

void Loop_10Hz(void)	//100ms执行一次
{

}

void Loop(void)
{
    if(schedule.cnt_1ms >= 1)
    {
        Loop_1000Hz();
        schedule.cnt_1ms = 0;
    }
    if(schedule.cnt_2ms >= 2)
    {
        Loop_500Hz();
        schedule.cnt_2ms = 0;
    }
    if(schedule.cnt_5ms >= 5)
    {
        Loop_200Hz();
        schedule.cnt_5ms = 0;
    }
    if(schedule.cnt_10ms >= 10)
    {
        Loop_100Hz();
        schedule.cnt_10ms = 0;
    }
    if(schedule.cnt_20ms >= 20)
    {
        Loop_50Hz();
        schedule.cnt_20ms = 0;
    }
    if(schedule.cnt_50ms >= 50)
    {
        Loop_20Hz();
        schedule.cnt_50ms = 0;
    }
    if(schedule.cnt_100ms >= 100)
    {
        Loop_10Hz();
        schedule.cnt_100ms = 0;
    }
}
