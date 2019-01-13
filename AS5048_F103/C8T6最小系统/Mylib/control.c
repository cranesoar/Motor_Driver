#include "main.h"

/**
  * @brief  摩擦轮pid控制
  * @param  setValue: 速度设定值
	*					fbValue: 速度反馈值
	*					result: 控制器输出
	*					T: 控制周期
	*					numConVar: 被控变量的个数
  * @retval None
  */

void TDT_Friction_PidControl(vec2int16* setValue, vec2int16* fbValue, vec2int16* result, float T, u8 dimConVar)
{
    static pid frictionMotor[2];

    static u8 paraLoadFlag = 0;

    u8 indexConVar;

    /* 如果参数没有加载，加载参数，底盘四个电机PID参数默认相同 */
    if(!paraLoadFlag)
    {
        for(indexConVar = 0; indexConVar < dimConVar; indexConVar++)
            TDT_Get_PIDparameters(&frictionMotor[indexConVar], pidFriction);
        paraLoadFlag = 1;
    }

    /* 循环dimConVar次，分别进行PID控制 */
    for(indexConVar = 0; indexConVar < dimConVar; indexConVar++)
    {
        /* 设定值 */
        frictionMotor[indexConVar].setValue = setValue->data[indexConVar];
        /* 反馈值 */
        frictionMotor[indexConVar].feedbackValue = fbValue->data[indexConVar];
        /* 偏差 = 设定值 - 反馈值 */
        frictionMotor[indexConVar].error = frictionMotor[indexConVar].setValue - frictionMotor[indexConVar].feedbackValue;
        /* 偏差进行积分 */
        frictionMotor[indexConVar].integralError += frictionMotor[indexConVar].error * T;
        /* 偏差的积分进行限制 */
        frictionMotor[indexConVar].integralError = LIMIT(frictionMotor[indexConVar].integralError, -frictionMotor[indexConVar].integralErrorMax, frictionMotor[indexConVar].integralErrorMax);
        /* 比例项的输出 */
        frictionMotor[indexConVar].pOut = frictionMotor[indexConVar].kp * frictionMotor[indexConVar].error;
        /* 积分项的输出 */
        frictionMotor[indexConVar].iOut = frictionMotor[indexConVar].ki * frictionMotor[indexConVar].integralError;
        /* 总的输出 = 比例项的输出 + 积分项的输出 */
        frictionMotor[indexConVar].out = frictionMotor[indexConVar].pOut + frictionMotor[indexConVar].iOut + 4000;
        /* 总的输出不能超出电机给定值的范围 */
        result->data[indexConVar] = LIMIT(frictionMotor[indexConVar].out, 4000, 7500);
    }
}


