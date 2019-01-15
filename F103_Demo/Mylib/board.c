#include "main.h"

u8 Init_OK;
int i;
//滴答定时器计数变量 ,49天后溢出
volatile uint32_t sysTickUptime=0;

#define TICK_PER_SECOND 1000 
#define TICK_US	(1000000/TICK_PER_SECOND)
RCC_ClocksTypeDef  rcc_clocks;
void SysTick_Configuration(void)
{
	
	uint32_t         cnts;

	RCC_GetClocksFreq(&rcc_clocks);

	cnts = (uint32_t)rcc_clocks.HCLK_Frequency / TICK_PER_SECOND;
	cnts = cnts / 8;

	SysTick_Config(cnts);
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}

uint32_t GetSysTime_us(void) 
{
	register uint32_t ms;
	u32 value;
	ms = sysTickUptime;
 	value = ms * TICK_US + (SysTick->LOAD - SysTick->VAL) * TICK_US / SysTick->LOAD;
	return value;
}


void DelayUs(uint32_t us)
{
    uint32_t now = GetSysTime_us();
    while (GetSysTime_us() - now < us);
}

void DelayMs(uint32_t ms)
{
    while (ms--)
      DelayUs(1000);
}
// USART3 //B10 TX B11 RX
// LED  //B12
// TIM1 //A8 A9 A10  CH1,2,3,4 //A7 B0 B1 CH1N,CH2N,CH3N 
// ADC  //A0 A1 A2 A3 A4 A5 (A6)
// SPI1 //B3 B4 B5 // B8 
// TIM4 //B6 B7 CH1,2
// SPI2 //B13 B14 B15 // A15 
// CAN  //A11 A12
// SWD  //A13 A14
// DRV8305 // B9 ENGATE
void Board_ALL_Init(void)
{	
		/*中断分组*/
		NVIC_PriorityGroupConfig(NVIC_GROUP);
  
		/*滴答定时器配置*/
		SysTick_Configuration();	
    LED_Configuration(); 
    TIM1_Init(1000-1,2-1);//互补PWM输出配置（中心对齐）   72M/2000=
    
		/*时间初始化*/
		Cycle_Time_Init();
    
    DRV8305_Init();//DRV8305通信SPI配置
  
    AS5048_Init(); //AS5048通信SPI配置

    Init_OK=1; 

}
