#include "control.h"
float keyboard_f;//键盘控制底盘速度系数
int Yaw_aimposition=0,Whiff_aimposition=0,Pitch_aimposition=-5000;
struct _Chassis_data Chassis_data;
float sin_rad,cos_rad;
double yaw_rad;
float forward_back=0,forward_back_out=0,left_right=0, rotation=0;
float Elastic_3510_outer=0,Position_Ball=0,Talon_3510_outer=0,xishu=0.1;
float adjust_ele=0;
float swing_signal;
float Dan_zhua=0,GM3510_SPEED_Pitch=0;
int Number_box=0;
  _Bool wind_flag=-1;






/**
  * @brief  云台pitch内环控制
  * @param  setValue: 期望角速度（度每秒）
	*         fbValue：当前角速度反馈值
	*         result：控制结果的存放位置
	*					T：控制周期
  * @retval None
  */
void TDT_Pitch_MPU_InnerControl(float* setValue, float* fbValue, float* result, float T)
{
	static pid pitchInner;
	static u8 paraLoadFlag = 0;
	
	/* 如果参数没有加载，加载参数*/	
	if(!paraLoadFlag)
	{
		TDT_Get_PIDparameters(&pitchInner, pidPitchInner);
		paraLoadFlag = 1;
	}
    /* 设定值 */	
		pitchInner.setValue = *setValue;	
		/* 反馈值 */			
		pitchInner.feedbackValue = *fbValue;	
  	/* 偏差 = 设定值 - 反馈值 */
		pitchInner.error = pitchInner.setValue - pitchInner.feedbackValue;	
		/* 比例项的输出 */			
		pitchInner.pOut = pitchInner.kp * pitchInner.error;
		/* 微分项的输出 */			
		pitchInner.dOut = pitchInner.kd * (pitchInner.error - pitchInner.lastError);
	  pitchInner.lastError = pitchInner.error;
		/* 总的输出 = 比例项的输出*/			
		pitchInner.out = pitchInner.pOut+pitchInner.dOut;	
		/* 总的输出不能超出电机给定值的范围 */	
		*result = LIMIT(pitchInner.out, -MAXSETDM50, MAXSETDM50);	
}

/**
  * @brief  云台pitch外环控制
  * @param  setValue: 期望角度（度）
	*         fbValue：当前角度反馈值
	*         result：控制结果的存放位置
	*					T：控制周期
  * @retval None
  */
void TDT_Pitch_MPU_OuterControl(float* setValue, float* fbValue, float* result, float T)
{
	static pid pitchOuter;
	static u8 paraLoadFlag = 0;
	
	/* 如果参数没有加载，加载参数*/	
	if(!paraLoadFlag)
	{
		TDT_Get_PIDparameters(&pitchOuter, pidPitchOuter);
		paraLoadFlag = 1;
	}

		/* 设定值 */	
		pitchOuter.setValue = *setValue;		
		/* 反馈值 */			
		pitchOuter.feedbackValue = *fbValue;		
		/* 偏差 = 设定值 - 反馈值 */
		pitchOuter.error = pitchOuter.setValue - pitchOuter.feedbackValue;	
		/* 偏差进行积分 */
		pitchOuter.integralError += pitchOuter.error * T;		
		/* 偏差的积分进行限制 */
		pitchOuter.integralError = LIMIT(pitchOuter.integralError, -pitchOuter.integralErrorMax, pitchOuter.integralErrorMax);	
		/* 比例项的输出 */
		pitchOuter.pOut = pitchOuter.kp * pitchOuter.error;	
		/* 积分项的输出 */
		pitchOuter.iOut = pitchOuter.ki * pitchOuter.integralError;
		/* 总的输出 = 比例项的输出 + 积分项输出 */
		pitchOuter.out = pitchOuter.pOut + pitchOuter.iOut;
		/* 内环的给定值在[-300,300]度每秒 */	
		*result = LIMIT(pitchOuter.out, -MAX_PITCH_ASPPED, MAX_PITCH_ASPPED);	
}
   /**
  * @brief  云台pitch控制  (MPU6050)
  * @param  T：控制周期
  * @retval None
  */

  
void TDT_Pitch_MPU_Control(float T)
{
	float innerSetValue = 0, innerFbValue = 0, innerResult = 0;
	float outerFbValue = 0;
	static float outerSetValue , outerResult = 0;//6000
  static float  pitchSetAngle=0,mouseAngle=0;
	static u8 outerControl = 0;
	
	float controlCycle = T;

	/* 内环设定值等于外环输出值 */	
	innerSetValue = outerResult;
	/* 云台绕y轴旋转的陀螺仪数值作为内环角速度反馈值,逆时针为负，内环设定为负 */
	innerFbValue = mpu6050Top.gyro.dps.data[y];
	TDT_Pitch_MPU_InnerControl(&innerSetValue, &innerFbValue, &innerResult, controlCycle);

	/* 外环的控制周期是内环的2倍 */
	if(++outerControl >= 2)
	{
			outerControl = 0;
		  pitchSetAngle = -RC.CH[1]*55.0f /660.0f; //右摇杆纵向作为pitch轴的设定值
      /*限制Pitch给定范围*/
			pitchSetAngle = LIMIT(pitchSetAngle, -30,25);
		  /*有大符控制的增加给定角度*/
			outerSetValue = pitchSetAngle;
      /*6623的机械角度作为外环角度反馈*/		
			outerFbValue = gimbalTopAngle.pitch;
			TDT_Pitch_MPU_OuterControl(&outerSetValue, &outerFbValue, &outerResult, 2*controlCycle);
	}				
	DM50_SPEED_Pitch1 = innerResult;			//内环结果给pitch1，通过485发送给电调
  DM50_SPEED_Pitch2 = -DM50_SPEED_Pitch1;//另一个pitch2取pitch1的相反数
}


///**
//  * @brief  云台pitch内环控制
//  * @param  setValue: 期望角速度（度每秒）
//	*         fbValue：当前角速度反馈值
//	*         result：控制结果的存放位置
//	*					T：控制周期
//  * @retval None
//  */
//void TDT_Pitch_Mech_InnerControl(float* setValue, float* fbValue, float* result, float T)
//{
//	static pid pitchInnerMech;
//	
//	static u8 paraLoadFlag = 0;
//	
//	/* 如果参数没有加载，加载参数*/	
//	if(!paraLoadFlag)
//	{
//		TDT_Get_PIDparameters(&pitchInnerMech, pidPitchInnerMech);
//		paraLoadFlag = 1;
//	}
//    /* 设定值 */	
//		pitchInnerMech.setValue = *setValue;	
//		/* 反馈值 */			
//		pitchInnerMech.feedbackValue = *fbValue;	
//  	/* 偏差 = 设定值 - 反馈值 */
//		pitchInnerMech.error = pitchInnerMech.setValue - pitchInnerMech.feedbackValue;	
//		/* 比例项的输出 */			
//		pitchInnerMech.pOut = pitchInnerMech.kp * pitchInnerMech.error;
//  	/* 微分项的输出 */			
//		pitchInnerMech.dOut = pitchInnerMech.kd * (pitchInnerMech.error - pitchInnerMech.lastError);
//	  pitchInnerMech.lastError = pitchInnerMech.error;
//		/* 总的输出 = 比例项的输出*/			
//		pitchInnerMech.out = pitchInnerMech.pOut+pitchInnerMech.dOut;	

//		/* 总的输出不能超出电机给定值的范围 */	
//		*result = LIMIT(pitchInnerMech.out, -MAXSETDM50, MAXSETDM50);	
//}

///**
//  * @brief  云台pitch外环控制
//  * @param  setValue: 期望角度（度）
//	*         fbValue：当前角度反馈值
//	*         result：控制结果的存放位置
//	*					T：控制周期
//  * @retval None
//  */
//void TDT_Pitch_Mech_OuterControl(float* setValue, float* fbValue, float* result, float T)
//{
//	static pid pitchOuterMech;
//	static u8 paraLoadFlag = 0;
//	
//	/* 如果参数没有加载，加载参数*/	
//	if(!paraLoadFlag)
//	{
//		TDT_Get_PIDparameters(&pitchOuterMech, pidPitchOuterMech);
//		paraLoadFlag = 1;
//	}

//		/* 设定值 */	
//		pitchOuterMech.setValue = *setValue;		
//		/* 反馈值 */			
//		pitchOuterMech.feedbackValue = *fbValue;		
//		/* 偏差 = 设定值 - 反馈值 */
//		pitchOuterMech.error = pitchOuterMech.setValue - pitchOuterMech.feedbackValue;	
//		/* 偏差进行积分 */
//		pitchOuterMech.integralError += pitchOuterMech.error * T;		
//		/* 偏差的积分进行限制 */
//		pitchOuterMech.integralError = LIMIT(pitchOuterMech.integralError, -pitchOuterMech.integralErrorMax, pitchOuterMech.integralErrorMax);	
//		/* 比例项的输出 */
//		pitchOuterMech.pOut = pitchOuterMech.kp * pitchOuterMech.error;	
//		/* 积分项的输出 */
//		pitchOuterMech.iOut = pitchOuterMech.ki * pitchOuterMech.integralError;
//		/* 总的输出 = 比例项的输出 + 积分项输出 */
//		pitchOuterMech.out = pitchOuterMech.pOut + pitchOuterMech.iOut;
//		/* 内环的给定值在[-300,300]度每秒 */	
//		*result = -LIMIT(pitchOuterMech.out, -MAX_PITCH_ASPPED, MAX_PITCH_ASPPED);	
//}

///**
//  * @brief  云台pitch控制  (机械角控制)
//  * @param  T：控制周期
//  * @retval None
//  */
//void TDT_Pitch_Mech_Control(float T)
//{
//	float innerSetValue = 0, innerFbValue = 0, innerResult = 0;
//	float outerFbValue = 0;

//	static float outerSetValue , outerResult = 0;//6000
//  static float  pitchSetAngle=0;
//	static u8 outerControl = 0;
//	
//	float controlCycle = T;

//	/* 内环设定值等于外环输出值 */	
//	innerSetValue = outerResult;
//	/* 云台绕y轴旋转的陀螺仪数值作为内环角速度反馈值,逆时针为负，内环设定为负 */
//	innerFbValue =mpu6050Top.gyro.dps.data[y];
//	TDT_Pitch_Mech_InnerControl(&innerSetValue, &innerFbValue, &innerResult, controlCycle);

//	/* 外环的控制周期是内环的2倍 */
//	if(++outerControl >= 2)
//	{
//		outerControl = 0;
//		pitchSetAngle = Pitch_aimposition + (PITCH_MID - START_Position_Pitch) ;				 
//		/*有大符控制的增加给定角度*/
//		outerSetValue = pitchSetAngle;
//    /*6623的机械角度作为外环角度反馈*/		
//		outerFbValue = Position_485Pitch;
//		TDT_Pitch_Mech_OuterControl(&outerSetValue, &outerFbValue, &outerResult, 2*controlCycle);
//	}				
//	DM50_SPEED_Pitch1 = innerResult;		//
//}

/**
  * @brief  云台Yaw内环控制
  * @param  setValue: 期望角速度（度每秒）
	*         fbValue：当前角速度反馈值
	*         result：控制结果的存放位置
	*					T：控制周期
  * @retval None
  */
void TDT_Yaw_InnerControl(float* setValue, float* fbValue, float* result, float T)
{
	static pid yawInner;
	
	static u8 paraLoadFlag = 0;
	
	/* 如果参数没有加载，加载参数*/	
	if(!paraLoadFlag)
	{
		TDT_Get_PIDparameters(&yawInner, pidYawInner);
		paraLoadFlag = 1;
	}
    /* 设定值 */	
		yawInner.setValue = *setValue;	
		/* 反馈值 */			
		yawInner.feedbackValue = *fbValue;	
  	/* 偏差 = 设定值 - 反馈值 */
		yawInner.error = yawInner.setValue - yawInner.feedbackValue;	
		/* 比例项的输出 */			
		yawInner.pOut = yawInner.kp * yawInner.error;
		/* 微分项的输出 */			
		yawInner.dOut = yawInner.kd * (yawInner.error - yawInner.lastError);
	  yawInner.lastError = yawInner.error;
		/* 总的输出 = 比例项的输出*/			
		yawInner.out = yawInner.pOut+yawInner.dOut;	

		/* 总的输出不能超出电机给定值的范围 */	
		*result = LIMIT(yawInner.out, -MAXSETDM50, MAXSETDM50);	
}

/**
  * @brief  云台yaw外环控制
  * @param  setValue: 期望角度（度）
	*         fbValue：当前角度反馈值
	*         result：控制结果的存放位置
	*					T：控制周期
  * @retval None
  */
void TDT_Yaw_OuterControl(float* setValue, float* fbValue, float* result, float T)
{
	static pid yawOuter;
	static u8 paraLoadFlag = 0;
	
	/* 如果参数没有加载，加载参数*/	
	if(!paraLoadFlag)
	{
		TDT_Get_PIDparameters(&yawOuter, pidYawOuter);
		paraLoadFlag = 1;
	}

		/* 设定值 */	
		yawOuter.setValue = *setValue;		
		/* 反馈值 */			
		yawOuter.feedbackValue = *fbValue;		
		/* 偏差 = 设定值 - 反馈值 */
		yawOuter.error = yawOuter.setValue - yawOuter.feedbackValue;	
		/* 偏差进行积分 */
		yawOuter.integralError += yawOuter.error * T;		
		/* 偏差的积分进行限制 */
		yawOuter.integralError = LIMIT(yawOuter.integralError, -yawOuter.integralErrorMax, yawOuter.integralErrorMax);	
		/* 比例项的输出 */
		yawOuter.pOut = yawOuter.kp * yawOuter.error;	
		/* 积分项的输出 */
		yawOuter.iOut = yawOuter.ki * yawOuter.integralError;
		/* 总的输出 = 比例项的输出 + 积分项输出 */
		yawOuter.out = yawOuter.pOut + yawOuter.iOut;
		/* 内环的给定值在[-300,300]度每秒 */	
		*result = LIMIT(yawOuter.out, -MAX_PITCH_ASPPED, MAX_PITCH_ASPPED);	
}

/**
  * @brief  云台yaw控制
  * @param  T：控制周期
  * @retval None
  */
 void TDT_Yaw_Control(float T)
{
	float innerSetValue = 0, innerFbValue = 0, innerResult = 0;
	float outerFbValue = 0;
	static float outerSetValue , outerResult = 0;//6000
	
  static float  yawSetAngle=0;
	static u8 outerControl = 0;
	
	float controlCycle = T;

	/* 内环设定值等于外环输出值 */	
	innerSetValue = outerResult;
	/* 云台绕y轴旋转的陀螺仪数值作为内环角速度反馈值,逆时针为负，内环设定为负 */
	innerFbValue = -mpu6050Top.gyro.dps.data[z];
	TDT_Yaw_InnerControl(&innerSetValue, &innerFbValue, &innerResult, controlCycle);	//yaw轴内环pid控制

	/* 外环的控制周期是内环的2倍 */
	if(++outerControl >= 2)
	{
		outerControl = 0;
		yawSetAngle += RC.CH[0]*5.0/660;		//右摇杆横向作为yaw轴设定值
     /*限制yaw给定范围*/
		//yawSetAngle = LIMIT(yawSetAngle, -45, 45);
		 /*有大符控制的增加给定角度*/
		outerSetValue = yawSetAngle;
     /*6623的机械角度作为外环角度反馈*/	        
		outerFbValue =-gimbalTopAngle.yawsum;
		TDT_Yaw_OuterControl(&outerSetValue, &outerFbValue, &outerResult, 2*controlCycle);
	}				
	DM50_SPEED_Yaw = innerResult;			//内环结果给yaw，由485发送 
}

/*拨弹轮内环pid控制函数*/
void TDT_FeedSprocket_InnerControl(float* setValue, float* fbValue, float* result, float T)
{
	static pid feedSprocket_Inner;
	static u8 paraLoadFlag = 0;
	
	/* 如果参数没有加载，加载参数*/	
	if(!paraLoadFlag)
	{
		TDT_Get_PIDparameters(&feedSprocket_Inner, pidfeedSprocket_Inner);
		paraLoadFlag = 1;
	}
 		/* 设定值 */	
		feedSprocket_Inner.setValue = *setValue;	
		/* 反馈值 */			
		feedSprocket_Inner.feedbackValue = *fbValue;	
		/* 偏差 = 设定值 - 反馈值 */			
		feedSprocket_Inner.error = feedSprocket_Inner.setValue - feedSprocket_Inner.feedbackValue;
		/* 偏差增量 */			
		feedSprocket_Inner.deltaError = feedSprocket_Inner.error - feedSprocket_Inner.lastError;
		/* 比例项的输出 */			
		feedSprocket_Inner.pOut = feedSprocket_Inner.kp * feedSprocket_Inner.error;	
		/* 微分项的输出 */			
		feedSprocket_Inner.dOut = feedSprocket_Inner.kd * feedSprocket_Inner.deltaError;
		/* 总的输出 = 比例项的输出 */			
		feedSprocket_Inner.out = feedSprocket_Inner.pOut + feedSprocket_Inner.dOut;	
		/* 总的输出不能超出电机给定值的范围 */	
		*result = LIMIT(-feedSprocket_Inner.out, -15000, 15000);	
}

/*拨弹轮外环pid控制函数*/
void TDT_FeedSprocket_OuterControl(float* setValue, float* fbValue, float* result, float T)
{
	static pid feedSprocket_Outer;
	
	static u8 paraLoadFlag = 0;
	
	/* 如果参数没有加载，加载参数*/	
	if(!paraLoadFlag)
	{
		TDT_Get_PIDparameters(&feedSprocket_Outer, pidfeedSprocket_Outer);
		paraLoadFlag = 1;
	}

		/* 设定值 */	
		feedSprocket_Outer.setValue = *setValue;		
		/* 反馈值 */			
		feedSprocket_Outer.feedbackValue = *fbValue;		
		/* 偏差 = 设定值 - 反馈值 */
		feedSprocket_Outer.error = feedSprocket_Outer.setValue - feedSprocket_Outer.feedbackValue;	
		/* 偏差进行积分 */
		feedSprocket_Outer.integralError += feedSprocket_Outer.error * T;		
		/* 偏差的积分进行限制 */
		feedSprocket_Outer.integralError = LIMIT(feedSprocket_Outer.integralError, -feedSprocket_Outer.integralErrorMax, feedSprocket_Outer.integralErrorMax);	
		/* 比例项的输出 */
		feedSprocket_Outer.pOut = feedSprocket_Outer.kp * feedSprocket_Outer.error;	
		/* 积分项的输出 */
		feedSprocket_Outer.iOut = feedSprocket_Outer.ki * feedSprocket_Outer.integralError;		
		/* 总的输出 = 比例项的输出 + 积分项输出 */
		feedSprocket_Outer.out = feedSprocket_Outer.pOut + feedSprocket_Outer.iOut;	
		/* 内环的给定值在[-300,300]度每秒 */	
		*result = LIMIT(feedSprocket_Outer.out, -15000, 15000);	
}

/*拨弹轮控制函数*/
void TDT_FeedSprocket_Position_Control(float T)
{
	float innerSetValue = 0, innerFbValue = 0, innerResult = 0;
	float outerFbValue = 0;
	float controlCycle = T;
	static float outerSetValue = 0, outerResult = 0, rest_time = 0, free_time = 0.15f;
 
	static u8 shoot_error = 0;
	static u8 outerControl = 0;
	
	innerSetValue = outerResult;	//外环结果给内环设定值
	innerFbValue = can2_feedback.feedSprocket_speed;//内环反馈值由can2返回
	TDT_FeedSprocket_InnerControl(&innerSetValue, &innerFbValue, &innerResult, controlCycle);	//执行内环pid控制
	
	if((RC.CH[4] == 2 || RC.CH[8] ==1) && (shoot_error == 0))		//未堵弹，遥控器左开关在最下方，或点击鼠标左键时进行拨弹
	{
		outerSetValue += SINGLESHOOT;
	}
	
		/*堵转保护*/
	if(ABS(outerSetValue - can2_feedback.feedSprocket_position) > SINGLESHOOT*4)
	{
		outerSetValue -= SINGLESHOOT*6;		//电机反转
		shoot_error = 1;					//堵转标记
	}
	if(shoot_error == 1)				//等待反转结束
	{
		rest_time += T;
		if(rest_time > 0.8)
		{
			shoot_error = 0;
			rest_time = 0;
			outerSetValue = can2_feedback.feedSprocket_position;		
		}
	}
		/* 外环的控制周期是内环的2倍 */
	if(++outerControl >= 2)										//内环pid执行两次，外环pid执行1次
	{
		outerControl = 0;
		outerFbValue = can2_feedback.feedSprocket_position;
		TDT_FeedSprocket_OuterControl(&outerSetValue, &outerFbValue, &outerResult, 2*controlCycle);
	}				
	TDT_feedSprocket_OutUpdate(&innerResult);//拨弹轮发送控制函数
}


