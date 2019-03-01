#ifndef _FILTER_H_
#define _FILTER_H_

#include "main.h"

#define M_PI_F 3.1415926f

#define KALMAN_Q				 0.03
#define KALMAN_R         10.0000

struct _LPF
{
    float           _cutoff_freq1;
    float           _a11;
    float           _a21;
    float           _b01;
    float           _b11;
    float           _b21;
    float           _delay_element_11;        // buffered sample -1
    float           _delay_element_21;        // buffered sample -2
};
typedef struct _LPF  LPF;
void lpf_k_init(void);
void LPF2pSetCutoffFreq(int index,float sample_freq, float cutoff_freq);
float LPF2pApply(int index,float sample);


struct _Kalman
{
    double x_last;
    double p_last;
};
typedef struct _Kalman  Kalman;

Kalman* GetKalman(int index);
double KalmanFilter_x(Kalman *kalman,const double ResrcData,double ProcessNiose_Q,double MeasureNoise_R);
extern Kalman mKalman[4];
float LoopValue(float value);

#endif
