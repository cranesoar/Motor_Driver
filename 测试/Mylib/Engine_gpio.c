#include "Engine_gpio.h"
u8 get_flag=0,up_flag=0;
int get_time=0,up_time=0;
float get_speed=0;
static void Task_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC , ENABLE );
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE );
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15 |GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);	
}	



void Ascend_Task_Init(void)
{
	Task_gpio_init();
	GPIO_ResetBits(GPIOC,GPIO_Pin_14);
	GPIO_ResetBits(GPIOC,GPIO_Pin_15);
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
//	GPIO_ResetBits(GPIOC,GPIO_Pin_3);
	//Ascend_Loosen;
	
}

void Ascend_Task_Control(void)
{

	TsskFlag_getvalue();	

}

void TsskFlag_getvalue(void)
{
	if(RC.mouse_last.left_jump==0&&RC.mouse.left_jump==1)
	{
		RC.flag.left_jump=-RC.flag.left_jump;
	}
	if(RC.mouse_last.Right_jump==0&&RC.mouse.Right_jump==1)
	{
		RC.flag.Right_jump=-RC.flag.Right_jump;
	}
		if(RC.last_key.F==0&&RC.key.F==1)
	{
		RC.flag.F=-RC.flag.F;
	}
	if(RC.last_key.Q==0&&RC.key.Q==1)
	{
		RC.flag.Q=-RC.flag.Q;
	}
	if(RC.last_key.E==0&&RC.key.E==1)
	{
		RC.flag.E=-RC.flag.E;
	}
	if(RC.last_key.Z==0&&RC.key.Z==1)
	{
		RC.flag.Z=-RC.flag.Z;
	}
	if(RC.last_key.X==0&&RC.key.X==1)
	{
		RC.flag.X=-RC.flag.X;
	}
	if(RC.last_key.C==0&&RC.key.C==1)
	{
		RC.flag.C=-RC.flag.C;
	}
	if(RC.last_key.V==0&&RC.key.V==1)
	{
		RC.flag.V=-RC.flag.V;
	}
	if(RC.last_key.B==0&&RC.key.B==1)
	{
		RC.flag.B=-RC.flag.B;
	}
	if(RC.last_key.G==0&&RC.key.G==1)
	{
		RC.flag.G=-RC.flag.G;
	}		
		if(RC.last_key.R==0&&RC.key.R==1)
	{
		RC.flag.R=-RC.flag.R;
	}	
	  RC.last_key.G=RC.key.G; 
		RC.last_key.Q=RC.key.Q;
		RC.last_key.E=RC.key.E;
		RC.last_key.F=RC.key.F;	
		RC.last_key.Z=RC.key.Z;
		RC.last_key.X=RC.key.X;
		RC.last_key.C=RC.key.C;
		RC.last_key.V=RC.key.V;
		RC.last_key.B=RC.key.B;
   	RC.last_key.R=RC.key.R;	
		RC.mouse_last.left_jump=RC.mouse.left_jump;
		RC.mouse_last.Right_jump=RC.mouse.Right_jump;

}
void Collide_Adc_init()
{  GPIO_InitTypeDef  		GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//Ê¹ÄÜGPIOCÊ±ÖÓ
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//Ê¹ÄÜGPIOCÊ±Ö
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //Ê¹ÄÜADC1Ê±ÖÓ

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 ;//PC1 Í¨µÀ11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//Ä£ÄâÊäÈë
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//²»´øÉÏÏÂÀ­
	GPIO_Init(GPIOC, &GPIO_InitStructure);//³õÊ¼»¯ 
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_1 ;//PC1 Í¨µÀ11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//Ä£ÄâÊäÈë
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//²»´øÉÏÏÂÀ­
	GPIO_Init(GPIOA, &GPIO_InitStructure);//³õÊ¼»¯ 
  	
 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1¸´Î»
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//¸´Î»½áÊø	 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//¶ÀÁ¢Ä£Ê½
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//Á½¸ö²ÉÑù½×¶ÎÖ®¼äµÄÑÓ³Ù5¸öÊ±ÖÓ
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAÊ§ÄÜ
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ô¤·ÖÆµ4·ÖÆµ¡£ADCCLK=PCLK2/4=84/4=21Mhz,ADCÊ±ÖÓ×îºÃ²»Òª³¬¹ı36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//³õÊ¼»¯
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12Î»Ä£Ê½
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//·ÇÉ¨ÃèÄ£Ê½	
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//¹Ø±ÕÁ¬Ğø×ª»»
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//½ûÖ¹´¥·¢¼ì²â£¬Ê¹ÓÃÈí¼ş´¥·¢
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ÓÒ¶ÔÆë	
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1¸ö×ª»»ÔÚ¹æÔòĞòÁĞÖĞ Ò²¾ÍÊÇÖ»×ª»»¹æÔòĞòÁĞ1 
  ADC_Init(ADC1, &ADC_InitStructure);//ADC³õÊ¼»¯
	
 
	ADC_Cmd(ADC1, ENABLE);
	
	
	
}

 void Adc_Read(void)     //forward--PC0,backward--PC1,left--PC2,right--PC3 
{
	//ÉèÖÃÖ¸¶¨ADCµÄ¹æÔò×éÍ¨µÀ£¬Ò»¸öĞòÁĞ£¬²ÉÑùÊ±¼ä
	ADC_RegularChannelConfig(ADC1, 12, 1, ADC_SampleTime_480Cycles );	//ADC1,ADCÍ¨µÀ,480¸öÖÜÆÚ,Ìá¸ß²ÉÑùÊ±¼ä¿ÉÒÔÌá¸ß¾«È·¶È			    
  ADC_SoftwareStartConv(ADC1);		//Ê¹ÄÜÖ¸¶¨µÄADC1µÄÈí¼ş×ª»»Æô¶¯¹¦ÄÜ	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//µÈ´ı×ª»»½áÊø
//	PositionJudge.analog_val=(float)(ADC_GetConversionValue(ADC1))*(3.3/4096);	//·µ»Ø×î½üÒ»´ÎADC1¹æÔò×éµÄ×ª»»½á¹û
}

u8 Positon_judge(void)
{
	Adc_Read();
//	if(PositionJudge.analog_val>1)
//		return 1;
//	else 
//		return 0;
}
