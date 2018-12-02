#ifndef __MODEL_IDENT_H__
#define __MODEL_IDENT_H__
#include "board.h" 
float model_ident_sin(float low_freq, float high_freq, float step, float run_freq, float repeat);
float model_ident_cos(float low_freq, float high_freq, float step, float run_freq, float repeat);
float model_ident_sin1(float low_freq, float high_freq, float step, float run_freq, float repeat);
extern float SinTable[3600];

#endif
