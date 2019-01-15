#include "main.h"

#ifndef __MYMATH_H__
#define __MYMATH_H__

#define ABS(x) ( (x)>0?(x):-(x) )
#define LIMIT( x,min,max ) ( (x) < (min)  ? (min) : ( (x) > (max) ? (max) : (x) ) )
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define CONTAIN( x, min, max ) ( (x<min)?(x+2*max):( (x>max)?(x-2*max):(x) ) )

#define TAN_MAP_RES     0.003921569f     /* (smallest non-zero value in table) */
#define RAD_PER_DEG     0.017453293f
#define TAN_MAP_SIZE    256
#define MY_PPPIII       3.14159f
#define MY_PPPIII_HALF  1.570796f

float fast_atan2(float y, float x);
float my_pow(float a);
float my_sqrt(float number);
double mx_sin(double rad);
double my_sin(double rad);
float my_cos(double rad);
float my_deathzoom(float x,float zoom);
float To_180_degrees(float x);

#endif
