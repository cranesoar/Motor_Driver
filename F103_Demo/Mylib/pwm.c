#include "main.h"

/*******************************************************************************
* 函 数 名         : TIM2_pwm_init
* 函数功能		     : IO端口及TIM2初始化函数	   
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void TIM2_pwm_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;   //声明一个结构体变量，用来初始化GPIO

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//声明一个结构体变量，用来初始化定时器

	TIM_OCInitTypeDef TIM_OCInitStructure;//根据TIM_OCInitStruct中指定的参数初始化外设TIMx

	/* 开启时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_Init(GPIOA,&GPIO_InitStructure);


	//TIM2定时器初始化
	TIM_TimeBaseInitStructure.TIM_Period =1000-1;	   //不分频,PWM 频率=72000/900=8Khz//设置自动重装载寄存器周期的值
	TIM_TimeBaseInitStructure.TIM_Prescaler =1440-1 ;//设置用来作为TIMx时钟频率预分频值，100Khz计数频率
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM向上计数模式
	TIM_TimeBaseInit(TIM2, & TIM_TimeBaseInitStructure);

	//PWM初始化	  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;//PWM输出使能
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_Low;

	TIM_OC1Init(TIM2,&TIM_OCInitStructure);
  TIM_OC2Init(TIM2,&TIM_OCInitStructure);
  TIM_OC3Init(TIM2,&TIM_OCInitStructure);
	TIM_OC4Init(TIM2,&TIM_OCInitStructure);
	//注意此处初始化时TIM_OC[x]Init而不是TIM_OCInit，否则会出错。因为固件库的版本不一样。

  TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);//使能或者失能TIMx在CCR1上的预装载寄存器
  TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);//使能或者失能TIMx在CCR2上的预装载寄存器
  TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);//使能或者失能TIMx在CCR3上的预装载寄存器
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);//使能或者失能TIMx在CCR4上的预装载寄存器
	
	TIM_Cmd(TIM2,ENABLE);//使能或者失能TIMx外设
}



/*******************************************************************************
* 函 数 名         : TIM3_pwm_init
* 函数功能		     : IO端口及TIM3初始化函数	   
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void TIM3_pwm_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;   //声明一个结构体变量，用来初始化GPIO

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//声明一个结构体变量，用来初始化定时器

	TIM_OCInitTypeDef TIM_OCInitStructure;//根据TIM_OCInitStruct中指定的参数初始化外设TIMx

	/* 开启时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_Init(GPIOA,&GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_Init(GPIOB,&GPIO_InitStructure);


	//TIM3定时器初始化
	TIM_TimeBaseInitStructure.TIM_Period =1000-1;	   //不分频,PWM 频率=72000/900=8Khz//设置自动重装载寄存器周期的值
	TIM_TimeBaseInitStructure.TIM_Prescaler =1440-1 ;//设置用来作为TIMx时钟频率预分频值，100Khz计数频率
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM向上计数模式
	TIM_TimeBaseInit(TIM3, & TIM_TimeBaseInitStructure);

	//PWM初始化	  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;//PWM输出使能
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_Low;

	TIM_OC1Init(TIM3,&TIM_OCInitStructure);
  TIM_OC2Init(TIM3,&TIM_OCInitStructure);
  TIM_OC3Init(TIM3,&TIM_OCInitStructure);
	TIM_OC4Init(TIM3,&TIM_OCInitStructure);
	//注意此处初始化时TIM_OC[x]Init而不是TIM_OCInit，否则会出错。因为固件库的版本不一样。

  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);//使能或者失能TIMx在CCR1上的预装载寄存器
  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);//使能或者失能TIMx在CCR2上的预装载寄存器
  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);//使能或者失能TIMx在CCR3上的预装载寄存器
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);//使能或者失能TIMx在CCR4上的预装载寄存器
	
	TIM_Cmd(TIM3,ENABLE);//使能或者失能TIMx外设
}

/*******************************************************************************
* 函 数 名         : TIM4_pwm_init
* 函数功能		     : IO端口及TIM4初始化函数	   
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void TIM4_pwm_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;   //声明一个结构体变量，用来初始化GPIO

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//声明一个结构体变量，用来初始化定时器

	TIM_OCInitTypeDef TIM_OCInitStructure;//根据TIM_OCInitStruct中指定的参数初始化外设TIMx

	/* 开启时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_Init(GPIOB,&GPIO_InitStructure);


	//TIM2定时器初始化
	TIM_TimeBaseInitStructure.TIM_Period =1000-1;	   //不分频,PWM 频率=72000/900=8Khz//设置自动重装载寄存器周期的值
	TIM_TimeBaseInitStructure.TIM_Prescaler =1440-1 ;//设置用来作为TIMx时钟频率预分频值，100Khz计数频率
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM向上计数模式
	TIM_TimeBaseInit(TIM4, & TIM_TimeBaseInitStructure);

	//PWM初始化	  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;//PWM输出使能
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_Low;

	TIM_OC1Init(TIM4,&TIM_OCInitStructure);
  TIM_OC2Init(TIM4,&TIM_OCInitStructure);
  TIM_OC3Init(TIM4,&TIM_OCInitStructure);
	TIM_OC4Init(TIM4,&TIM_OCInitStructure);
	//注意此处初始化时TIM_OC[x]Init而不是TIM_OCInit，否则会出错。因为固件库的版本不一样。

  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);//使能或者失能TIMx在CCR1上的预装载寄存器
  TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);//使能或者失能TIMx在CCR2上的预装载寄存器
  TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);//使能或者失能TIMx在CCR3上的预装载寄存器
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);//使能或者失能TIMx在CCR4上的预装载寄存器
	
	TIM_Cmd(TIM4,ENABLE);//使能或者失能TIMx外设
}


void PWM_OUT(int A0 ,int A1 ,int A2 , int A3 , int A6 , int A7 , int B0 , int B1 , int B6 , int B7 , int B8 , int B9 )   //赋值0-1000  占空比0-100%
{
  TIM_SetCompare1(TIM2,A0);
  TIM_SetCompare2(TIM2,A1);  
  TIM_SetCompare3(TIM2,A2);
  TIM_SetCompare4(TIM2,A3);
  
  TIM_SetCompare1(TIM3,A6);
  TIM_SetCompare2(TIM3,A7);  
  TIM_SetCompare3(TIM3,B0);
  TIM_SetCompare4(TIM3,B1);  
  
  TIM_SetCompare1(TIM4,B6);
  TIM_SetCompare2(TIM4,B7);  
  TIM_SetCompare3(TIM4,B8);
  TIM_SetCompare4(TIM4,B9);   
}

