/*
 * wsTypes.h
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

#ifndef WSTYPES_H_
#define WSTYPES_H_

#include "stm32f10x.h"
#include "wsConfig.h"

typedef struct
{
  uint8_t	Red;
  uint8_t	Green;
  uint8_t Blue;
}WS_LED_T;

typedef struct
{
  uint8_t	H;
  uint8_t	S;
  uint8_t	V;
}WS_LED_HSV_T;

typedef enum
{
  WS_COL_RED = 0,
  WS_COL_GREEN,
  WS_COL_BLUE,
  WS_COL_ALL
}WS_COL_ID_T;

#if WS_BIT_ZERO > 255 || WS_BIT_ONE > 255
#warning "Try to avoid WS_BIT_ZERO > 255 || WS_BIT_ONE > 255 in order to decrease RAM usage!"
typedef uint16_t WS_DMA_BIT_SIZE_T;
#define WS_DMA_TRANSFER_T DMA_MemoryDataSize_HalfWord
#else
typedef uint8_t WS_DMA_BIT_SIZE_T;
#define WS_DMA_TRANSFER_T DMA_MemoryDataSize_Byte
#endif

typedef struct
{
#if WS_COLOR_ORDER == 0
  WS_DMA_BIT_SIZE_T Red[8];
  WS_DMA_BIT_SIZE_T Green[8];
  WS_DMA_BIT_SIZE_T Blue[8];
#elif WS_COLOR_ORDER == 1
  WS_DMA_BIT_SIZE_T Green[8];
  WS_DMA_BIT_SIZE_T Red[8];
  WS_DMA_BIT_SIZE_T Blue[8];
#endif
}WS_LED_BITFIELD_T;

#endif /* WSTYPES_H_ */
