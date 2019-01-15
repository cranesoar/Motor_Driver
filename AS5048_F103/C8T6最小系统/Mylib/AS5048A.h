#ifndef __AS5048A_H
#define __AS5048A_H

#include "main.h"

//#define AS5048A_DEBUG

// const int AS5048A_CLEAR_ERROR_FLAG              = 0x0001;
// const int AS5048A_PROGRAMMING_CONTROL           = 0x0003;
// const int AS5048A_OTP_REGISTER_ZERO_POS_HIGH    = 0x0016;
// const int AS5048A_OTP_REGISTER_ZERO_POS_LOW     = 0x0017;
// const int AS5048A_DIAG_AGC                      = 0x3FFD;
// const int AS5048A_MAGNITUDE                     = 0x3FFE;
// const int AS5048A_ANGLE                         = 0x3FFF;

void AS5048A_Init(void);

uint16_t AS5048A_read_always(void);

uint16_t AS5048A_Read_Angle_once(void);

uint16_t AS5048A_Read_Angle_once_16bit(void);

uint16_t AS5048A_read(uint16_t registerAddress);

uint16_t AS5048A_getRawRotation(void);

uint16_t AS5048A_getRotation(void);

uint16_t AS5048A_getErrors(void);

void AS5048A_clear_error(void);

// word AS5048A_getState(void);

// void AS5048A_printState(void);

// byte AS5048A_getGain(void);

// void AS5048A_setZeroPosition(word arg_position);

// word AS5048A_getZeroPosition(void);

// bool AS5048A_error(void);

// word AS5048A_write(word registerAddress, word data);

#endif /* __AS5048A_H */
