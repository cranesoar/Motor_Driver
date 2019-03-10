#include "Filter.h"

static LPF lpf4[10];

#define N 50




void lpf_k_init(void)
{

    LPF2pSetCutoffFreq(0,5000,1);
    LPF2pSetCutoffFreq(1,2000,2);
    LPF2pSetCutoffFreq(2,200,5);
//    LPF2pSetCutoffFreq(3,1000,10);
//    LPF2pSetCutoffFreq(4,1000,10);
//    LPF2pSetCutoffFreq(5,1000,100);
//    LPF2pSetCutoffFreq(6,2000,5);
}


void LPF2pSetCutoffFreq(int index,float sample_freq, float cutoff_freq)
{
    LPF* lpf;
    float fr =0;
    float ohm =0;
    float c =0;  
    lpf=&lpf4[index];


    fr= sample_freq/cutoff_freq;
    ohm=tanf(M_PI_F/fr);
    c=1.0f+2.0f*cosf(M_PI_F/4.0f)*ohm + ohm*ohm;

    lpf->_cutoff_freq1 = cutoff_freq;
    if (lpf->_cutoff_freq1 > 0.0f)
    {
        lpf->_b01 = ohm*ohm/c;
        lpf->_b11 = 2.0f*lpf->_b01;
        lpf->_b21 = lpf->_b01;
        lpf->_a11 = 2.0f*(ohm*ohm-1.0f)/c;
        lpf->_a21 = (1.0f-2.0f*cosf(M_PI_F/4.0f)*ohm+ohm*ohm)/c;
    }
}



float LPF2pApply(int index,float sample)
{
    LPF* lpf;
    float delay_element_0 = 0, output=0;  
    lpf=&lpf4[index];
  
    if (lpf->_cutoff_freq1 <= 0.0f) {
        // no filtering
        return sample;
    }
    else
    {
        delay_element_0 = sample - lpf->_delay_element_11 * lpf->_a11 - lpf->_delay_element_21 * lpf->_a21;
        // do the filtering
        if (isnan(delay_element_0) || isinf(delay_element_0)) {
            // don't allow bad values to propogate via the filter
            delay_element_0 = sample;
        }
        output = delay_element_0 * lpf->_b01 + lpf->_delay_element_11 * lpf->_b11 + lpf->_delay_element_21 * lpf->_b21;

        lpf->_delay_element_21 = lpf->_delay_element_11;
        lpf->_delay_element_11 = delay_element_0;

        // return the value.  Should be no need to check limits
        return output;
    }
}
