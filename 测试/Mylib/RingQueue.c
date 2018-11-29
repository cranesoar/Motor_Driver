/*
*********************************************************************************************************
*
*
*                                       RingQueueStruct
*                                         环形队列结构
*
* File : RingQueue.c
* By   : Lin Shijun(http://blog.csdn.net/lin_strong)
* Date : 2018/02/23
* version: V1.2 
* NOTE(s): 
*
* History : 2017/04/25   the original version of RingQueueStruct.
*           2017/10/16   put functions used frequently,RingQueueIn and RingQueueOut, in non-banked address;
*                        modify single line function RingQueueIsEmpty and RingQueueIsFull to marco function;
*                        to get better efficiency.
*           2018/02/23   1.add the marco(RQ_ARGUMENT_CHECK_EN) to controll argument check so user can save 
*                          more code.
*                        2.add the ADDRESSING MODE so the buffer can be defined in banked addressing area.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                     INCLUDES
*********************************************************************************************************
*/
#include "RingQueue.h"

/*
*********************************************************************************************************
*                                LOCAL FUNCTION DECLARATION
*********************************************************************************************************
*/

#if(RQ_ARGUMENT_CHECK_EN == TRUE)
  #define argCheck(cond,err,rVal)  if(cond) { *perr = (err); return (rVal); }
#else
  #define argCheck(cond,err,rVal)
#endif // of (SPI_ARGUMENT_CHECK_EN == TRUE)



/*
*********************************************************************************************************
*                                LOCAL FUNCTION DECLARE
*********************************************************************************************************
*/
#pragma CODE_SEG __NEAR_SEG NON_BANKED
// 内部使用，给定将给定指针在环形缓冲区内向前移动一步(到尾了会移回头)
static void _forwardPointer(RING_QUEUE *pQueue,pRQTYPE* pPointer);
#pragma CODE_SEG DEFAULT
/*
*********************************************************************************************************
*                                        RingQueueInit()
*
* Description : To initialize the ring queue.    初始化环形队列
*
* Arguments   : pQueue   pointer to the ring queue control block;     指向环形队列控制块的指针
*               pbuf     pointer to the buffer(an array);             指向自定义的缓冲区(实际就是个数组)
*               bufSize  the Size of the buffer;                      缓冲区的大小;
*               perr     a pointer to a variable containing an error message which will be set by this
*                          function to either:
*
*                           RQ_ERR_NONE                                       
*                           RQ_ERR_SIZE_ZERO
*                           RQ_ERR_POINTER_NULL
*
* Return      : the pointer to the ring queue control block;        返回指向环形队列控制块的指针
*               0x00 if any error;                                  如果出错了则返回NULL
*
*Note(s):
*********************************************************************************************************
*/

RING_QUEUE* RingQueueInit(RING_QUEUE *pQueue,pRQTYPE pbuf,unsigned short bufSize,unsigned char *perr)
{
  argCheck(pQueue == 0x00 || pbuf == 0x00,RQ_ERR_POINTER_NULL,0x00);
  argCheck(bufSize == 0,RQ_ERR_SIZE_ZERO,0x00);
  pQueue->RingBufCtr = 0;
  pQueue->RingBuf = pbuf;
  pQueue->RingBufInPtr = pbuf;
  pQueue->RingBufOutPtr = pbuf;
  pQueue->RingBufSize = bufSize;
  pQueue->RingBufEnd = pbuf + bufSize;
  *perr = RQ_ERR_NONE;     
  return pQueue;
}

/*
*********************************************************************************************************
*                                        RingQueueIn()
*
* Description : Enqueue an element.    入队一个元素
*
* Arguments   : pQueue   pointer to the ring queue control block;    指向环形队列控制块的指针
*               data     the data to enqueue;                        要入队的数据
*               option   option when queue is full ,you can choose:  当队列满的时候的选项,你可以选择:
*                            RQ_OPTION_WHEN_FULL_DISCARD_FIRST          抛弃队头的元素来填进去新的元素
*                            RQ_OPTION_WHEN_FULL_DONT_IN                不入队新给的元素
*               perr     a pointer to a variable containing an error message which will be set by this
*                          function to either:
*
*                             RQ_ERR_NONE                            if no err happen
*                             RQ_ERR_POINTER_NULL                    if pointer is 0x00
*                             RQ_ERR_BUFFER_FULL                     if buffer is full
*
* Return       : the Elements Count after enqueue the element
*                    调用函数后队列中的元素个数
*Note(s)       :
*********************************************************************************************************
*/
#pragma CODE_SEG __NEAR_SEG NON_BANKED
unsigned short RingQueueIn(RING_QUEUE *pQueue,RQTYPE data,unsigned char option,unsigned char *perr)
{
  argCheck(pQueue == 0x00,RQ_ERR_POINTER_NULL,0x00);
  if(pQueue->RingBufCtr >= pQueue->RingBufSize){
    *perr = RQ_ERR_BUFFER_FULL;     
    if(option == RQ_OPTION_WHEN_FULL_DISCARD_FIRST){
      _forwardPointer(pQueue,&pQueue->RingBufOutPtr); /* Wrap OUT pointer                          */  
    }else{                                            // option == RQ_OPTION_WHEN_FULL_DONT_IN
      return pQueue->RingBufCtr;
    }
  }else{
    pQueue->RingBufCtr++;                             /* No, increment character count            */      
    *perr = RQ_ERR_NONE;
  }
  *pQueue->RingBufInPtr = data;                       /* Put character into buffer                */  
  _forwardPointer(pQueue,&pQueue->RingBufInPtr);      /* Wrap IN pointer                          */  
  return pQueue->RingBufCtr;
}
/*
*********************************************************************************************************
*                                        RingQueueOut()
*
* Description : Dequeue an element.       出队一个元素
*
* Arguments   : pQueue   pointer to the ring queue control block;     指向环形队列控制块的指针
*               perr     a pointer to a variable containing an error message which will be set by this
*                          function to either:
*
*                              RQ_ERR_NONE                            if no err happen
*                              RQ_ERR_POINTER_NULL                    if pointer is 0x00
*                              RQ_ERR_BUFFER_EMPTY                    if buffer is empty
*
* Return      : 0                 if any error or the data is 0;
*               others            the data 
*               
*Note(s):
*********************************************************************************************************
*/
RQTYPE RingQueueOut(RING_QUEUE *pQueue,unsigned char *perr)
{
  RQTYPE data;
  argCheck(pQueue == 0x00,RQ_ERR_POINTER_NULL,0x00);
  if(pQueue->RingBufCtr == 0){
    *perr = RQ_ERR_BUFFER_EMPTY;        
    return 0;
  }
  pQueue->RingBufCtr--;                                      /*  decrement character count           */  
  data = *pQueue->RingBufOutPtr;                      /* Get character from buffer                */  
  _forwardPointer(pQueue,&pQueue->RingBufOutPtr);        /* Wrap OUT pointer                          */  
  *perr = RQ_ERR_NONE;
  return data;
}
#pragma CODE_SEG DEFAULT
/*
*********************************************************************************************************
*                                        RingQueueMatch()
*
* Description : Match the given buffer in RingQueue          在环形队列中匹配给定缓冲区
*
* Arguments   : pQueue   pointer to the ring queue control block;     指向环形队列控制块的指针
*               pbuf     pointer to the chars need to match;
*               len      the length of the chars
* Return      :  -1       Don't match            -1    则没有匹配到
*                >= 0     match                  >= 0  则匹配到了
*
*Note(s):
*********************************************************************************************************
*/

short RingQueueMatch(RING_QUEUE *pQueue,pRQTYPE pbuf,unsigned short len)
{
  pRQTYPE pPosQ,pCurQ,pCurB,pEndB;
  unsigned short rLen,Cnt;
  if(len > pQueue->RingBufCtr)
    return -1;
  pPosQ = pQueue->RingBufOutPtr;
  pEndB = pbuf + len;
  Cnt = 0;
  rLen = pQueue ->RingBufCtr;
  while(rLen-- >= len){          // if remian length of queue bigger than buffer. continue
    pCurQ = pPosQ;
    pCurB = pbuf;
    while(pCurB != pEndB && *pCurQ == *pCurB) {    // compare one by one,until match all(pCurB==pEndB) or some one don't match
      _forwardPointer(pQueue,&pCurQ);
      pCurB++;
    }
    if(pCurB == pEndB)                                                 // if match all
       return Cnt;
     Cnt++;
     _forwardPointer(pQueue,&pPosQ);
  }
  return -1;
}

/*
*********************************************************************************************************
*                                        RingQueueClear()
*
* Description:  Clear the RingQueue.        清空环形队列
*
* Arguments  :  pQueue    pointer to the ring queue control block;     指向环形队列控制块的指针
*
* Return:             
*
* Note(s):
*********************************************************************************************************
*/

void RingQueueClear(RING_QUEUE *pQueue)
{
#if(RQ_ARGUMENT_CHECK_EN == TRUE)
  if(pQueue == 0x00)
    return;
#endif
  pQueue->RingBufCtr = 0;
  pQueue->RingBufInPtr = pQueue -> RingBufOutPtr;
}

/*
*********************************************************************************************************
*                                       LOCAL FUNCTION 
*********************************************************************************************************
*/

#pragma CODE_SEG __NEAR_SEG NON_BANKED
static void _forwardPointer(RING_QUEUE *pQueue,pRQTYPE* pPointer)
{
  if (++*pPointer == pQueue->RingBufEnd)   
    *pPointer = pQueue->RingBuf;        /* Wrap OUT pointer                          */  
}
#pragma CODE_SEG DEFAULT
