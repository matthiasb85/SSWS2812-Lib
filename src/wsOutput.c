/*
 * wsOutput.c
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

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"

#include "wsConfig.h"

#if WS_USE_LED_DETECT > 0
/*
 * Cortex-M3 debug register
 */
#define  DWT_CR      *(uint32_t *)0xE0001000
#define  DWT_CYCCNT  *(uint32_t *)0xE0001004
#define  DEM_CR      *(uint32_t *)0xE000EDFC

#define  DEM_CR_TRCENA                   (1 << 24)
#define  DWT_CR_CYCCNTENA                (1 <<  0)

static void _ws_EnableCycleCounter(void) __attribute__((always_inline));
static void _ws_DisableCycleCounter(void) __attribute__((always_inline));
static uint32_t _ws_ReadCycleCounter(void) __attribute__((always_inline));

static inline void _ws_EnableCycleCounter(void)
{
  DEM_CR		|= (uint32_t)DEM_CR_TRCENA;
  DWT_CYCCNT	= (uint32_t)0u;
  DWT_CR		|= (uint32_t)DWT_CR_CYCCNTENA;
}
static inline void _ws_DisableCycleCounter(void)
{
  DEM_CR		&= ~((uint32_t)DEM_CR_TRCENA);
  DWT_CYCCNT	= (uint32_t)0u;
  DWT_CR		&= ~((uint32_t)DWT_CR_CYCCNTENA);
}
static inline uint32_t _ws_ReadCycleCounter(void)
{
  return ((uint32_t)DWT_CYCCNT);
}
#endif

static void _ws_RCC_Init(void)
{
  /* GPIO clock enable */
  WS_OUTPUT_DA_RCC_CMD(WS_OUTPUT_DA_RCC_PERIPH, ENABLE);
#if WS_USE_POWER_ENABLE > 0
  WS_OUTPUT_PE_RCC_CMD(WS_OUTPUT_PE_RCC_PERIPH, ENABLE);
#endif
#if WS_USE_LED_DETECT > 0
  WS_OUTPUT_DT_RCC_CMD(WS_OUTPUT_DT_RCC_PERIPH, ENABLE);
#endif

  /* AFIO clock enable */
  WS_OUTPUT_AFIO_RCC_CMD(WS_OUTPUT_AFIO_RCC_PERIPH, ENABLE);

  /* WS_OUTPUT_TIM clock enable */
  RCC_APB1PeriphClockCmd(WS_OUTPUT_TIM_RCC_PERIPH, ENABLE);
}

static void _ws_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* DA as Data Out */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = WS_OUTPUT_DA_PIN;
  GPIO_Init(WS_OUTPUT_DA_PORT, &GPIO_InitStructure);

#if WS_USE_POWER_ENABLE > 0
  /* PE as power enable */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin = WS_OUTPUT_PE_PIN;
  GPIO_Init(WS_OUTPUT_PE_PORT, &GPIO_InitStructure);
#endif

#if WS_USE_LED_DETECT > 0
  /* DT for LED detection */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_InitStructure.GPIO_Pin = WS_OUTPUT_DT_PIN;
  GPIO_Init(WS_OUTPUT_DT_PORT, &GPIO_InitStructure);
#endif
}

static void _ws_Timer_Init(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;

  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Period = WS_PERIOD;
  TIM_TimeBaseStructure.TIM_Prescaler = WS_PRESCALE;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(WS_OUTPUT_TIM, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration */
  TIM_OCStructInit(&TIM_OCInitStructure);
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = WS_PERIOD+1;
  WS_OUTPUT_TIM_OC_INIT_CMD(WS_OUTPUT_TIM, &TIM_OCInitStructure);

  WS_OUTPUT_TIM_OC_PLOAD_CMD(WS_OUTPUT_TIM, TIM_OCPreload_Enable);
  TIM_ARRPreloadConfig(WS_OUTPUT_TIM, ENABLE);
  TIM_CCxCmd(WS_OUTPUT_TIM, WS_OUTPUT_TIM_CH, TIM_CCx_Enable);

  /* WS_OUTPUT_TIM enable/disable counter */
  TIM_Cmd(WS_OUTPUT_TIM, ENABLE);
}

void _wsOutput_State(FunctionalState State)
{
  /* WS_OUTPUT_TIM enable/disable DMA Req */
  TIM_DMACmd(WS_OUTPUT_TIM, WS_OUTPUT_TIM_DMA_CH, State);
}
void wsOutput_Enable()
{
#if WS_USE_POWER_ENABLE > 0
  GPIO_WriteBit(WS_OUTPUT_PE_PORT, WS_OUTPUT_PE_PIN, WS_OUTPUT_PE_ON);
#endif
  _wsOutput_State(ENABLE);
}

void wsOutput_Disable()
{
#if WS_USE_POWER_ENABLE > 0
  GPIO_WriteBit(WS_OUTPUT_PE_PORT, WS_OUTPUT_PE_PIN, WS_OUTPUT_PE_OFF);
#endif
  _wsOutput_State(DISABLE);
}

void wsOutput_Init()
{
  _ws_RCC_Init();
  _ws_GPIO_Init();
  _ws_Timer_Init();
}

#if WS_USE_LED_DETECT > 0
uint32_t wsOutput_DetectLeds()
{
  RCC_ClocksTypeDef  rcc_clocks;
  RCC_GetClocksFreq(&rcc_clocks);
  uint32_t CpuSpeed = ((uint32_t)rcc_clocks.HCLK_Frequency);
  /*
   * TicksPerUS  = (CpuSpeed/1000)/1000
   * TicksPerLed = TicksPerUS*1.25*24
   *             = TicksPerUS*5/4*24
   *             = TicksPerUS*30
   *             = ((CpuSpeed*30)/1000)/1000
   */

  uint32_t TicksPerLed = ((CpuSpeed*30)/1000)/1000;
  TicksPerLed += WS_OUTPUT_DT_OFFSET;

  wsOutput_Enable();

  /*
   * Setup cycle counter
   */
  _ws_EnableCycleCounter();
  WS_OUTPUT_TIM_SET_CMP_CMD(WS_OUTPUT_TIM,WS_BIT_ZERO);
  /*
   * Start PWM and wait unitl
   * the feedback loop hits the PA05
   */
  while(GPIO_ReadInputDataBit(WS_OUTPUT_DT_PORT, WS_OUTPUT_DT_PIN) != Bit_SET);
  /*
   * Stop cycle counter and
   * get timestamp, disable PWM
   */
  uint32_t Stop = _ws_ReadCycleCounter();
  WS_OUTPUT_TIM_SET_CMP_CMD(WS_OUTPUT_TIM,0);
  wsOutput_Disable();

  return (Stop/TicksPerLed);
}
#endif
