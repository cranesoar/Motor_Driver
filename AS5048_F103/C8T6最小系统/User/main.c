#include "main.h"

u8 Init_ok = 0;

int main(void)
{

    LED_Configuration();

    SPI1_Init();
	
		SPI2_Init();

    AS5048_Init();

    LPF2pSetCutoffFreq_1(1000,40);
	
    LPF2pSetCutoffFreq_2(1000,40);

    SysTick_Init();						//滴答定时器初始化
	
		CAN_Configuration();
	
    PWM_Configuration(10000);
		
		Init_ok = 1;

    while(1)
    {
//			as5048_read_angle();
    }
}
