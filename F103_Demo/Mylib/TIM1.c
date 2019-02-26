#include "TIM1.h"


void TIM1_Init(u16  arr,  u16 psc)
{
 	GPIO_InitTypeDef GPIO_InitStructure;   //声明一个结构体变量，用来初始化GPIO
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//声明一个结构体变量，用来初始化定时器
	TIM_OCInitTypeDef TIM_OCInitStructure;//根据TIM_OCInitStruct中指定的参数初始化外设TIMx 
  TIM_BDTRInitTypeDef TIM_BDTRStructure;//主动刹车
     
	/* 开启时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_TIM1,ENABLE);
 
	/*  配置TIM1的GPIO的模式和IO口 */
	GPIO_InitStructure .GPIO_Pin=GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_Init(GPIOA,&GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_Init(GPIOB,&GPIO_InitStructure);

  //重映射
  GPIO_AFIODeInit();
  GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1 , ENABLE); 

	//TIM1定时器初始化
  TIM_DeInit(TIM1);
	TIM_TimeBaseInitStructure.TIM_Period =arr;	   //不分频,PWM 频率=72000/4500=16Khz
	TIM_TimeBaseInitStructure.TIM_Prescaler =psc ;//设置用来作为TIMx时钟频率预分频值，100Khz计数频率
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1;	//TIM向上计数模式
  TIM_TimeBaseInitStructure.TIM_RepetitionCounter =1;//周期计数器值 
  TIM_ARRPreloadConfig(TIM1,ENABLE); ////设置自动重装载寄存器
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);
 
  TIM_SelectSlaveMode(TIM1,TIM_SlaveMode_Trigger);
  TIM_SelectInputTrigger(TIM1,TIM_TS_ITR1);
  
  TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_OC4Ref);  
  TIM_SelectMasterSlaveMode(TIM1,TIM_MasterSlaveMode_Disable);

	//PWM初始化	  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
  TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset;//当MOE=0重置输出比较空闲状态
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCNIdleState_Reset;//当MOE=0重置输出比较N空闲状
  TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCNPolarity_High;// //互补输出比较极性高
  TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//输出比较极性高
  TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;//使能输出比较状态
  TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Enable;//使能互补输出比较状态
  TIM_OCInitStructure.TIM_Pulse=0;//待装入捕获比较寄存器的脉冲值
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);
  TIM_OC2Init(TIM1,&TIM_OCInitStructure);
  TIM_OC3Init(TIM1,&TIM_OCInitStructure);
  
  //OC4为触发ADC1注入通道配置
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;                  
  TIM_OCInitStructure.TIM_Pulse =((PWM_PERIOD_CYCLES) / 2) - (HTMIN);                    
  TIM_OC4Init(TIM1, &TIM_OCInitStructure); 
  
  //PWM使能
  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);//使能或者失能TIMx在CCR1上的预装载寄存器
  TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);//使能或者失能TIMx在CCR2上的预装载寄存器
  TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);//使能或者失能TIMx在CCR3上的预装载寄存器
  TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);//使能或者失能TIMx在CCR4上的预装载寄存器
  
  //刹车死区配置
  TIM_BDTRStructure.TIM_AutomaticOutput=TIM_AutomaticOutput_Disable;// 自动输出功能使能
  TIM_BDTRStructure.TIM_Break=TIM_Break_Disable;//失能刹车输入
  TIM_BDTRStructure.TIM_BreakPolarity=TIM_BreakPolarity_Low; //刹车输入管脚极性高
  TIM_BDTRStructure.TIM_DeadTime=28; //输出打开和关闭状态之间的延时     =28/72Mhz=0.3864us
  TIM_BDTRStructure.TIM_LOCKLevel=TIM_LOCKLevel_1;// 锁电平参数: 不锁任何位
  TIM_BDTRStructure.TIM_OSSIState=TIM_OSSIState_Enable; //设置在运行模式下非工作状态选项
  TIM_BDTRStructure.TIM_OSSRState=TIM_OSSRState_Enable; //设置在运行模式下非工作状态选项
  TIM_BDTRConfig(TIM1,&TIM_BDTRStructure);
	

  //enable interrupt
  TIM_ITConfig(TIM1, TIM_IT_CC4, ENABLE);//CCR4的中断，这个通过设置CCR4的pulse来控制产生中断相当于PWM-ON的位置
  TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
  TIM_ITConfig(TIM1, TIM_IT_Break,ENABLE);

  TIM_SelectOutputTrigger(TIM1,TIM_TRGOSource_OC4Ref);
  TIM_SelectOutputTrigger(TIM1,TIM_TRGOSource_Update);
  /* Main Output Enable */
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
  TIM_GenerateEvent(TIM1,TIM_EventSource_CC4);   
  TIM_GenerateEvent(TIM1,TIM_EventSource_Update); 
  
    /* TIM1 counter enable */
  TIM_Cmd(TIM1, ENABLE);
}


void TIM1_UP_IRQHandler(void)							//
{
    if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
    if(TIM_GetITStatus(TIM1, TIM_IT_CC4) != RESET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_CC4);
    }
    if(TIM_GetITStatus(TIM1, TIM_IT_COM) != RESET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_COM);
    }
    
     
}
