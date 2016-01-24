/*
 * wsLED.c
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
 * The code for the HSV -> RGB conversion was inspired by:
 * https://stackoverflow.com/posts/14733008/revisions
 *
 */

#include "stm32f10x.h"
#include "string.h"
#include "wsConfig.h"
#include "wsTypes.h"
#include "wsOutput.h"
#include "wsDMA.h"

#if WS_USE_DOUBLE_BUF > 0
WS_LED_T wsLED_LedBuffer[WS_MAXNUMOFLEDS];
#endif
WS_LED_BITFIELD_T wsLED_BitBuffer[WS_MAX_BUF_LEN];
uint32_t WS_NumOfLeds = 0;

static void _wsLED_LedToPWM(WS_LED_T *pSrc, WS_LED_BITFIELD_T *pDest)
{
  unsigned int i = 0;
  for(i=0; i < 8; i++)
  {
    pDest->Red[i] = (pSrc->Red & (1<<(8-i))) ?
        WS_BIT_ONE : WS_BIT_ZERO;
    pDest->Green[i] = (pSrc->Green & (1<<(8-i))) ?
        WS_BIT_ONE : WS_BIT_ZERO;
    pDest->Blue[i] = (pSrc->Blue & (1<<(8-i))) ?
        WS_BIT_ONE : WS_BIT_ZERO;
  }
}

#if WS_USE_DOUBLE_BUF == 0
static void _wsLED_PWMToLed(WS_LED_T *pDest, WS_LED_BITFIELD_T *pSrc)
{
  unsigned int i = 0;
  for(i=0; i < 8; i++)
  {
    pDest->Red = (pSrc->Red[i] == WS_BIT_ONE) ?
        (pDest->Red | (1<<(8-i))) : pDest->Red;
    pDest->Green = (pSrc->Green[i] == WS_BIT_ONE) ?
        (pDest->Green | (1<<(8-i))) : pDest->Green;
    pDest->Blue = (pSrc->Blue[i] == WS_BIT_ONE) ?
        (pDest->Blue | (1<<(8-i))) : pDest->Blue;
  }
}
#endif

static void _wsLED_SetLed_Hsv(WS_LED_HSV_T *pHSV,WS_LED_T * pLed)
{
  unsigned char region, remainder, p, q, t;

  if (pHSV->S == 0)
  {
    pLed->Red = pHSV->V;
    pLed->Green = pHSV->V;
    pLed->Blue = pHSV->V;
    return ;
  }

  region = pHSV->H / 43;
  remainder = (pHSV->H - (region * 43)) * 6;

  p = (pHSV->V * (255 - pHSV->S)) >> 8;
  q = (pHSV->V * (255 - ((pHSV->S * remainder) >> 8))) >> 8;
  t = (pHSV->V * (255 - ((pHSV->S * (255 - remainder)) >> 8))) >> 8;

  switch (region)
  {
  case 0:
    pLed->Red = pHSV->V; pLed->Green = t; pLed->Blue = p;
    break;
  case 1:
    pLed->Red = q; pLed->Green = pHSV->V; pLed->Blue = p;
    break;
  case 2:
    pLed->Red = p; pLed->Green = pHSV->V; pLed->Blue = t;
    break;
  case 3:
    pLed->Red = p; pLed->Green = q; pLed->Blue = pHSV->V;
    break;
  case 4:
    pLed->Red = t; pLed->Green = p; pLed->Blue = pHSV->V;
    break;
  default:
    pLed->Red = pHSV->V; pLed->Green = p; pLed->Blue = q;
    break;
  }

  return ;
}

void wsLED_SetLed(WS_LED_T *pLed,uint8_t Index)
{
#if WS_USE_DOUBLE_BUF > 0
  wsLED_LedBuffer[Index].Red = pLed->Red;
  wsLED_LedBuffer[Index].Blue = pLed->Blue;
  wsLED_LedBuffer[Index].Green = pLed->Green;
#else
  _wsLED_LedToPWM(pLed, &wsLED_BitBuffer[Index]);
#endif
}
void wsLED_GetLed(WS_LED_T *pLed,uint8_t Index)
{
#if WS_USE_DOUBLE_BUF > 0
  pLed->Red = wsLED_LedBuffer[Index].Red;
  pLed->Blue = wsLED_LedBuffer[Index].Blue;
  pLed->Green = wsLED_LedBuffer[Index].Green;
#else
  _wsLED_PWMToLed(pLed,&wsLED_BitBuffer[Index]);
#endif
}

void wsLED_MixLed(WS_LED_T *pLed,uint8_t Index)
{
  WS_LED_T NewLed;
  WS_LED_T OldLed;

  wsLED_GetLed(&OldLed,Index);

  NewLed.Red = (((uint16_t)(OldLed.Red + pLed->Red)) > 255 ) ? 255 : OldLed.Red + pLed->Red;
  NewLed.Blue = (((uint16_t)(OldLed.Blue + pLed->Blue)) > 255 ) ? 255 : OldLed.Blue + pLed->Blue;
  NewLed.Green = (((uint16_t)(OldLed.Green + pLed->Green)) > 255 ) ? 255 : OldLed.Green + pLed->Green;

  wsLED_SetLed(&NewLed,Index);

}

void wsLED_SetLed_Hsv(WS_LED_HSV_T *pHSV,uint8_t Index)
{
  WS_LED_T NewLed;
  _wsLED_SetLed_Hsv(pHSV,&NewLed);
  wsLED_SetLed(&NewLed,Index);
}

void wsLED_MixLed_Hsv(WS_LED_HSV_T *pHSV,uint8_t Index)
{
  WS_LED_T Led;
  _wsLED_SetLed_Hsv(pHSV,&Led);
  wsLED_MixLed(&Led, Index);
}

void wsLED_TestAndSetLed_Hsv(WS_LED_HSV_T *pHSV,uint8_t Index)
{
  WS_LED_T NewLed;
  _wsLED_SetLed_Hsv(pHSV,&NewLed);
  if(!(NewLed.Blue || NewLed.Red || NewLed.Green))
  {
    _wsLED_SetLed_Hsv(pHSV,&NewLed);
    wsLED_SetLed(&NewLed,Index);
  }

}

void wsLED_SetAllLed_Hsv(WS_LED_HSV_T *pHSV)
{
  unsigned int i = 0;
  for(i=0; i < WS_NumOfLeds; i++)
    wsLED_SetLed_Hsv(pHSV,i);
}

void wsLED_SetAllLed(WS_LED_T *pLed)
{
  unsigned int i = 0;
  for(i=0; i < WS_NumOfLeds; i++)
    wsLED_SetLed(pLed,i);
}

void wsLED_ClearLed(uint8_t Index)
{
  WS_LED_T Dummy = {0,0,0};
  wsLED_SetLed(&Dummy,Index);
}

void wsLED_ClearAllLed(void)
{
  WS_LED_T Dummy = {0,0,0};
  wsLED_SetAllLed(&Dummy);
}

void wsLED_UpdateStripe(void)
{
#if WS_USE_DOUBLE_BUF > 0
unsigned int i = 0;
  for(i=0; i < WS_NumOfLeds; i++)
  {
    _wsLED_LedToPWM(&wsLED_LedBuffer[i], &wsLED_BitBuffer[i]);
  }
#endif
}

void wsLED_Init(void)
{
#if WS_USE_DOUBLE_BUF > 0
  memset(wsLED_LedBuffer,0,sizeof(wsLED_LedBuffer));
#endif
  memset(wsLED_BitBuffer,0,sizeof(wsLED_BitBuffer));

  wsLED_ClearAllLed();
  wsLED_UpdateStripe();

  wsOutput_Init();

#if WS_USE_LED_DETECT > 0
  WS_NumOfLeds = wsOutput_DetectLeds();
  WS_NumOfLeds = (WS_NumOfLeds > WS_MAXNUMOFLEDS) ? WS_MAXNUMOFLEDS : WS_NumOfLeds;
#else
  WS_NumOfLeds = WS_MAXNUMOFLEDS;
#endif

  wsDMA_Init((void *)wsLED_BitBuffer, (void *)WS_OUTPUT_TIM_CCR,(WS_BUF_LEN/sizeof(WS_DMA_BIT_SIZE_T)));
  wsOutput_Disable();
}
