#include "main.h"
int BLDC_POSTION=0; 
int temp_max =0;
int temp_min =6000;
int BLDC_step=1;
int BLDC_pwm=500;
int BLDC_flag=0;
_Bool BLDC_DIR_FLG=0;  
int BLDC_POS_DIFF=0;;
//     A    B    C
//     U    V    W

// 1   UV
// 2   UW
// 3   VW
// 4   VU
// 5   WU
// 6   WV

//UW
//WV
//UV
//VU
//VW
//WU
 

// 172	110	46	10	3532	3472	3429	3364	3310	3263	3199	
//
int main(void)
{
		static u32 BLDC_POSTION_last=0;
		static int BLDC_flag_last=0;
		PWM_Configuration();		  //pwm配置
	
		INH_Init();
	
		Load_Parameters();				//加载参数

		TIM3_Cap_Init(0XFFFF,3-1);	//以1Mhz的频率计数 
	
		KEY_Configuration();

		SysTick_Init();						//滴答定时器初始化
	
		LPF2pSetCutoffFreq_1(1000,25);
	
	
    while(1)
    {
				runFlag.shootFlag = 1;
				if(TIM3CH3_CAPTURE_STA&0X80)//成功捕获到了一次上升沿
				{
					BLDC_POSTION=TIM3CH3_CAPTURE_STA&0X3F;
					BLDC_POSTION*=65536;//溢出时间总和
					BLDC_POSTION+=TIM3CH3_CAPTURE_VAL;//得到总的高电平时间
					TIM3CH3_CAPTURE_STA=0;//开启下一次捕获
					if(BLDC_POSTION>24000)
						BLDC_POSTION=BLDC_POSTION_last;
					BLDC_POS_DIFF = BLDC_POSTION-BLDC_POSTION_last;
					if(BLDC_POSTION>temp_max)
						temp_max=BLDC_POSTION;
					if(BLDC_POSTION<temp_min)
						temp_min=BLDC_POSTION;
					BLDC_POSTION_last=BLDC_POSTION;
			}
				
			BLDC_flag=Commutation(BLDC_POSTION);
			if(BLDC_flag ==-1)
				BLDC_flag=BLDC_flag_last;
			BLDC_flag_last=BLDC_flag;
			if(BLDC_flag!=0)
				BLDC_step=BLDC_flag;
				switch(BLDC_step)
				{
					case 0:                //停止
						INH_U_OFF;
						INH_V_OFF;
						INH_W_OFF;
						BLDC_U(0);
						BLDC_V(0);
						BLDC_W(0);
						break;
					
					case 1:									//UV
						BLDC_U(BLDC_pwm);
						BLDC_V(0);
						BLDC_W(0);
						INH_U_ON;
						INH_V_ON;
						INH_W_OFF;
						break;
					
					case 2:									//UW
						BLDC_U(BLDC_pwm);
						BLDC_V(0);
						BLDC_W(0);
						INH_U_ON;
						INH_V_OFF;
						INH_W_ON;
						break;
					
					case 3:									//VW
						BLDC_U(0);
						BLDC_V(BLDC_pwm);
						BLDC_W(0);
						INH_U_OFF;
						INH_V_ON;
						INH_W_ON;
						break;
					
					case 4:									//VU
						BLDC_U(0);
						BLDC_V(BLDC_pwm);
						BLDC_W(0);
						INH_U_ON;
						INH_V_ON;
						INH_W_OFF;
						break;
					
					case 5:									//WU
						BLDC_U(0);
						BLDC_V(0);
						BLDC_W(BLDC_pwm);
						INH_U_ON;
						INH_V_OFF;
						INH_W_ON;
						break;
	
					case 6:									//WV
						BLDC_U(0);
						BLDC_V(0);
						BLDC_W(BLDC_pwm);
						INH_U_OFF;
						INH_V_ON;
						INH_W_ON;
						break;
				}
				
				

    }
}




//				switch(BLDC_step)
//				{
//					case 0:                //停止
//						INH_U_OFF;
//						INH_V_OFF;
//						INH_W_OFF;
//						BLDC_U(0);
//						BLDC_V(0);
//						BLDC_W(0);
//						break;
//					
//					case 1:									//UW
//						BLDC_U(BLDC_pwm);
//						BLDC_V(0);
//						BLDC_W(0);
//						INH_U_ON;
//						INH_V_OFF;
//						INH_W_ON;
//						break;
//					
//					case 2:									//WV
//						BLDC_U(0);
//						BLDC_V(0);
//						BLDC_W(BLDC_pwm);
//						INH_U_OFF;
//						INH_V_ON;
//						INH_W_ON;
//						break;
//					
//					case 3:									//UV
//						BLDC_U(BLDC_pwm);
//						BLDC_V(0);
//						BLDC_W(0);
//						INH_U_ON;
//						INH_V_ON;
//						INH_W_OFF;
//						break;
//					
//					case 4:									//VU
//						BLDC_U(0);
//						BLDC_V(BLDC_pwm);
//						BLDC_W(0);
//						INH_U_ON;
//						INH_V_ON;
//						INH_W_OFF;
//						break;
//					
//					case 5:									//VW
//						BLDC_U(0);
//						BLDC_V(BLDC_pwm);
//						BLDC_W(0);
//						INH_U_OFF;
//						INH_V_ON;
//						INH_W_ON;
//						break;
//	
//					case 6:									//WU
//						BLDC_U(0);
//						BLDC_V(0);
//						BLDC_W(BLDC_pwm);
//						INH_U_ON;
//						INH_V_OFF;
//						INH_W_ON;
//						break;
//				}
