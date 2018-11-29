#ifndef _IMU_H
#define	_IMU_H
#include "board.h"

#define IMU_INTEGRAL_LIM  ( 2.0f *ANGLE_TO_RAD )

#define RtA 	  57.324841f

typedef struct _eulerAngle{
														float pitch;
														float roll;
														float yaw;
	                          float lastyaw;
                            float yawsum;
													}eulerAngle;


extern eulerAngle gimbalTopAngle;
extern eulerAngle gimbalBotAngle;
extern float Position_GyroYaw;
void TDT_IMUTopupdate(float half_T, vec3f* gyro, vec3f* acc);
void TDT_IMUBotupdate(float half_T, vec3f* gyro, vec3f* acc);

#endif
