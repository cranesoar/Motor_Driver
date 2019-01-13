#include "main.h"

void TDT_Get_PIDparameters(pid* pidStruct, u8 pidIndex)
{
    switch(pidIndex)
    {
    case pidFriction:
    {
        pidStruct->kp = 0.0f;
        pidStruct->ki = 0.2;//3.5;
        pidStruct->integralErrorMax = 4000;
        break;
    }

    default:
        break;
    }
}






