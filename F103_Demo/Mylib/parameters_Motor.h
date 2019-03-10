#ifndef __PARAMETERS_MOTOR_H
#define __PARAMETERS_MOTOR_H

#define TRIG_CONV_LATENCY_NS	(3000uL/ADC_CLK_MHz)
#define ADV_TIM_CLK_MHz    72
#define ADC_CLK_MHz        12
#define SAMPLING_TIME_NS ((((28 * 1000uL)+500)/ADC_CLK_MHz)+TRIG_CONV_LATENCY_NS)
#define TNOISE_NS                     1000
#define TRISE_NS                      1500 
#define TRISE (uint16_t)((((uint16_t)(TRISE_NS)) * ADV_TIM_CLK_MHz)/1000uL)
#define SW_DEADTIME_NS                 400  /*!< Dead-time to be inserted  
                                                           by FW, only if low side 
                                                           signals are enabled */
#define DEADTIME_NS  SW_DEADTIME_NS       //=800
#define TMIN (((uint16_t)(((DEADTIME_NS+((uint16_t)(TRISE_NS))+\
			 ((uint16_t)(SAMPLING_TIME_NS+TRIG_CONV_LATENCY_NS)))*ADV_TIM_CLK_MHz)/1000ul))+1)
#define HTMIN (uint16_t)(TMIN >> 1)
#define MAX_TNTR_NS     TRISE_NS
#define TW_AFTER ((uint16_t)(((DEADTIME_NS+MAX_TNTR_NS)*ADV_TIM_CLK_MHz)/1000ul))
#define TW_BEFORE (((uint16_t)(((((uint16_t)(SAMPLING_TIME_NS+TRIG_CONV_LATENCY_NS)))*ADV_TIM_CLK_MHz)/1000ul))+1u)

/** @name ADC channel number definitions */
/** @{ */
#define MC_ADC_CHANNEL_0     0
#define MC_ADC_CHANNEL_1     1
#define MC_ADC_CHANNEL_2     2
#define MC_ADC_CHANNEL_3     3
#define MC_ADC_CHANNEL_4     4
#define MC_ADC_CHANNEL_5     5
#define MC_ADC_CHANNEL_6     6
#define MC_ADC_CHANNEL_7     7
#define MC_ADC_CHANNEL_8     8
#define MC_ADC_CHANNEL_9     9
#define MC_ADC_CHANNEL_10    10
#define MC_ADC_CHANNEL_11    11
#define MC_ADC_CHANNEL_12    12
#define MC_ADC_CHANNEL_13    13
#define MC_ADC_CHANNEL_14    14
#define MC_ADC_CHANNEL_15    15
#define MC_ADC_CHANNEL_16    16
#define MC_ADC_CHANNEL_17    17
#define MC_ADC_CHANNEL_18    18
/** @} */

#define PHASE_A_MSK       (uint32_t)((uint32_t)(MC_ADC_CHANNEL_4) << 15)
#define PHASE_B_MSK       (uint32_t)((uint32_t)(MC_ADC_CHANNEL_5) << 15)
#define PHASE_C_MSK       (uint32_t)((uint32_t)(MC_ADC_CHANNEL_6) << 15)

#endif


