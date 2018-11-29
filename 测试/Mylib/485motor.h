#ifndef __485motor_H__
#define __485motor_H__

#include "board.h"

#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
#define SEND_BUF_SIZE 10	//发送数据长度,最好等于sizeof(TEXT_TO_SEND)+2的整数倍.
#define RX_BUF_SIZE 4	//发送数据长度,最好等于sizeof(TEXT_TO_SEND)+2的整数倍.	  	
#define DM50     0

extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	
//如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 bound);
extern u8 RS485_RX_BUF[64]; 		//接收缓冲,最大64个字节
extern u8 RS485_RX_CNT;   			//接收到的数据长度

//模式控制
#define RS485_TX_EN		PCout(2)	//485模式控制.0,接收;1,发送.
//如果想串口中断接收，设置EN_USART2_RX为1，否则设置为0
#define EN_USART3_RX 	1			//0,不接收;1,接收.

extern u8 Send485buf[10]; 
extern u8 rx_rs485buf[10]; 
extern u8 SendBuff[SEND_BUF_SIZE];	//发送数据缓冲区
extern u8 RxBuff[1000];	//发送数据缓冲区
extern long int DM50_POSTION_SUM;	

void RS485_Init(u32 bound);
void RS485_Send_Data(u8 *buf,u8 len);
void RS485_Receive_Data(u8 *buf,u8 *len);		 
void Init_485motor_send(u16 arr,u16 psc);
void TIM3_IRQHandler(void);
void USART3_IRQHandler(void);
#endif 