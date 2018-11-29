#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "stm32f10x.h"

//连发模式
#define burstsMode1 		0x01			//单发			间隔200ms允许触发一次
#define burstsMode2  		0x02			//二连发    间隔475ms允许触发一次
#define burstsMode3 		0x03			//三连发    间隔900ms允许触发一次
#define burstsMode4 		0x04			//四连发    间隔9050ms允许触发一次
#define burstsMode5 		0x05			//五连发    间隔1000ms允许触发一次
#define burstsMode6     0x06      //基地模式  时间间隔不限制

//与连发模式对应的节拍之间的时间间隔，保证任1秒时间内不超5发（基地模式除外）
#define beatTimeMode1   200
#define beatTimeMode2   475
#define beatTimeMode3   900
#define beatTimeMode4   950
#define beatTimeMode5   1000
#define beatTimeMode6   0        

typedef struct _position_pid {
																float kp;
																float ki;
																float kd;
																int exp;
																int feedback;
																int err;
																float p_out;
																float err_inc;
																float i_max;
																float i_out;
																float out;
																float sampleTime;
														 } _position_pid;

typedef struct _run_flag {
														u8 shootFlag;
														u8 shootMode;
														u8 shootAllow;
														u8 bulletFired;
														u8 positionIndex;
														u16 beatTime;
												 } _run_flag;

												 
typedef struct _pid
{
	float setValue;
	float feedbackValue;
	float error;
	float error_0;       //当前误差
	float error_1;				//上次误差
	float error_2;				//上上次误差
	float lastError;
	float deltaError;
	float integralError;
	float integralErrorMax;
	float kp;
	float ki;
	float kd;
	float pOut;
	float iOut;
	float dOut;
	float out;
}pid;



extern _position_pid position_pid;
extern _run_flag runFlag;
extern float BLDC_set_speed;
void Position_Control(u8 index);
void TDT_bldc_InnerControl(float* setValue, float* fbValue, float* result, float T);
void BLDC_Control(float T);
extern int set_postion;


#endif
