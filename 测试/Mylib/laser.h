#ifndef __LASER_H__
#define __LASER_H__

#include "board.h"

#define  LASER2_ON      GPIOA->BSRRL = GPIO_Pin_15
#define  LASER2_OFF     GPIOA->BSRRH = GPIO_Pin_15

void Laser_Init(void);

#endif
