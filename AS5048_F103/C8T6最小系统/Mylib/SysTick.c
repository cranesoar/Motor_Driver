#include "main.h"

/* SystemFrequency / 1000    1ms中断一次
 * SystemFrequency / 100000	 10us中断一次
 * SystemFrequency / 1000000 1us中断一次
 */
RCC_ClocksTypeDef  rcc_clocks;

void SysTick_Init(void)
{
		RCC_GetClocksFreq(&rcc_clocks);

    if (SysTick_Config(SystemCoreClock / 1000))
    {
        /* Capture error */
        while (1);
    }
}
