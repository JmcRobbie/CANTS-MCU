/*
 * FreeRTOS Kernel V10.2.0
 * Copyright (C) 2019 Skylabs d.o.o. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * 1 tab == 4 spaces!
 */

/*
Changes from V1.0.0

	+ PicoSkyFT port - New port fot PicoSkyFT architercture.
*/

#ifndef PORTMACRO_H
#define PORTMACRO_H

#include "soc.h"
#ifdef NVIC
#include "nvic.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------
 * Port specific definitions.
 *
 * The settings in this file configure FreeRTOS correctly for the
 * given hardware and compiler.
 *
 * These settings should not be altered.
 *-----------------------------------------------------------
 */

/* Type definitions. */
#define portCHAR		char
#define portFLOAT		float
#define portDOUBLE		double
#define portLONG		long
#define portSHORT		int
#define portSTACK_TYPE	uint8_t
#define portBASE_TYPE	char

/* 16 bit pointers */
#define portPOINTER_SIZE_TYPE   uint16_t

typedef portSTACK_TYPE StackType_t;
typedef signed char BaseType_t;
typedef unsigned char UBaseType_t;

#if( configUSE_16_BIT_TICKS == 1 )
	typedef uint16_t TickType_t;
	#define portMAX_DELAY ( TickType_t ) 0xffff
#else
	typedef uint32_t TickType_t;
	#define portMAX_DELAY ( TickType_t ) 0xffffffffUL
#endif
/*-----------------------------------------------------------*/

#ifdef NVIC
extern uint8_t ulSetInterruptMaskFromISR( void );
extern void vClearInterruptMaskFromISR( uint8_t ulMask );

#define portSET_INTERRUPT_MASK_FROM_ISR()		ulSetInterruptMaskFromISR()
#define portCLEAR_INTERRUPT_MASK_FROM_ISR(x)	vClearInterruptMaskFromISR( x )
#endif

/* Critical section management. */
#define portENTER_CRITICAL()		__asm volatile ( "dlq __tmp_reg__, __SREG__" :: );	\
									__asm volatile ( "ci" :: );							\
									__asm volatile ( "push __tmp_reg__" :: )

#define portEXIT_CRITICAL()			__asm volatile ( "pop __tmp_reg__" :: );			\
									__asm volatile ( "dsq __SREG__, __tmp_reg__" :: )

#define portDISABLE_INTERRUPTS()	__asm volatile ( "ci" :: );
#define portENABLE_INTERRUPTS()		__asm volatile ( "si" :: );
/*-----------------------------------------------------------*/

/* Architecture specifics. */
#define portSTACK_GROWTH			( -1 )
#define portTICK_PERIOD_MS			( ( TickType_t ) 1000 / configTICK_RATE_HZ )
#define portBYTE_ALIGNMENT			1
#define portNOP()					__asm volatile ( "nop" );
/*-----------------------------------------------------------*/

/* Kernel utilities. */
#ifdef NVIC
#define portYIELD()					nvic_trigger_int(NVIC)
#else
extern void vPortYield( void ) __attribute__ ( ( naked ) );
#define portYIELD()					vPortYield()
#endif

/*-----------------------------------------------------------*/

/* Task function macros as described on the FreeRTOS.org WEB site. */
#define portTASK_FUNCTION_PROTO( vFunction, pvParameters ) void vFunction( void *pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters ) void vFunction( void *pvParameters )

#define portEND_SWITCHING_ISR( xSwitchRequired )	if( xSwitchRequired )	\
													{						\
														portYIELD();		\
													}

#ifdef __cplusplus
}
#endif

#endif /* PORTMACRO_H */

