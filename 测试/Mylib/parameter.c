#include "parameter.h"

float pp = 0;
float dd = 0,PPO=0,IIO=0,IIIO=0;

void TDT_Get_PIDparameters(pid* pidStruct, u8 pidIndex)
{
	switch(pidIndex)
	{
		case pidChassisMove:
		{
			pidStruct->kp = 0.8f;  //0.8f
			pidStruct->ki = 40;    //12
			pidStruct->kd = 0.02;  //0.02
			pidStruct->integralErrorMax = 1000;
			break;	
		}
	
		case pidChassisRotate:
		{
			pidStruct->kp = 1.2;
			break;
		}
		
		case pidPosition_3510Inner:
		{
			pidStruct->kp =1.8f;//20
			pidStruct->ki = 0;
			pidStruct->integralErrorMax = 0;
			break;
		}
		
		case pidPosition_3510Outer:
		{
			pidStruct->kp = 0.2;//0.5
			pidStruct->ki = 0.2;//0.2
			pidStruct->integralErrorMax = 500;
			break;
		}
		
		case pidPitchInner:
		{
			pidStruct->kp = 1.9;//0.7; //0.7
			pidStruct->kd = 0;//1.8;//1.6;//0.8
			break;
		}
		
		case pidPitchOuter:
		{
			pidStruct->kp = 20;//65;//65;  //70
			pidStruct->ki = 0;//230;//200;  //200
			pidStruct->integralErrorMax = 0;//0.8;//0.8;  //0.8
//			pidStruct->integralError = 0;
			break;
		}
		
		case pidPitchInnerMech:
		{
			pidStruct->kp = 0.9;//0.7; //0.7
			pidStruct->kd = 1.9;//1.6;//0.8
			break;
		}
		
		case pidPitchOuterMech:
		{
			pidStruct->kp = 0.4;//65;  //70
			pidStruct->ki = 50;//200;  //200
			pidStruct->integralErrorMax = 1;//0.8;  //0.8
//			pidStruct->integralError = 0;
			break;
		}
		case pidYawInner:
		{
			pidStruct->kp = 6;//1; //1
			pidStruct->kd = 0;//3; //5
			break;
		}
		
		case pidYawOuter:
		{
			pidStruct->kp = 14;//0.4;  //0.4
			pidStruct->ki = 0;//28;  //30
			pidStruct->integralErrorMax = 0;//2;  //2
//			pidStruct->integralError = 0;
			break;
		}
		
		case pidWhiffInner:
		{
			pidStruct->kp =0.6;//0.6
			pidStruct->kd =1.1;//1.1
			pidStruct->integralErrorMax = 0;
			break;
		}
		
		case pidWhiffOuter:
		{
			pidStruct->kp = 0.5;//0.5
			pidStruct->ki = 20;//20
			pidStruct->integralErrorMax = 4;//4
			break;
		}
		
		case pidfeedSprocket_Inner:
		{
			pidStruct->kp = 2.0;//1.8;//0.6
			pidStruct->kd = 0;
			break;
		}
		
		case pidfeedSprocket_Outer:
		{
			pidStruct->kp = 0.1;//0.05;//10;//10;//0.6
			pidStruct->ki = 0;//0.25;//25;//1
			pidStruct->integralErrorMax = 6;
			break;
		}
		default:
			break;

			
	}
}

void Struct_Init()
{
	RC.flag.Q=-1;
	RC.flag.E=-1;
  RC.flag.F=-1;
	RC.flag.Z=-1;
	RC.flag.X=-1;
	RC.flag.C=-1;
	RC.flag.V=-1;
	RC.flag.B=-1;
	RC.flag.G=-1;
	RC.flag.R=-1;
	RC.flag.left_jump=-1;
	RC.flag.Right_jump=-1;
}
