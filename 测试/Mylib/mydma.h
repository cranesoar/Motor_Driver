#ifndef __MYDMA_H__
#define	__MYDMA_H__

#include "board.h"

void MYDMA_Config(DMA_Stream_TypeDef *DMA_Streamx,u32 chx,u32 par,u32 mar,u16 ndtr);//配置DMAx_CHx
void MYDMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr);	//使能一次DMA传输		   
void DMA1_Stream3_IRQHandler(void);

#endif
