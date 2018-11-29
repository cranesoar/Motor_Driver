#include "task_control.h"


static void Task_Valve(void);
static void bsp_relay_Init(void);


mouse mymouse;
u8 m1,m2,m3,m4;
u8 left_state,right_state;

void Task_Init(void)
{
	bsp_relay_Init();
	Right_Loosen;
	Left_Loosen;
	left_state=1;
	right_state=1;
	Right_valve_close;
	Left_valve_close;
}

static void bsp_relay_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE );
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);	
	
}
void TsskFlag_getvalue(void)
{
	if((RC.last_key.E==0&&RC.key.E==1)||(RC.mouse_last.left_jump==0&&RC.mouse.left_jump==1))
	{
		RC.flag.E=-RC.flag.E;
	}
	if((RC.last_key.Q==0&&RC.key.Q==1)||(RC.mouse_last.Right_jump==0&&RC.mouse.Right_jump==1))
	{
		RC.flag.Q=-RC.flag.Q;
	}
	if(RC.last_key.Z==0&&RC.key.Z==1)
	{
		RC.flag.Z=-RC.flag.Z;
	}
	if(RC.last_key.X==0&&RC.key.X==1)
	{
		RC.flag.X=-RC.flag.X;
	}
	
		RC.last_key.Q=RC.key.Q;
		RC.last_key.E=RC.key.E;
		RC.last_key.Z=RC.key.Z;
		RC.last_key.X=RC.key.X;
		RC.mouse_last.left_jump=RC.mouse.left_jump;
		RC.mouse_last.Right_jump=RC.mouse.Right_jump;

}

void Task_control(void)
{
	
	if(RC.flag.Z==1)
		{Left_valve_open;}
	else
		{Left_valve_close;}
		
	if(RC.flag.X==1)
		{Right_valve_open;}
	else
		{Right_valve_close;}
		
	if(RC.flag.Q==1)
		{Right_Loosen;}
	else
		{Right_Clamp;}
		
	if(RC.flag.E==1)
		{Left_Loosen;}
	else
		{Left_Clamp;}

}
int16_t PWM_left,PWM_right;
u8 m1=0,m2=0;
void Pump_Start(void)
{
	if(RC.flag.X==1)
//	if(m1==1)
	{
		if(PWM_left<6000)
		PWM_left+=10;
		PWM_left=LIMIT(PWM_left,4000,6000);
		TIM3->CCR1=PWM_left;
	}
	else {TIM3->CCR1=4000;PWM_left=4000;}
	if(RC.flag.Z==1)
//	if(m2==1)
	{
		if(PWM_right<6000)
		PWM_right+=10;
		PWM_right=LIMIT(PWM_right,4000,6000);
		TIM3->CCR2=PWM_right;
	}
	else {TIM3->CCR2=4000;PWM_right=4000;}
}


static void Task_Valve(void)
{
	
}

