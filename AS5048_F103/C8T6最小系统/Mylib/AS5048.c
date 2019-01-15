#include "main.h"
as5048_t as5048_A;
as5048_t as5048_B;
void AS5048_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    AS5048_CS_HIGH;
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    AS5048_1_CS_HIGH;
}
int16_t pRxData;

int as5048_read_angle(void)
{
    uint16_t receive_buffer;
    // 写数据 读数据
    AS5048_CS_LOW;
    pRxData= SPI1_ReadWrite16Bit(AS5048_ANGLE)>>13;
    AS5048_CS_HIGH;
//    delay_us(20);
    AS5048_CS_LOW;
    receive_buffer = SPI1_ReadWrite16Bit(AS5048_ANGLE);
    AS5048_CS_HIGH;
//    delay_us(20);
    AS5048_CS_LOW;
    receive_buffer = SPI1_ReadWrite16Bit(0xc001);
    AS5048_CS_HIGH;
//    delay_us(20);

    as5048_A.reg = receive_buffer&0x3fff;
    as5048_A.ef = (receive_buffer&0x4000)>>13;

    return 1;
}

int as5048_singelread_angle(void)
{
    // 写数据 读数据
//		int16_t receive_buffer;

    AS5048_CS_LOW;
    as5048_A.reg = (uint16_t)(SPI1_ReadWrite16Bit(AS5048_ANGLE)&0x3fff);
    AS5048_CS_HIGH;
	
    AS5048_1_CS_LOW;
    as5048_B.reg = (uint16_t)(SPI2_ReadWrite16Bit(AS5048_ANGLE)&0x3fff);
    AS5048_1_CS_HIGH;

//    as5048.reg = (int16_t)(receive_buffer&0x3fff);
//		as5048.ef = (receive_buffer&0x4000)>>13;

    return 1;
}

void as5048_data_prepare(void)
{
    static int pos_last_A = 0,cnt_A = 0;
    static long long pos_sum_last_A=0;

	  static int pos_last_B = 0,cnt_B = 0;
    static long long pos_sum_last_B=0;

	
    if(as5048_A.reg - pos_last_A <-8000)
        cnt_A ++;
    if(as5048_A.reg - pos_last_A >8000)
        cnt_A --;
    as5048_A.pos_sum = as5048_A.reg + 16384*cnt_A;

    as5048_A.speed = (as5048_A.pos_sum - pos_sum_last_A)/16384.0/0.001f*60;
    as5048_A.speed_ef = LPF2pApply_1(as5048_A.speed);
		as5048_A.speed_jscop = (int16_t)as5048_A.speed_ef * 100;
    pos_last_A = as5048_A.reg;
    pos_sum_last_A = as5048_A.pos_sum;
		
		
    if(as5048_B.reg - pos_last_B <-8000)
        cnt_B ++;
    if(as5048_B.reg - pos_last_B >8000)
        cnt_B --;
    as5048_B.pos_sum = as5048_B.reg + 16384*cnt_B;

    as5048_B.speed = (as5048_B.pos_sum - pos_sum_last_B)/16384.0/0.001f*60;
    as5048_B.speed_ef = LPF2pApply_2(as5048_B.speed);
		as5048_B.speed_jscop = (int16_t)as5048_B.speed_ef * 100;
    pos_last_B = as5048_B.reg;
    pos_sum_last_B = as5048_B.pos_sum;
		
		
}


