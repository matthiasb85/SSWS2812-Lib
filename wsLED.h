/*
 * wsLED.h
 *
 *  Created on: 07.10.2014
 *      Author: matthiasb
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
