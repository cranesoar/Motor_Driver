#ifndef _CONTROL_H
#define	_CONTROL_H

#include "board.h"

#define  Peak_Position 0
#define  Bottom_Position 1

#define MAX_PITCH_ASPPED  1200	///云台pitch的最大旋转角速度
#define MID_YAW_ANGLE     11816
#define BULLET_POSITION   300
#define DISTANCE_SENSOR   1710  //两激光测距间距 51CM
#define GET_POSITION      95000
#define WHIFF_ON           TIM3->CCR2=9000
#define WHIFF_OFF          TIM3->CCR2=4000
#define YAW_MID           30793 
#define WHIFF_MID         31744 
#define PITCH_MID         31800
#define SINGLESHOOT  			32768  //8192*36/9
typedef struct 
{
	int16_t speed_set;       //速度给定
	int16_t speed_output;    //速度给定,直接用于can发送，由speed_set得出
}Motor;

/*****************模式选择*************************/
typedef enum
{
  ANGLE_CTRL_MODE,        //云台机械角控制模式
  RADPS_CTRL_MODE,        //云台陀螺仪角度控制模式
	FOLLOWING_MODE,         //底盘跟随模式
  SWING_MODE,             //底盘摇摆模式
	MANUAL_MODE,            //普通手动模式
	UNCONTROL_MODE,         //环视模式
	GETTING_MODE,           //取弹模式
	REVOLVE_UP_MODE,        //上岛抱柱旋转模式 
	REVOLVE_DOWN_MODE,      //下岛抱柱旋转模式
	PARALLEL_MODE,          //校准车体姿态平行资源岛模式
}_mode_e;


struct _Chassis_data
{
	u8 enable_flag;
	int forward_back_data;
	int left_right_data;
	int rotation_data;
	_mode_e mode_data;
};



extern Motor motor[6];
extern struct _Chassis_data Chassis_data;
extern float Elevator_right_outer,Elevator_left_outer,Position_set_zhua,Position_set_dan,Elastic_3510_outer ,GM3510_SPEED_Pitch;
extern int Yaw_aimposition,Whiff_aimposition,Number_box;
//extern float forward_back,forward_back_out,left_right, rotation;
/*底盘速度设置*/
void Chassis_Speed_Set(void);
 
/*底盘电机PID计算*/
void TDT_Chassis_PidControl(vec4f* setValue, vec4f* fbValue, vec4f* result, float T, u8 dimConVar);
/*底盘电机PID控制*/
void TDT_Chassis_Control(float T);
/*底盘电机数据发送*/
void TDT_Chassis_OutUpdate(vec4f* controllerOut);

void TDT_Elevator_OutUpdate(float* controllerOut_right, float* controllerOut_left, float* elastic_out);
void TDT_Position_InnerControl(float* setValue, float* fbValue, float* result, float T);
void TDT_Position_OuterControl(float* setValue, double* fbValue, float* result, float T);
void TDT_Position_Slow_OuterControl(float* setValue, double* fbValue, float* result, float T);
void TDT_3510_Position_Control(float T);
void TDT_3510_Elastic_Control(float T);
void TDT_3510_Talon_Control(float T);
void TDT_Pitch_MPU_Control(float T);
void TDT_Pitch_Mech_Control(float T);
void TDT_Yaw_Control(float T);
void TDT_Whiff_Control(float T);
void TDT_Pitch_MPU_OuterControl(float* setValue, float* fbValue, float* result, float T);
void TDT_Pitch_MPU_InnerControl(float* setValue, float* fbValue, float* result, float T);
void TDT_Pitch_Mech_OuterControl(float* setValue, float* fbValue, float* result, float T);
void TDT_Pitch_Mech_InnerControl(float* setValue, float* fbValue, float* result, float T);
void TDT_Yaw_OuterControl(float* setValue, float* fbValue, float* result, float T);
void TDT_Yaw_InnerControl(float* setValue, float* fbValue, float* result, float T);
void TDT_Whiff_OuterControl(float* setValue, float* fbValue, float* result, float T);
void TDT_Whiff_InnerControl(float* setValue, float* fbValue, float* result, float T);
void Elevator_right_Control(float T);
void Elevator_left_Control (float T);
void TDT_Elevator_Control(int mode, float T );
void Action_Control( float T);
void TDT_feedSprocket_InnerControl(float* setValue, float* fbValue, float* result, float T);
void TDT_FeedSprocket_OuterControl(float* setValue, float* fbValue, float* result, float T);
void TDT_FeedSprocket_Position_Control(float T);
#endif
