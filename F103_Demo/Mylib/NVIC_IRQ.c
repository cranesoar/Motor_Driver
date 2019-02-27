#include "NVIC_IRQ.h"

void FOC_NIVC_IRQ(void)
{
  NVIC_InitTypeDef NVIC_InitStructure; //TIM1中断配置

    //通道4触发中断配置
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
 
}

void R3LM1_Init(void)
{
  
    ADC_ITConfig(ADC1,ADC_IT_JEOC,ENABLE);//ENABLE INJECTED INTERRUPT
    ADC_ExternalTrigInjectedConvCmd(ADC1,ENABLE); 
    ADC_ExternalTrigInjectedConvCmd(ADC2,ENABLE);  
    
    ADC_Cmd(ADC1, ENABLE);
    ADC_Cmd(ADC2, ENABLE);
  
    //使能ADC1复位校准寄存器
    ADC_ResetCalibration(ADC1);
    ADC_ResetCalibration(ADC2);    
    //检查校准寄存器是否复位完毕
    while(ADC_GetResetCalibrationStatus(ADC1));
    while(ADC_GetResetCalibrationStatus(ADC2));   
  
    ADC_SoftwareStartConvCmd(ADC1,ENABLE);
    ADC_SoftwareStartConvCmd(ADC2,ENABLE); 
  
//    ADC_InjectedSequencerLengthConfig(ADC1,0);
//    ADC_InjectedSequencerLengthConfig(ADC2,0);
    
    ADC_StartCalibration(ADC1);
    ADC_StartCalibration(ADC2); 
  
    //检查校准寄存器是否复位完毕
    while(ADC_GetResetCalibrationStatus(ADC1)||ADC_GetResetCalibrationStatus(ADC2))
    {
    }  
  /*Enable external trigger fo injected conv of ADC2 (mandatory for Dual mode)*/ 
//    SET_BIT(ADC2->CR2, (ADC_CR2_JSWSTART | ADC_CR2_JEXTTRIG));     
}

void R3LM1_SetADCSampPointSect1(void)
{
   u16 TIM1CCR4num=0, DeltaDuty=0;
  
  /* Set Polarity of CC4 active high (default) */  
   TIM_OC4PolarityConfig(TIM1,TIM_OCPolarity_High);
  
   if((Time1_Period - DRV8305.Duty.MOTA) >  TW_AFTER)
   {
   TIM1CCR4num = MAX_Duty - 1;
     
   ADC1->JSQR = PHASE_A_MSK;
   ADC2->JSQR = PHASE_B_MSK;       
   }
  
   else
   {
     DeltaDuty = DRV8305.Duty.MOTA - DRV8305.Duty.MOTB;
     
     if(DeltaDuty > (Time1_Period - DRV8305.Duty.MOTA) * 2u)
     TIM1CCR4num = DRV8305.Duty.MOTA - TW_BEFORE;
     
     else
     {
     TIM1CCR4num = DRV8305.Duty.MOTA + TW_AFTER;
        if(TIM1CCR4num >= Time1_Period )
        {
        /* Set Polarity of CC4 active low */
        TIM_OC4PolarityConfig(TIM1,TIM_OCPolarity_Low);
        
        TIM1CCR4num = (2u * Time1_Period) - TIM1CCR4num - 1u;        
        }               
     }
    ADC1->JSQR = PHASE_B_MSK;
    ADC2->JSQR = PHASE_C_MSK;   
   }
   
   /* Set TIMx_CH4 value */
   TIM1->CCR4 = TIM1CCR4num; 
}

void R3LM1_SetADCSampPointSect2(void)
{
   u16 TIM1CCR4num=0, DeltaDuty=0;
  
  /* Set Polarity of CC4 active high (default) */  
   TIM_OC4PolarityConfig(TIM1,TIM_OCPolarity_High);
  
   if((Time1_Period - DRV8305.Duty.MOTB) >  TW_AFTER)
   {
   TIM1CCR4num = MAX_Duty - 1;
     
   ADC1->JSQR = PHASE_A_MSK;
   ADC2->JSQR = PHASE_B_MSK;       
   }
  
   else
   {
     DeltaDuty = DRV8305.Duty.MOTB - DRV8305.Duty.MOTA;
     
     if(DeltaDuty > (Time1_Period - DRV8305.Duty.MOTB) * 2u)
     TIM1CCR4num = DRV8305.Duty.MOTB - TW_BEFORE;
     
     else
     {
     TIM1CCR4num = DRV8305.Duty.MOTB + TW_AFTER;
        if(TIM1CCR4num >= Time1_Period )
        {
        /* Set Polarity of CC4 active low */
        TIM_OC4PolarityConfig(TIM1,TIM_OCPolarity_Low);
        
        TIM1CCR4num = (2u * Time1_Period) - TIM1CCR4num - 1u;        
        }               
     }
    ADC1->JSQR = PHASE_A_MSK;
    ADC2->JSQR = PHASE_C_MSK;   
   }
   
   /* Set TIMx_CH4 value */
   TIM1->CCR4 = TIM1CCR4num; 
}


void R3LM1_SetADCSampPointSect3(void)
{
   u16 TIM1CCR4num=0, DeltaDuty=0;
  
  /* Set Polarity of CC4 active high (default) */  
   TIM_OC4PolarityConfig(TIM1,TIM_OCPolarity_High);
  
   if((Time1_Period - DRV8305.Duty.MOTB) >  TW_AFTER)
   {
   TIM1CCR4num = MAX_Duty - 1;
     
   ADC1->JSQR = PHASE_A_MSK;
   ADC2->JSQR = PHASE_B_MSK;       
   }
  
   else
   {
     DeltaDuty = DRV8305.Duty.MOTB - DRV8305.Duty.MOTC;
     
     if(DeltaDuty > (Time1_Period - DRV8305.Duty.MOTB) * 2u)
     TIM1CCR4num = DRV8305.Duty.MOTB - TW_BEFORE;
     
     else
     {
     TIM1CCR4num = DRV8305.Duty.MOTB + TW_AFTER;
        if(TIM1CCR4num >= Time1_Period )
        {
        /* Set Polarity of CC4 active low */
        TIM_OC4PolarityConfig(TIM1,TIM_OCPolarity_Low);
        
        TIM1CCR4num = (2u * Time1_Period) - TIM1CCR4num - 1u;        
        }               
     }
    ADC1->JSQR = PHASE_A_MSK;
    ADC2->JSQR = PHASE_C_MSK;   
   }
   
   /* Set TIMx_CH4 value */
   TIM1->CCR4 = TIM1CCR4num; 
}


void R3LM1_SetADCSampPointSect4(void)
{
   u16 TIM1CCR4num=0, DeltaDuty=0;
  
  /* Set Polarity of CC4 active high (default) */  
   TIM_OC4PolarityConfig(TIM1,TIM_OCPolarity_High);
  
   if((Time1_Period - DRV8305.Duty.MOTC) >  TW_AFTER)
   {
   TIM1CCR4num = MAX_Duty - 1;
     
   ADC1->JSQR = PHASE_A_MSK;
   ADC2->JSQR = PHASE_B_MSK;       
   }
  
   else
   {
     DeltaDuty = DRV8305.Duty.MOTC - DRV8305.Duty.MOTB;
     
     if(DeltaDuty > (Time1_Period - DRV8305.Duty.MOTC) * 2u)
     TIM1CCR4num = DRV8305.Duty.MOTC - TW_BEFORE;
     
     else
     {
     TIM1CCR4num = DRV8305.Duty.MOTC + TW_AFTER;
        if(TIM1CCR4num >= Time1_Period )
        {
        /* Set Polarity of CC4 active low */
        TIM_OC4PolarityConfig(TIM1,TIM_OCPolarity_Low);
        
        TIM1CCR4num = (2u * Time1_Period) - TIM1CCR4num - 1u;        
        }               
     }
    ADC1->JSQR = PHASE_A_MSK;
    ADC2->JSQR = PHASE_B_MSK;   
   }
   
   /* Set TIMx_CH4 value */
   TIM1->CCR4 = TIM1CCR4num; 
}

void R3LM1_SetADCSampPointSect5(void)
{
   u16 TIM1CCR4num=0, DeltaDuty=0;
  
  /* Set Polarity of CC4 active high (default) */  
   TIM_OC4PolarityConfig(TIM1,TIM_OCPolarity_High);
  
   if((Time1_Period - DRV8305.Duty.MOTC) >  TW_AFTER)
   {
   TIM1CCR4num = MAX_Duty - 1;
     
   ADC1->JSQR = PHASE_A_MSK;
   ADC2->JSQR = PHASE_B_MSK;       
   }
  
   else
   {
     DeltaDuty = DRV8305.Duty.MOTC - DRV8305.Duty.MOTA;
     
     if(DeltaDuty > (Time1_Period - DRV8305.Duty.MOTC) * 2u)
     TIM1CCR4num = DRV8305.Duty.MOTC - TW_BEFORE;
     
     else
     {
     TIM1CCR4num = DRV8305.Duty.MOTC + TW_AFTER;
        if(TIM1CCR4num >= Time1_Period )
        {
        /* Set Polarity of CC4 active low */
        TIM_OC4PolarityConfig(TIM1,TIM_OCPolarity_Low);
        
        TIM1CCR4num = (2u * Time1_Period) - TIM1CCR4num - 1u;        
        }               
     }
    ADC1->JSQR = PHASE_A_MSK;
    ADC2->JSQR = PHASE_B_MSK;   
   }
   
   /* Set TIMx_CH4 value */
   TIM1->CCR4 = TIM1CCR4num; 
}

void R3LM1_SetADCSampPointSect6(void)
{
   u16 TIM1CCR4num=0, DeltaDuty=0;
  
  /* Set Polarity of CC4 active high (default) */  
   TIM_OC4PolarityConfig(TIM1,TIM_OCPolarity_High);
  
   if((Time1_Period - DRV8305.Duty.MOTA) >  TW_AFTER)
   {
   TIM1CCR4num = MAX_Duty - 1;
     
   ADC1->JSQR = PHASE_A_MSK;
   ADC2->JSQR = PHASE_B_MSK;       
   }
  
   else
   {
     DeltaDuty = DRV8305.Duty.MOTA - DRV8305.Duty.MOTC;
     
     if(DeltaDuty > (Time1_Period - DRV8305.Duty.MOTA) * 2u)
     TIM1CCR4num = DRV8305.Duty.MOTA - TW_BEFORE;
     
     else
     {
     TIM1CCR4num = DRV8305.Duty.MOTA + TW_AFTER;
        if(TIM1CCR4num >= Time1_Period )
        {
        /* Set Polarity of CC4 active low */
        TIM_OC4PolarityConfig(TIM1,TIM_OCPolarity_Low);
        
        TIM1CCR4num = (2u * Time1_Period) - TIM1CCR4num - 1u;        
        }               
     }
    ADC1->JSQR = PHASE_B_MSK;
    ADC2->JSQR = PHASE_C_MSK;   
   }
   
   /* Set TIMx_CH4 value */
   TIM1->CCR4 = TIM1CCR4num; 
}
