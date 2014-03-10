#ifndef __CARME_IO1_H__
#define __CARME_IO1_H__
/**
 *****************************************************************************
 * @addtogroup	CARME
 * @{
 * @addtogroup	CARME_IO1
 * @{
 *
 * @file		carme_io1.h
 * @version		1.0
 * @date		2012-12-19
 * @author		rct1
 *
 * @brief		CARME IO1 extension module board support package.
 *
 *****************************************************************************
 * @copyright
 * @{
 *
 * This software can be used by students and other personal of the Bern
 * University of Applied Sciences under the terms of the MIT license.
 * For other persons this software is under the terms of the GNU General
 * Public License version 2.
 *
 * Copyright &copy; 2013, Bern University of Applied Sciences.
 * All rights reserved.
 *
 *
 * ##### MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 *
 * ##### GNU GENERAL PUBLIC LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 *
 * @}
 *****************************************************************************
 */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*----- Header-Files -------------------------------------------------------*/
#include <stm32f4xx.h>				/* Processor STM32F407IG				*/

/*----- Macros -------------------------------------------------------------*/

/*----- Data types ---------------------------------------------------------*/
/**
 * \enum	_CARME_IO1_PIO_PORT
 * \typedef	CARME_IO1_PIO_PORT
 * \brief	PIO Port names.
 */
typedef enum _CARME_IO1_PIO_PORT {
	CARME_IO1_PORTA = 0,	/**< PIO Port A									*/
	CARME_IO1_PORTB = 8,	/**< PIO Port B									*/
	CARME_IO1_PORTC = 16	/**< PIO Port C									*/
} CARME_IO1_PIO_PORT;

/*----- Function prototypes ------------------------------------------------*/
extern void CARME_IO1_Init(void);
extern void CARME_IO1_LED_Set(uint8_t write, uint8_t mask);
extern inline void CARME_IO1_LED_Get(uint8_t *pStatus);
extern inline void CARME_IO1_SWITCH_Get(uint8_t *pStatus);
extern void CARME_IO1_BUTTON_Get(uint8_t *pStatus);
extern void CARME_IO1_PIO_Set(CARME_IO1_PIO_PORT port, uint8_t write,
                              uint8_t mask);
extern inline void CARME_IO1_PIO_Get(CARME_IO1_PIO_PORT port,
                                     uint8_t *pStatus);

/*----- Data ---------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

/**
 * @}
 * @}
 */

#endif /* __CARME_IO1_H__ */
