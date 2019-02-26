#ifndef __PARAMETERS_MOTOR_H
#define __PARAMETERS_MOTOR_H



#define TRIG_CONV_LATENCY_NS	(3000uL/ADC_CLK_MHz)
#define ADV_TIM_CLK_MHz    72
#define ADC_CLK_MHz        12
#define SAMPLING_TIME_NS ((((28 * 1000uL)+500)/ADC_CLK_MHz)+TRIG_CONV_LATENCY_NS)
#define TNOISE_NS                     1550
#define TRISE_NS                      1550 
#define TRISE (uint16_t)((((uint16_t)(TRISE_NS)) * ADV_TIM_CLK_MHz)/1000uL)
#define SW_DEADTIME_NS                   800 /*!< Dead-time to be inserted  
                                                           by FW, only if low side 
                                                           signals are enabled */
#define DEADTIME_NS  SW_DEADTIME_NS       //=800
#define TMIN (((uint16_t)(((DEADTIME_NS+((uint16_t)(TRISE_NS))+\
			 ((uint16_t)(SAMPLING_TIME_NS+TRIG_CONV_LATENCY_NS)))*ADV_TIM_CLK_MHz)/1000ul))+1)
#define HTMIN (uint16_t)(TMIN >> 1)

#endif


