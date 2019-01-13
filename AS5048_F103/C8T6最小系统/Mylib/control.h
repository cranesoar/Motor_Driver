#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "main.h"

typedef struct _vec2int16
{
    int16_t data[2];
} vec2int16;

/**
* @struct  _pid
* @brief pid½á¹¹Ìå
*/
typedef struct _pid
{
    float setValue;
    float feedbackValue;
    float error;
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
} pid;
void TDT_Friction_PidControl(vec2int16* setValue, vec2int16* fbValue, vec2int16* result, float T, u8 dimConVar);

#endif
