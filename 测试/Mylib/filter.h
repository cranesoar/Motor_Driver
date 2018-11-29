#ifndef _FILTER_H
#define _FILTER_H

#include "board.h"

#define M_PI_F 3.1415926f

#define KALMAN_Q				 0.03
#define KALMAN_R         10.0000

void LPF2pSetCutoffFreq_1(float sample_freq, float cutoff_freq);
float LPF2pApply_1(float sample);
void LPF2pSetCutoffFreq_2(float sample_freq, float cutoff_freq);
float LPF2pApply_2(float sample);
void LPF2pSetCutoffFreq_3(float sample_freq, float cutoff_freq);
float LPF2pApply_3(float sample);
void LPF2pSetCutoffFreq_4(float sample_freq, float cutoff_freq);
float LPF2pApply_4(float sample);
void LPF2pSetCutoffFreq_5(float sample_freq, float cutoff_freq);
float LPF2pApply_5(float sample);
void LPF2pSetCutoffFreq_6(float sample_freq, float cutoff_freq);
float LPF2pApply_6(float sample);

double KalmanFilter_x(const double ResrcData,double ProcessNiose_Q,double MeasureNoise_R);
#endif
