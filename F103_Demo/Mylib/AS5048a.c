#include "main.h"
as5048_t as5048_A;
as5048_t as5048_B;
void AS5048_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
  
	  SPI2_Init();
  
    RCC_APB2PeriphClockCmd(AS5048_CS_RCC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = AS5048_CS_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(AS5048_CS_PORT,&GPIO_InitStructure);
    AS5048_CS_HIGH;
}
int16_t pRxData;

int as5048_singelread_angle(void)
{
    // 写数据 读数据
//		int16_t receive_buffer;

    AS5048_CS_LOW;
    as5048_A.reg = (uint16_t)(SPI2_ReadWrite16Bit(AS5048_ANGLE)&0x3fff);
    AS5048_CS_HIGH;
    
//    as5048.reg = (int16_t)(receive_buffer&0x3fff);
//		as5048.ef = (receive_buffer&0x4000)>>13;

    return 1;
}

void as5048_data_prepare(void)
{
	static int Point, Last_Point;
  static _Bool zero_flag=0;
  
  Point = as5048_A.reg;
  if(zero_flag==0)
  {
  Last_Point = Point;
  zero_flag=1;
  }
  if((Point - Last_Point) < -8192)
  as5048_A.speed_jscop = Point - Last_Point + 16383;
  else if ((Point - Last_Point) > 8192)
  as5048_A.speed_jscop = Point - Last_Point - 16383; 
  else
  as5048_A.speed_jscop = Point - Last_Point;   
   
  Last_Point = Point;  
	
  DRV8305.Speed.Feedback = as5048_A.speed_jscop;	
}


