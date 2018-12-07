#include "TIM1.h"

void TIM1_ADC_Init(u16 arr, u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStruct;
    TIM_BDTRInitTypeDef TIM_BDTRStructure;
    ADC_InitTypeDef ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;    
  
    ADC_DeInit();
  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1|RCC_APB2Periph_ADC1,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB,ENABLE);
  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10; //TIM1_CH1,2,3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;//TIM1_CHN1,2,3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;     
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA,&GPIO_InitStructure); 
           
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_TIM1);       
  
    TIM_DeInit(TIM1);
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; //时钟分割
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_CenterAligned1;// 向上计数模式
    TIM_TimeBaseInitStructure.TIM_Period=arr; //自动重装载寄存器周期的值
    TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //预分频值
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter=1; //周期计数器值
    TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);
        
    //PWM配置
    TIM_OCInitStruct.TIM_OCIdleState=TIM_OCIdleState_Reset;//当MOE=0重置输出比较空闲状态
    TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1; //脉冲宽度调制模式1
    TIM_OCInitStruct.TIM_OCNIdleState=TIM_OCNIdleState_Reset;//当MOE=0重置输出比较N空闲状
    TIM_OCInitStruct.TIM_OCNPolarity=TIM_OCNPolarity_High;// //互补输出比较极性高
    TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;//输出比较极性高
    TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;//使能输出比较状态
    TIM_OCInitStruct.TIM_OutputNState=TIM_OutputNState_Enable;//使能互补输出比较状态
    TIM_OCInitStruct.TIM_Pulse=0;//待装入捕获比较寄存器的脉冲值
    TIM_OC1Init(TIM1,&TIM_OCInitStruct);
    TIM_OC2Init(TIM1,&TIM_OCInitStruct);
    TIM_OC3Init(TIM1,&TIM_OCInitStruct);//根据TIM1_OCInitStruct中指定的参数初始化 TIM1通道x
  
    //Set ADC sample time 
    TIM_OCInitStruct.TIM_Pulse=0x505;//待装入捕获比较寄存器的脉冲值    
    TIM_OC4Init(TIM1,&TIM_OCInitStruct);
    TIM_ITConfig(TIM1, TIM_IT_CC4, ENABLE); //CCR4的中断，这个通过设置CCR4的pulse来控制产生中断相当于PWM-ON的位置
    //PWM使能
    TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);
//   TIM_OC4PreloadConfig(TIM1,TIM_OCPreload_Enable);
    //刹车死区配置
    TIM_BDTRStructure.TIM_AutomaticOutput=TIM_AutomaticOutput_Enable;// 自动输出功能使能
    TIM_BDTRStructure.TIM_Break=TIM_Break_Disable;//失能刹车输入
    TIM_BDTRStructure.TIM_BreakPolarity=TIM_BreakPolarity_High; //刹车输入管脚极性高
    TIM_BDTRStructure.TIM_DeadTime=1; //输出打开和关闭状态之间的延时     =1/84Mhz=0.0119us
    TIM_BDTRStructure.TIM_LOCKLevel=TIM_LOCKLevel_OFF;// 锁电平参数: 不锁任何位
    TIM_BDTRStructure.TIM_OSSIState=TIM_OSSIState_Disable; //设置在运行模式下非工作状态选项
    TIM_BDTRStructure.TIM_OSSRState=TIM_OSSRState_Disable; //设置在运行模式下非工作状态选项
    TIM_BDTRConfig(TIM1,&TIM_BDTRStructure);

    TIM_ARRPreloadConfig(TIM1,ENABLE); //使能TIM1在ARR上的预装载寄存器

    TIM_Cmd(TIM1,ENABLE);//打开TIM1    
    
    TIM_CtrlPWMOutputs(TIM1,ENABLE);//PWM使能输出

 
    //ADC初始化
    ADC_CommonInitStructure.ADC_Mode=ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_Prescaler=ADC_Prescaler_Div4;
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;//DMA失能
    ADC_CommonInitStructure.ADC_TwoSamplingDelay=ADC_TwoSamplingDelay_5Cycles;
    
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;    
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //禁用启用
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_RisingFalling;    
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigInjecConv_T1_CC4;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;

    ADC_Init(ADC1,&ADC_InitStructure);      
      
}


