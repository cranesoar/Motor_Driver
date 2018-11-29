#ifndef _Dbus_H
#define _Dbus_H

#include "board.h"
#include "string.h"

#ifdef  __DBUS_DRIVER_GLOBALS
#define __DBUS_DRIVER_EXT
#else
#define __DBUS_DRIVER_EXT extern
#endif
					
__DBUS_DRIVER_EXT Remote_Control  RC;
__DBUS_DRIVER_EXT int64_t Position_set;


void Dbus_Config(void);
void Handle_data(void);
void DBUS_ButtonCheckJump(uint32_t CurrentTick);
uint8_t DBUS_CheckPush(uint16_t Key);
uint8_t DBUS_CheckJumpKey(uint16_t Key);				
extern int Dbus_Updata_flag;
int Get_Keypress(uint16_t Key);


#endif /*_Dbus_H*/
