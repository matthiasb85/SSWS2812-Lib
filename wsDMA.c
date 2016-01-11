/*
 * wsDMA.c
 *
 *  Created on: 07.10.2014
 *      Author: matthiasb
 */

#include "stm32f10x.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_rcc.h"
#include "wsConfig.h"

static  DMA_InitTypeDef _wsDMA_PayLoadInit;

void wsDMA_Init(void * pSrc,void *pDest, uint32_t DataLen)
{

	/* Enable DMA1 clock */
	WS_DMA_RCC_CMD(WS_DMA_RCC_PERIPH, ENABLE);

	DMA_DeInit(WS_DMA_CHANNEL);
	DMA_StructInit(&_wsDMA_PayLoadInit);
	_wsDMA_PayLoadInit.DMA_PeripheralBaseAddr = (uint32_t)pDest;
	_wsDMA_PayLoadInit.DMA_MemoryBaseAddr = (uint32_t)pSrc;
	_wsDMA_PayLoadInit.DMA_DIR = DMA_DIR_PeripheralDST;
	_wsDMA_PayLoadInit.DMA_BufferSize = DataLen;
	_wsDMA_PayLoadInit.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	_wsDMA_PayLoadInit.DMA_MemoryInc = DMA_MemoryInc_Enable;
	_wsDMA_PayLoadInit.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	_wsDMA_PayLoadInit.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	_wsDMA_PayLoadInit.DMA_Priority = DMA_Priority_High;
	_wsDMA_PayLoadInit.DMA_M2M = DMA_M2M_Disable;
	_wsDMA_PayLoadInit.DMA_Mode = DMA_Mode_Circular;

	/* DMA1 Channel6 Config */
	DMA_Init(WS_DMA_CHANNEL, &_wsDMA_PayLoadInit);

	/* DMA1 Channel6 enable */
	DMA_Cmd(WS_DMA_CHANNEL, ENABLE);

}
