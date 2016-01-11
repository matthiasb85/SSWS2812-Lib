/*
 * wsLED.h
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

#ifndef WSLED_H_
#define WSLED_H_

#include "wsConfig.h"
#include "wsTypes.h"

extern void wsLED_Init(void);
extern void wsLED_SetLed_Hsv(WS_LED_HSV_T *pHSV,uint8_t Index);
extern void wsLED_TestAndSetLed_Hsv(WS_LED_HSV_T *pHSV,uint8_t Index);
extern void wsLED_SetAllLed_Hsv(WS_LED_HSV_T *pHSV);
extern void wsLED_MixLed_Hsv(WS_LED_HSV_T *pHSV,uint8_t Index);
extern void wsLED_SetLed(WS_LED_T *pLed,uint8_t Index);
extern void wsLED_GetLed(WS_LED_T *pLed,uint8_t Index);
extern void wsLED_SetAllLed(WS_LED_T *pLed);
extern void wsLED_MixLed(WS_LED_T *pLed,uint8_t Index);
extern void wsLED_ClearLed(uint8_t Index);
extern void wsLED_ClearAllLed(void);
extern void wsLED_UpdateStripe(void);

extern uint32_t WS_NumOfLeds;

#endif /* WSLED_H_ */
