/*
 * wsConfig.h
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

#ifndef WSCONFIG_H_
#define WSCONFIG_H_

/* Toplevel USE flags */
#define WS_USE_LED_DETECT			1		/* Detect number of LEDs automatically, connect WS_OUTPUT_DT_PIN to DO of the last LED */
#define WS_USE_DOUBLE_BUF			0		/* Use double buffering if you have problems with aliasing                             */
#define WS_COLOR_ORDER				1		/* Depend on WS2812 version -> 0 RGB, 1 GRB                                            */
#define WS_MAXNUMOFLEDS				(100)	/* Maximum number of supported LEDs                                                    */

/*
 * Two empty leds are enough to generate a synchgap
 * Final buffersizes can be calculated based on the number of leds
 */
#define WS_SYNCHFILLER				(2)
#define WS_MAX_BUF_LEN				(WS_MAXNUMOFLEDS + WS_SYNCHFILLER)
#define WS_BUF_LEN					(sizeof(WS_LED_BITFIELD_T)*(WS_NumOfLeds + WS_SYNCHFILLER))

/*
 * This settings depend on your clock
 * tree configuration and the resulting
 * clock source of the used timer
 */
#define WS_PERIOD					90
#define WS_PRESCALE					0
#define WS_BIT_ZERO					25
#define WS_BIT_ONE					65

/*
 * WS_DMA:
 * Configuration for wsDMA.c
 * */
#define WS_DMA_RCC_CMD				RCC_AHBPeriphClockCmd
#define WS_DMA_RCC_PERIPH			RCC_AHBPeriph_DMA1
#define WS_DMA_CHANNEL				DMA1_Channel6

/*
 * WS_OUTPUT:
 * Configuration for wsOutput.c
 */
#define WS_OUTPUT_TIM				TIM3
#define WS_OUTPUT_TIM_RCC_CMD		RCC_APB1PeriphClockCmd
#define WS_OUTPUT_TIM_RCC_PERIPH	RCC_APB1Periph_TIM3
#define WS_OUTPUT_TIM_CCR			(&((WS_OUTPUT_TIM)->CCR1))
#define WS_OUTPUT_TIM_CH			TIM_Channel_1
#define WS_OUTPUT_TIM_DMA_CH		TIM_DMA_CC1
#define WS_OUTPUT_TIM_OC_INIT_CMD	TIM_OC1Init
#define WS_OUTPUT_TIM_OC_PLOAD_CMD	TIM_OC1PreloadConfig
#define WS_OUTPUT_TIM_SET_CMP_CMD	TIM_SetCompare1

#define WS_OUTPUT_DA_PORT			GPIOA
#define WS_OUTPUT_DA_PIN			GPIO_Pin_6
#define WS_OUTPUT_DA_RCC_CMD		RCC_APB2PeriphClockCmd
#define WS_OUTPUT_DA_RCC_PERIPH		RCC_APB2Periph_GPIOA

#define WS_OUTPUT_AFIO_RCC_CMD		RCC_APB2PeriphClockCmd
#define WS_OUTPUT_AFIO_RCC_PERIPH	RCC_APB2Periph_AFIO

#define WS_OUTPUT_PE_PORT			GPIOA
#define WS_OUTPUT_PE_PIN			GPIO_Pin_7
#define WS_OUTPUT_PE_RCC_CMD		RCC_APB2PeriphClockCmd
#define WS_OUTPUT_PE_RCC_PERIPH		RCC_APB2Periph_GPIOA

#if WS_USE_LED_DETECT > 0
#define WS_OUTPUT_DT_PORT			GPIOA
#define WS_OUTPUT_DT_PIN			GPIO_Pin_8
#define WS_OUTPUT_DT_RCC_CMD		RCC_APB2PeriphClockCmd
#define WS_OUTPUT_DT_RCC_PERIPH		RCC_APB2Periph_GPIOA
#define WS_OUTPUT_DT_OFFSET			30
#endif

#endif /* WSCONFIG_H_ */
