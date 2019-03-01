#include "main.h"

u8 Init_OK;
int i;
//滴答定时器计数变量 ,49天后溢出
volatile uint32_t sysTickUptime=0;

#define TICK_PER_SECOND 10000 
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
// ADC  //A0 VSENA A1 VSENB A2 VSENC A3 VSENPVDD A4 ISENA A5 ISENB A6 ISENC  ()
// SPI1 //B3 SCK B4 MISO B5 MOSI // B8 
// TIM4 //B6 B7 CH1,2
// SPI2 //B13 SCK B14 MISO B15 MOSI// A15 
// CAN  //A11 A12
// SWD  //A13 A14
// DRV8305 // B9 ENGATE
u16 DRV=0;
void Board_ALL_Init(void)
{	
		/*中断分组*/
		NVIC_PriorityGroupConfig(NVIC_GROUP);
  
		/*滴答定时器配置*/
		SysTick_Configuration();	
    LED_Configuration(); 
   
		/*时间初始化*/
		Cycle_Time_Init();

    TIM1_Init(2250,1-1);//必须在DRV8305之前初始化//互补PWM输出配置（中心对齐）/ADC采样控制   72M/(2250*2)=16Khz
  
    DRV8305_Init();//DRV8305通信SPI配置
                          
    AS5048_Init(); //AS5048通信SPI配置
    
    DRV8305_SCS_L;        
    DRV=SPI1_ReadWriteByte(Set_80V);   //设置成80V增益    
    DRV8305_SCS_H;
    DelayMs(100); //  两帧之间须有100ns以上时间间隔 
      
    Protect_AdcInit(); //ADC初始化务必放在SPI初始化后面，否则ADC影响SPI配置通过，初始化不成功。 
    R3LM1_Init();
    FOC_NIVC_IRQ();
    lpf_k_init();
    Init_OK=1; 
}
