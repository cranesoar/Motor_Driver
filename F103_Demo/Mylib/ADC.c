#include "ADC.h"

union adcbuff ADCBUFF;

void Protect_AdcInit(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    //启动ADC时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
    //采样脚设置
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);    //设置 ADC 分频因子 6   //72M/6=12,ADC 最大时间不能超过 14M
  
    GPIO_InitStructure.GPIO_Pin =/*GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|*/GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;  //A0 VSENA A1 VSENB A2 VSENC A3 VSENPVDD A4 ISENA A5 ISENB A6 ISENC  ()
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
 
    ADC_DeInit(ADC1);    //复位 ADC1,将外设  ADC1  的全部寄存器重设为缺省值 
    ADC_InitStructure.ADC_Mode = ADC_Mode_InjecSimult;   //同步注入模式
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Left;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_DiscModeCmd(ADC1,DISABLE);    
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_DeInit(ADC2);    //复位 ADC2,将外设  ADC2  的全部寄存器重设为缺省值 
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Left;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_DiscModeCmd(ADC2,DISABLE);    
    ADC_Init(ADC2, &ADC_InitStructure);

    //下面是注入通道，这里注入通道设置成TIM1的CCR4事件触发
    ADC_AutoInjectedConvCmd(ADC1,DISABLE);   //禁用自动注入组转换
    ADC_InjectedDiscModeCmd(ADC1,DISABLE);   //失能不连续模式
    ADC_SetInjectedOffset(ADC1,ADC_InjectedChannel_1,0);    //校准值设定
    ADC_SetInjectedOffset(ADC1,ADC_InjectedChannel_2,0);    //校准值设定
    ADC_SetInjectedOffset(ADC1,ADC_InjectedChannel_3,0);    //校准值设定    
    ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_T1_CC4);//这个可以使用time1的CCR4事件来触发采样
    ADC_InjectedSequencerLengthConfig(ADC1, 3);
    ADC_InjectedChannelConfig(ADC1,ADC_Channel_6,1,ADC_SampleTime_28Cycles5);
    ADC_InjectedChannelConfig(ADC1,ADC_Channel_5,2,ADC_SampleTime_28Cycles5);
    ADC_InjectedChannelConfig(ADC1,ADC_Channel_4,3,ADC_SampleTime_28Cycles5);

    //ADC2 注入通道，同步注入
    ADC_AutoInjectedConvCmd(ADC2,DISABLE);   //禁用自动注入组转换
    ADC_InjectedDiscModeCmd(ADC2,DISABLE);   //失能不连续模式
    ADC_SetInjectedOffset(ADC2,ADC_InjectedChannel_1,0);    //校准值设定
    ADC_SetInjectedOffset(ADC2,ADC_InjectedChannel_2,0);    //校准值设定
    ADC_SetInjectedOffset(ADC2,ADC_InjectedChannel_3,0);    //校准值设定        
    ADC_InjectedSequencerLengthConfig(ADC2, 3);
    ADC_InjectedChannelConfig(ADC2,ADC_Channel_6,1,ADC_SampleTime_28Cycles5);
    ADC_InjectedChannelConfig(ADC2,ADC_Channel_5,2,ADC_SampleTime_28Cycles5);
    ADC_InjectedChannelConfig(ADC2,ADC_Channel_4,3,ADC_SampleTime_28Cycles5);   
  
  
//    ADC_Cmd(ADC1,ENABLE);
//    ADC_Cmd(ADC2,ENABLE);
        
}   

s32 inj_v1[3];
//int Feedback_Theta=0,SetAdvance_Theta=200;
void ADC1_2_IRQHandler(void)
{
  int16_t wAux1 , wAux2;

    if(ADC_GetITStatus(ADC1,ADC_IT_JEOC) != RESET) 
    {
                                         
        switch(Section)   
        {
          case 1: case 6:   //1=B 2=C
                  wAux1 = (int16_t)( ADC1->JDR1 );
                  wAux2 = (int16_t)( ADC2->JDR1 );          
//                  wAux1 *= 1;  wAux2 *= 1;
//                  wAux1 = (int16_t)( DRV8305.Offset.PhaseBOffset ) - wAux1;
//                  wAux2 = (int16_t)( DRV8305.Offset.PhaseCOffset ) - wAux2;         
                  wAux1 = (int16_t)(limit( wAux1 , -32767 , 32767));
                  wAux2 = (int16_t)(limit( wAux2 , -32767 , 32767));          
                  DRV8305.Clack.Ia = wAux1-16384;
                  DRV8305.Clack.Ib = wAux2-16384;
                  DRV8305.Clack.Ic = 0-wAux1-wAux2+32768;
                  break;
          
          case 2: case 3:  //1=A 2=C
                  wAux1 = (int16_t)( ADC1->JDR1 );
                  wAux2 = (int16_t)( ADC2->JDR1 );          
//                  wAux1 *= 1;  wAux2 *= 1;
//                  wAux1 = (int16_t)( DRV8305.Offset.PhaseAOffset ) - wAux1;
//                  wAux2 = (int16_t)( DRV8305.Offset.PhaseBOffset ) - wAux2;         
                  wAux1 = (int16_t)(limit( wAux1 , -32767 , 32767));
                  wAux2 = (int16_t)(limit( wAux2 , -32767 , 32767));          
                  DRV8305.Clack.Ia = wAux1-16384;
                  DRV8305.Clack.Ib = wAux2-16384;
                  DRV8305.Clack.Ic = 0-wAux1-wAux2+32768;
                  break;

          case 4: case 5:   //1=A 2=B
                  wAux1 = (int16_t)( ADC1->JDR1 );
                  wAux2 = (int16_t)( ADC2->JDR1 );          
//                  wAux1 *= 1;  wAux2 *= 1;
//                  wAux1 = (int16_t)( DRV8305.Offset.PhaseAOffset ) - wAux1;
//                  wAux2 = (int16_t)( DRV8305.Offset.PhaseBOffset ) - wAux2;         
                  wAux1 = (int16_t)(limit( wAux1 , -32767 , 32767));
                  wAux2 = (int16_t)(limit( wAux2 , -32767 , 32767));          
                  DRV8305.Clack.Ia = wAux1-16384;
                  DRV8305.Clack.Ib = wAux2-16384;
                  DRV8305.Clack.Ic = 0-wAux1-wAux2+32768;
                  break;
          
          default :  
             break;                    
        }  
 
        ADC_ClearITPendingBit(ADC1,ADC_IT_JEOC);                                                                                   
    }

}


