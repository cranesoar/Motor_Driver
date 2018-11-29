#include "usart.h"	

// 串口作为Hall反馈 的控制引脚
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
unsigned char rx_buffer[10];

void Hall_init(void)
{
    GPIO_InitTypeDef   GPIO_InitStructure;
	  USART_InitTypeDef USART_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);
   	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	  GPIO_InitStructure.GPIO_OType =GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;//下拉
    GPIO_Init(GPIOB,& GPIO_InitStructure);
	
	 USART_InitStructure.USART_BaudRate = 115200;
   USART_InitStructure.USART_WordLength = USART_WordLength_8b;
   USART_InitStructure.USART_StopBits = USART_StopBits_1;
   USART_InitStructure.USART_Parity = USART_Parity_No;
   USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
   USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	 USART_Init(USART3,&USART_InitStructure);
	 
   USART_Cmd(USART3, ENABLE);
}




void USART_SendChar(char b)
{
  while (USART_GetFlagStatus(USART3,USART_FLAG_TC) == RESET);
	USART_SendData(USART3,b);
}

PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART3, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
  {}
  return ch;
}
