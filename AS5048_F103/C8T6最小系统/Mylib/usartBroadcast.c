#include "main.h"

void USART_Broadcast(_dataPrepare* dataStruct)
{
    u8 data_buffer[32];
    u8 i = 0, sum = 0;

    data_buffer[i++] = 0x88;
    data_buffer[i++] = 0xA1;
    data_buffer[i++] = 0x1C;

    for(i = 0; i < 14; i++)
    {
        data_buffer[2*i+3] = BYTE1(dataStruct->data[i]);
        data_buffer[2*i+4] = BYTE0(dataStruct->data[i]);
    }

    for(i = 0; i < 31; i++)
        sum += data_buffer[i];

    data_buffer[i] = sum;

    for(i = 0; i < 32; i++)
        USART2_SendChar(data_buffer[i]);
}

void USART_Send(void)
{
    _dataPrepare dataStruct;

    dataStruct.data[0] = 0;
    dataStruct.data[1] = 0;
    dataStruct.data[2] = 0;
    dataStruct.data[3] = 0;
    dataStruct.data[4] = 0;
    dataStruct.data[5] = 0;
    dataStruct.data[6] = 0;
    dataStruct.data[7] = 0;
    dataStruct.data[8] = 0;
    dataStruct.data[9] = 0;
    dataStruct.data[10] = 0;
    dataStruct.data[11] = 0;
    dataStruct.data[12] = 0;
    dataStruct.data[13] = 0;

    USART_Broadcast(&dataStruct);
}
