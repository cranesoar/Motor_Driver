#ifndef __CAN_H__
#define __CAN_H__
#include "main.h"

typedef struct _canFeedback
{
	int16_t Set_speed_A;
	int16_t Set_speed_B;
} canFeedback_t;

extern canFeedback_t canFeedback;

void CAN_Configuration(void);

#endif
