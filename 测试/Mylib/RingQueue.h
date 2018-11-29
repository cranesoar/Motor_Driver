/*
*********************************************************************************************************
*
*
*                                       RingQueueStruct
*                                         环形队列结构
*
* File : RingQueue.h
* By   : Lin Shijun(http://blog.csdn.net/lin_strong)
* Date : 2018/02/23
* version: V1.2
* NOTE(s): 这段程序用来对一个给定的缓冲区进行模拟环形队列的管理
*                   程序本身不会自动分配缓冲区空间,用户需要自己负责分配空间,并且要保证不直接访问缓存区
*                   // 在某处分配内存空间
*                   RQTYPE buffer[BUFFER_SIZE];
*                   RING_QUEUE que,*ptr_que;
*                   unsigned char err;
*                   // 初始化
*                   ptr_que = RingQueueInit(&que,buffer,BUFFER_SIZE,&err);
*                   if(err == RQ_ERR_NONE  ){
*                     // 初始化成功,使用其他函数
*                   }
* History : 2017/04/25   the original version of RingQueueStruct.
*           2017/10/16   put functions used frequently,RingQueueIn and RingQueueOut, in non-banked address;
*                        modify single line function RingQueueIsEmpty and RingQueueIsFull to marco function;
*                        to get better efficiency.
*           2018/02/23   1.add the marco(RQ_ARGUMENT_CHECK_EN) to controll argument check so user can save 
*                          more code.
*                        2.add the ADDRESSING MODE so the buffer can be defined in banked addressing area.
*********************************************************************************************************
*/


#ifndef   RING_QUEUE_H
#define   RING_QUEUE_H

/*
********************************************************************************************
*                                   MISCELLANEOUS
********************************************************************************************
*/

#ifndef  FALSE
#define  FALSE    0
#endif

#ifndef  TRUE
#define  TRUE     1
#endif

/*
*********************************************************************************************************
*                                       ADDRESSING MODE 寻址模式
*********************************************************************************************************
*/

// uncomment the corresponding line to select the addressing mode to the buffer of RingQueue module.
// if you don't understand. Just use the extended addressing mode
// 取消对应行的注释以选择环形缓冲区模块访问缓冲区时使用的寻址方式
// 如果你不知道这是什么意思的话，那就用扩展寻址就行了，这是默认的方式

// extended addressing mode 扩展区寻址(默认)
#define RQ_ADDRESSING_MODE
// banked RAM addressing mode   RAM分页区寻址
//#define RQ_ADDRESSING_MODE __rptr
// global addressing mode   全局寻址
//#define RQ_ADDRESSING_MODE __far

/*
*********************************************************************************************************
*                                       CONFIGURATION  配置
*********************************************************************************************************
*/

#define RQ_ARGUMENT_CHECK_EN    TRUE     // TRUE: arguments will be checked, however,this will 
                                         //       cost a little code volume.

/*
*********************************************************************************************************
*                                        CONSTANTS     常量
*********************************************************************************************************
*/
#define   RQ_ERR_NONE                        0u

#define   RQ_ERR_POINTER_NULL                1u
#define   RQ_ERR_SIZE_ZERO                   2u

#define   RQ_ERR_BUFFER_FULL                 3u
#define   RQ_ERR_BUFFER_EMPTY                4u

#define   RQ_OPTION_WHEN_FULL_DISCARD_FIRST  0u       // discard the first element when ring buffer is full
#define   RQ_OPTION_WHEN_FULL_DONT_IN        1u       // discard new element when ring buffer is full
/*
*********************************************************************************************************
*                                    DATA TYPE    数据类型
*********************************************************************************************************
*/

// define the data type that stores in the RingQueue.       定义存在环形缓冲区内的数据的类型
typedef unsigned char RQTYPE;
typedef RQTYPE *RQ_ADDRESSING_MODE pRQTYPE;
typedef struct {
    unsigned short  RingBufCtr;              /* Number of characters in the ring buffer */
    unsigned short  RingBufSize;             /* Ring buffer Size */    
    pRQTYPE RingBufInPtr;                    /* Pointer to where next character will be inserted        */  
    pRQTYPE RingBufOutPtr;                   /* Pointer from where next character will be extracted     */  
    pRQTYPE RingBuf;                         /* Ring buffer array */  
    pRQTYPE RingBufEnd;                      /* Point to the end of the buffer */
} RING_QUEUE;

/*
*********************************************************************************************************
*                                  FUNCTION PROTOTYPES 函数原型
*********************************************************************************************************
*/

RING_QUEUE *RingQueueInit(RING_QUEUE *pQueue,pRQTYPE pbuf,unsigned short bufSize,unsigned char *perr);
//#pragma CODE_SEG __NEAR_SEG NON_BANKED
unsigned short RingQueueIn(RING_QUEUE *pQueue,RQTYPE data,unsigned char option,unsigned char *perr);
RQTYPE RingQueueOut(RING_QUEUE *pQueue,unsigned char *perr);
//#pragma CODE_SEG DEFAULT
short RingQueueMatch(RING_QUEUE *pQueue,pRQTYPE pbuf,unsigned short len);
void RingQueueClear(RING_QUEUE *pQueue);

/*
*********************************************************************************************************
*                                        RingQueueIsEmpty()
*
* Description :  whether the RingQueue is empty.   环形队列是否为空
*
* Arguments   :  pQueue  pointer to the ring queue control block;     指向环形队列控制块的指针
*
* Return      :  TRUE    the RingQueue is empty.
*                FALSE   the RingQueue is not empty.
* Note(s)     :
*********************************************************************************************************
*/

#define RingQueueIsEmpty(pQueue) ((pQueue)->RingBufCtr == 0)

/*
*********************************************************************************************************
*                                        RingQueueIsFull()
*
* Description : whether the RingQueue is full.    环形队列是否为空
*
* Arguments   : pQueue  pointer to the ring queue control block;   指向环形队列控制块的指针
*
* Return      : TRUE    the RingQueue is full.
*               FALSE   the RingQueue is not full.
* Note(s)     :
*********************************************************************************************************
*/

#define RingQueueIsFull(pQueue)  ((pQueue)->RingBufCtr >= (pQueue)->RingBufSize)

#endif
