#ifndef __DRV8305_H
#define __DRV8305_H
#include "main.h"

#define RCC_DRV8305	     RCC_APB2Periph_GPIOB

#define DRV8305_PORT      GPIOB

#define DRV8305_Pin_SCS      GPIO_Pin_8
#define DRV8305_Pin_ENGATE   GPIO_Pin_9
//#define DRV8305_Pin_WAKE      GPIO_Pin_5
//#define DRV8305_Pin_PWRGD    GPIO_Pin_1

#define DRV8305_SCS_H     GPIO_SetBits(DRV8305_PORT   ,   DRV8305_Pin_SCS)
#define DRV8305_SCS_L     GPIO_ResetBits(DRV8305_PORT ,    DRV8305_Pin_SCS)

#define DRV8305_ENGATE_H     GPIO_SetBits(DRV8305_PORT   ,   DRV8305_Pin_ENGATE)
#define DRV8305_ENGATE_L     GPIO_ResetBits(DRV8305_PORT ,    DRV8305_Pin_ENGATE)

//#define DRV8305_SCS_H     GPIO_SetBits(DRV8305_PORT   ,   DRV8305_Pin_SCS)
//#define DRV8305_SCS_L     GPIO_ResetBits(DRV8305_PORT ,    DRV8305_Pin_SCS)

//#define DRV8305_SCS_H     GPIO_SetBits(DRV8305_PORT   ,   DRV8305_Pin_SCS)
//#define DRV8305_SCS_L     GPIO_ResetBits(DRV8305_PORT ,    DRV8305_Pin_SCS)
 
//#define READ   

#define Set_80V  0x503F   
#define Set_40V  0x502A  
#define Set_20V  0x5015;
void DRV8305_Init(void);

#endif
