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
    ADC_InitStructure.ADC_NbrOfChannel = 0;
    ADC_DiscModeCmd(ADC1,DISABLE);    
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_DeInit(ADC2);    //复位 ADC2,将外设  ADC2  的全部寄存器重设为缺省值 
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Left;
    ADC_InitStructure.ADC_NbrOfChannel = 0;
    ADC_DiscModeCmd(ADC2,DISABLE);    
    ADC_Init(ADC2, &ADC_InitStructure);

    //下面是注入通道，这里注入通道设置成TIM1的CCR4事件触发
    ADC_AutoInjectedConvCmd(ADC1,DISABLE);   //禁用自动注入组转换
    ADC_InjectedDiscModeCmd(ADC1,DISABLE);   //失能不连续模式
    ADC_SetInjectedOffset(ADC1,ADC_InjectedChannel_1,0);    //校准值设定
    ADC_SetInjectedOffset(ADC1,ADC_InjectedChannel_2,0);    //校准值设定
    ADC_SetInjectedOffset(ADC1,ADC_InjectedChannel_3,0);    //校准值设定    
    ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_T1_CC4);//这个可以使用time1的CCR4事件来触发采样
    ADC_InjectedSequencerLengthConfig(ADC1, 2);
    ADC_InjectedChannelConfig(ADC1,ADC_Channel_4,1,ADC_SampleTime_41Cycles5);
    ADC_InjectedChannelConfig(ADC1,ADC_Channel_6,2,ADC_SampleTime_41Cycles5);
    ADC_ExternalTrigInjectedConvCmd(ADC1,ENABLE); 
    //ADC2 注入通道，同步注入
    ADC_AutoInjectedConvCmd(ADC2,DISABLE);   //禁用自动注入组转换
    ADC_InjectedDiscModeCmd(ADC2,DISABLE);   //失能不连续模式
    ADC_SetInjectedOffset(ADC2,ADC_InjectedChannel_1,0);    //校准值设定
    ADC_SetInjectedOffset(ADC2,ADC_InjectedChannel_2,0);    //校准值设定
    ADC_SetInjectedOffset(ADC2,ADC_InjectedChannel_3,0);    //校准值设定        
    ADC_InjectedSequencerLengthConfig(ADC2, 1);
    ADC_InjectedChannelConfig(ADC2,ADC_Channel_5,1,ADC_SampleTime_41Cycles5);
    
    
    
    ADC_ITConfig(ADC1,ADC_IT_JEOC,ENABLE);//ENABLE INJECTED INTERRUPT
    /* Enable automatic injected conversion start after regular one */
    //  ADC_AutoInjectedConvCmd(ADC1, ENABLE);//是否在规则通道结束之后自动开始注入通道采样

//    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);
    ADC_Cmd(ADC2, ENABLE);

        //使能ADC1复位校准寄存器
    ADC_ResetCalibration(ADC1);
    ADC_ResetCalibration(ADC2);    
    //检查校准寄存器是否复位完毕
    while(ADC_GetResetCalibrationStatus(ADC1)||ADC_GetResetCalibrationStatus(ADC2))
    {
    }
    
   ADC_ExternalTrigInjectedConvCmd(ADC2,ENABLE);
                
    
}   

s32 inj_v1[3];
//int Feedback_Theta=0,SetAdvance_Theta=200;
void ADC1_2_IRQHandler(void)
{

    if(ADC_GetITStatus(ADC1,ADC_IT_JEOC) != RESET) 
    {
                                         
        switch(Section)   
        {
          case 1: case 6:
                  DRV8305.Clack.Ib = ADC_GetInjectedConversionValue(ADC2,ADC_InjectedChannel_1)-15800;
                  DRV8305.Clack.Ic = ADC_GetInjectedConversionValue(ADC1,ADC_InjectedChannel_2)-15897;
                  DRV8305.Clack.Ia = 0-DRV8305.Clack.Ib-DRV8305.Clack.Ic;
                  break;
          
          case 2: case 3:
                  DRV8305.Clack.Ia = ADC_GetInjectedConversionValue(ADC1,ADC_InjectedChannel_1)-15796;
                  DRV8305.Clack.Ic = ADC_GetInjectedConversionValue(ADC1,ADC_InjectedChannel_2)-15897;
                  DRV8305.Clack.Ib = 0-DRV8305.Clack.Ia-DRV8305.Clack.Ic;
                  break;

          case 4: case 5:
                  DRV8305.Clack.Ia = ADC_GetInjectedConversionValue(ADC1,ADC_InjectedChannel_1)-15796;
                  DRV8305.Clack.Ib = ADC_GetInjectedConversionValue(ADC2,ADC_InjectedChannel_1)-15897;
                  DRV8305.Clack.Ic = 0-DRV8305.Clack.Ib-DRV8305.Clack.Ia;
                  break;
          
          default : DRV8305.Clack.Ia = ADC_GetInjectedConversionValue(ADC1,ADC_InjectedChannel_1)-15796;
                    DRV8305.Clack.Ib = ADC_GetInjectedConversionValue(ADC2,ADC_InjectedChannel_1)-15897;
                    DRV8305.Clack.Ic = ADC_GetInjectedConversionValue(ADC1,ADC_InjectedChannel_2)-15897;
                    break;                    
        }  


      
        ADC_ClearITPendingBit(ADC1,ADC_IT_JEOC);
    }

}

//初始化AD
void Adcdma_Init(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    //---------------------充电AD初始化--------------------
    //启动DMA时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    //启动ADC1时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    //采样脚设置
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //DMA1通道1配置
    DMA_DeInit(DMA1_Channel1);
    //外设地址
    DMA_InitStructure.DMA_PeripheralBaseAddr =  (uint32_t)&ADC1->DR;
    //内存地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADCBUFF.ADCConvertedValue;
    //dma传输方向单向
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    //设置DMA在传输时缓冲区的长度
    DMA_InitStructure.DMA_BufferSize = 7;
    //设置DMA的外设递增模式，一个外设
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    //设置DMA的内存递增模式
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    //外设数据字长
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    //内存数据字长
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    //设置DMA的传输模式：连续不断的循环模式
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    //设置DMA的优先级别
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    //设置DMA的2个memory中的变量互相访问
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    //使能通道1
    DMA_Cmd(DMA1_Channel1, ENABLE);

    //ADC1配置
    //独立工作模式
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    //扫描方式
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    //连续转换
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    //外部触发禁止
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    //数据右对齐
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    //用于转换的通道数
    ADC_InitStructure.ADC_NbrOfChannel = 7;
    ADC_Init(ADC1, &ADC_InitStructure);

    //规则模式通道配置
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 6, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 7, ADC_SampleTime_239Cycles5);

    //使能ADC1的DMA
    ADC_DMACmd(ADC1, ENABLE);

    //使能ADC1
    ADC_Cmd(ADC1, ENABLE);

    //使能ADC1复位校准寄存器
    ADC_ResetCalibration(ADC1);
    //检查校准寄存器是否复位完毕
    while(ADC_GetResetCalibrationStatus(ADC1));

    //开始校准
    ADC_StartCalibration(ADC1);
    //检测是否校准完毕
    while(ADC_GetCalibrationStatus(ADC1));

    //开启ADC1的软件转换
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

