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

void Board_ALL_Init(void)
{	
		/*中断分组*/
		NVIC_PriorityGroupConfig(NVIC_GROUP);
		/*滴答定时器配置*/
		SysTick_Configuration();	
    LED_Configuration(); //B12
    TIM1_Init(1000-1,4-1); //A8 A9 A19 CH1,2,3 //A7 B0 B1 CH1N,CH2N,CH3N 
    
		/*时间初始化*/
		Cycle_Time_Init();
    
//    DRV8305_Init();

    Init_OK=1; 

}
