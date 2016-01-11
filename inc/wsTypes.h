/*
 * wsTypes.h
 *
 *  Created on: 07.10.2014
 *      Author: matthiasb
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

typedef struct
{
#if WS_COLOR_ORDER == 0
	uint16_t Red[8];
	uint16_t Green[8];
	uint16_t Blue[8];
#elif WS_COLOR_ORDER == 1
	uint16_t Green[8];
	uint16_t Red[8];
	uint16_t Blue[8];
#endif
}WS_LED_BITFIELD_T;

#endif /* WSTYPES_H_ */
