#include "main.h"

_position_pid position_pid;
_run_flag runFlag;
float BLDC_set_speed=100;

int aimPosition[5] = {0,256,512,768,1024};

void Position_Control(u8 index)
{
	/*期望位置与反馈位置做偏差*/
	position_pid.exp = aimPosition[index];
	position_pid.feedback = encoder.position;
	position_pid.err = position_pid.exp - position_pid.feedback;

	/*P输出*/	
	position_pid.err = contain( position_pid.err, -640, 640);
	position_pid.p_out = position_pid.kp * position_pid.err;
	
	/*I输出*/	
	position_pid.err_inc += (float)position_pid.err * position_pid.sampleTime;
	position_pid.err_inc = limit(position_pid.err_inc, -position_pid.i_max, position_pid.i_max);
	position_pid.i_out = position_pid.ki * position_pid.err_inc;
	
	/*总输出*/
	position_pid.out = position_pid.p_out + position_pid.i_out;
}

//void TDT_bldc_InnerControl(float* setValue, float fbValue, float* result, float T)
//{
//	static pid bldcInner;
//	
//	static u8 paraLoadFlag = 0;
//	
//	/* 如果参数没有加载，加载参数*/	
//	if(!paraLoadFlag)
//	{
//		TDT_Get_PIDparameters(&bldcInner, pidSPEED);
//		paraLoadFlag = 1;
//	}
//		/* 设定值 */	
//		bldcInner.setValue = *setValue;		
//		/* 反馈值 */			
//		bldcInner.feedbackValue = fbValue;		
//		/* 偏差 = 设定值 - 反馈值 */
//		bldcInner.error = bldcInner.setValue - bldcInner.feedbackValue;	
//		
//		/* 偏差进行积分 */
//		bldcInner.integralError += bldcInner.error*T;		
//		/* 偏差的积分进行限制 */
//		bldcInner.integralError = LIMIT(bldcInner.integralError, -bldcInner.integralErrorMax, bldcInner.integralErrorMax);	
//		/* 比例项的输出 */
//		bldcInner.pOut = bldcInner.kp * bldcInner.error;	
//		/* 积分项的输出 */
//		bldcInner.iOut = bldcInner.ki * bldcInner.integralError;	
//		/* 总的输出 = 比例项的输出 + 积分项输出 */
//		bldcInner.out = bldcInner.pOut + bldcInner.iOut;	
//	  /* 误差传递 */
//	  bldcInner.lastError = bldcInner.error ;
//		/* 总的输出不能超出电机给定值的范围 */	
//		*result = LIMIT(bldcInner.out, -MAX_PWM_BLDC, MAX_PWM_BLDC);	
//}

void TDT_pos_OuterControl(float* setValue, float* fbValue, float* result, float T)
{
	static pid posOuter;
	
	static u8 paraLoadFlag = 0;
	
	/* 如果参数没有加载，加载参数*/	
	if(!paraLoadFlag)
	{
		TDT_Get_PIDparameters(&posOuter, PID_POSTION);
		paraLoadFlag = 1;
	}

		/* 设定值 */	
		posOuter.setValue = *setValue;		
		/* 反馈值 */			
		posOuter.feedbackValue = *fbValue;		
		/* 偏差 = 设定值 - 反馈值 */
		posOuter.error = posOuter.setValue - posOuter.feedbackValue;	
		/* 偏差进行积分 */
		posOuter.integralError += posOuter.error * T;		
		/* 偏差的积分进行限制 */
		posOuter.integralError = LIMIT(posOuter.integralError, -posOuter.integralErrorMax, posOuter.integralErrorMax);	
		/* 比例项的输出 */
		posOuter.pOut = posOuter.kp * posOuter.error;	
		/* 积分项的输出 */
		posOuter.iOut = posOuter.ki * posOuter.integralError;
		/* 总的输出 = 比例项的输出 + 积分项输出 */
		posOuter.out = posOuter.pOut + posOuter.iOut;
		/* 内环的给定值在[-300,300]度每秒 */	
		*result = LIMIT(posOuter.out, -200, 200);	
}

int set_postion=4000;
int set_speed_scop=0;
void BLDC_Control(float T)
{
	float my_result,fbvalue;
	static u8 outerControl = 0;
	float outerFbValue = 0;
	static float outerSetValue = 0, outerResult = 0;
	
	fbvalue = BLDC_SPEED_FILT;
	outerSetValue = set_postion;
	set_speed_scop = BLDC_set_speed;
	TDT_bldc_InnerControl(&BLDC_set_speed,&fbvalue,&my_result,T);
	if(my_result>0)
		BLDC_DIR_FLG=0;
	else
	{
		BLDC_DIR_FLG=1;
		my_result=-my_result;
	}
	BLDC_pwm=my_result;
	
	if(++outerControl >= 5)
	{
		outerControl = 0;
		outerFbValue = BLDC_POSTION;
		TDT_pos_OuterControl(&outerSetValue,&outerFbValue,&outerResult,T);
		BLDC_set_speed = outerResult;
	}

}


void TDT_bldc_InnerControl(float* setValue, float* fbValue, float* result, float T)
{
	static pid bldcInner;
	float ep,ei,ed;
	
	static u8 paraLoadFlag = 0;
	
	/* 如果参数没有加载，加载参数*/	
	if(!paraLoadFlag)
	{
		TDT_Get_PIDparameters(&bldcInner, pidSPEED);
		paraLoadFlag = 1;
	}
		/* 设定值 */	
		bldcInner.setValue = *setValue;		
		/* 反馈值 */			
		bldcInner.feedbackValue = *fbValue;		
		/* 偏差 = 设定值 - 反馈值 */
		bldcInner.error_0 = bldcInner.setValue - bldcInner.feedbackValue;	
		
		ep = bldcInner.error_0 - bldcInner.error_1;
	
		ei = bldcInner.error_0;
	
	  ed = bldcInner.error_0 - 2.0*bldcInner.error_1 + bldcInner.error_2;
	
		/* 比例项的输出 */
		bldcInner.pOut = bldcInner.kp * ep;	
		/* 积分项的输出 */
		bldcInner.iOut = bldcInner.ki * ei;
		/* 微分项的输出 */
		bldcInner.dOut = bldcInner.kd * ed;
		/* 总的输出 = 比例项的输出 + 积分项输出 */
		bldcInner.out += bldcInner.pOut + bldcInner.iOut + bldcInner.dOut; 	
		
  	bldcInner.out = LIMIT(bldcInner.out,-bldcInner.integralErrorMax,bldcInner.integralErrorMax); 	
	  /* 误差传递 */
	  bldcInner.error_2 = bldcInner.error_1;
		bldcInner.error_1 = bldcInner.error_0;
		/* 总的输出不能超出电机给定值的范围 */	
		*result = LIMIT(bldcInner.out, -MAX_PWM_BLDC, MAX_PWM_BLDC);	
}

