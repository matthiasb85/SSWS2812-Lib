/*
 * wsDMA.c
 *
 *  Created on: 07.10.2014
 *      Author: Matthias Beckert <beckert.matthias@googlemail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "stm32f10x.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_rcc.h"
#include "wsConfig.h"

static  DMA_InitTypeDef _wsDMA_PayLoadInit;

void wsDMA_Init(void * pSrc,void *pDest, uint32_t DataLen)
{

	/* Enable WS_DMA_CHANNEL clock */
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

	/* WS_DMA_CHANNEL Config */
	DMA_Init(WS_DMA_CHANNEL, &_wsDMA_PayLoadInit);

	/* WS_DMA_CHANNEL enable */
	DMA_Cmd(WS_DMA_CHANNEL, ENABLE);

}
