#ifndef __DRV8305_H
#define __DRV8305_H
#include "board.h"

#define RCC_DRV8305	     RCC_AHB1Periph_GPIOC 

#define DRV8305_PORT      GPIOC

#define DRV8305_Pin_SCS      GPIO_Pin_4
#define DRV8305_Pin_PWRGD    GPIO_Pin_1
#define DRV8305_Pin_ENGATE   GPIO_Pin_0
#define DRV8305_Pin_WAKE      GPIO_Pin_5

#define DRV8305_SCS_H     GPIO_SetBits(DRV8305_PORT   ,   DRV8305_Pin_SCS)
#define DRV8305_SCS_L     GPIO_ResetBits(DRV8305_PORT ,    DRV8305_Pin_SCS)

#define DRV8305_ENGATE_H     GPIO_SetBits(DRV8305_PORT   ,   DRV8305_Pin_ENGATE)
#define DRV8305_ENGATE_L     GPIO_ResetBits(DRV8305_PORT ,    DRV8305_Pin_ENGATE)

//#define DRV8305_SCS_H     GPIO_SetBits(DRV8305_PORT   ,   DRV8305_Pin_SCS)
//#define DRV8305_SCS_L     GPIO_ResetBits(DRV8305_PORT ,    DRV8305_Pin_SCS)

//#define DRV8305_SCS_H     GPIO_SetBits(DRV8305_PORT   ,   DRV8305_Pin_SCS)
//#define DRV8305_SCS_L     GPIO_ResetBits(DRV8305_PORT ,    DRV8305_Pin_SCS)
 
//#define READ   



typedef struct  _phase{
                   int   H_duty;
                   int   L_duty;   
}phase;

typedef struct {
                 phase MOTA;
                 phase MOTB;
                 phase MOTC;  
}_duty;

typedef struct{
               _duty  Duty;
}_DRV8305;

extern _DRV8305 DRV8305;
extern u8 Step_Flag;


void DRV8305_Init(void);
void DRV8305_Output(int Duty[6]);
void DRC8305_Driver_Control(float T);
#endif