#include "main.h"

void Load_Parameters(void)
{
	//电机位置pid
	position_pid.kp = 10;
	position_pid.ki = 0;
	position_pid.i_max = 20;
	position_pid.sampleTime = 0.01;
	
	//发弹模式
	runFlag.shootMode = burstsMode5;
	runFlag.beatTime = beatTimeMode5;
	runFlag.bulletFired = burstsMode5;
}
void TDT_Get_PIDparameters(pid* pidStruct, u8 pidIndex)
{
	switch(pidIndex)
	{
		case pidSPEED:
		{
			pidStruct->kp = 5;//0.6
			pidStruct->ki = 0.75;//2
			pidStruct->kd = 0;
			pidStruct->integralErrorMax = 5000;
			break;
		}
		case PID_POSTION:
		{
			pidStruct->kp = 0.01;//0.6
			pidStruct->ki = 0.02;//0.005
			pidStruct->integralErrorMax = 100;
			break;
		}
		default:
			break;
	}
}


u16 BLDC_HALL_POS[66]=   //0-3611
{
3532	,
3472	,
3429	,
3365	,
3311	,
3263	,
3199	,
3150	,
3098	,
3035	,
2984	,
2931	,
2872	,
2816	,
2767	,
2712	,
2650	,
2600	,
2553	,
2486	,
2438	,
2392	,
2324	,
2278	,
2234	,
2161	,
2118	,
2064	,
1998	,
1960	,
1897	,
1835	,
1799	,
1733	,
1675	,
1636	,
1571	,
1517	,
1468	,
1408	,
1359	,
1305	,
1243	,
1196	,
1142	,
1085	,
1029	,
979	,
926	,
862	,
816	,
768	,
701	,
653	,
610	,
538	,
493	,
443	,
375	,
333	,
280	,
211	,
173	,
110	,
45	,
10
};

u8 BLDC_POS2STEP[66]=
{
6	,
1	,
2	,
3	,
4	,
5	,
6	,
1	,
2	,
3	,
4	,
5	,
6	,
1	,
2	,
3	,
4	,
5	,
6	,
1	,
2	,
3	,
4	,
5	,
6	,
1	,
2	,
3	,
4	,
5	,
6	,
1	,
2	,
3	,
4	,
5	,
6	,
1	,
2	,
3	,
4	,
5	,
6	,
1	,
2	,
3	,
4	,
5	,
6	,
1	,
2	,
3	,
4	,
5	,
6	,
1	,
2	,
3	,
4	,
5	,
6	,
1	,
2	,
3	,
4	,
5	
};
