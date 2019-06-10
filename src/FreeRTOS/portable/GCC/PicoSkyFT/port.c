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

#include <stdlib.h>
#include <picosky/interrupt.h>

#include "FreeRTOS.h"
#include "task.h"

#include "timer.h"

/*-----------------------------------------------------------
 * Implementation of functions defined in portable.h for the PicoSkyFT port.
 *----------------------------------------------------------*/

/* Start tasks with interrupts enables. */
#define portFLAGS_INT_ENABLED					( ( StackType_t ) 0x80 )

/*-----------------------------------------------------------*/

/* We require the address of the pxCurrentTCB variable, but don't want to know
any details of its type. */
typedef void TCB_t;
extern volatile TCB_t * volatile pxCurrentTCB;

/*-----------------------------------------------------------*/

/*
 * Macro to save all the general purpose registers, the save the stack pointer
 * into the TCB.
 *
 * The first thing we do is save the flags then disable interrupts.  This is to
 * guard our stack against having a context switch interrupt after we have already
 * pushed the registers onto the stack - causing the 32 registers to be on the
 * stack twice.
 *
 * r1 is set to zero as the compiler expects it to be thus, however some
 * of the math routines make use of R1.
 *
 * The interrupts will have been disabled during the call to portSAVE_CONTEXT()
 * so we need not worry about reading/writing to the stack pointer.
 */


#if defined(__PICOSKY_HAVE_EARX__) && __PICOSKY_HAVE_EARX__
#define     push_earx()     "dlq	r0, __EARX__            \n\t"		\
                            "push	r0                      \n\t"		\
                            "dsq	__EARX__, r1            \n\t"

#define     pop_earx()      "pop r0                         \n\t"       \
                            "dsq	__EARX__, r0            \n\t"
#else
#define     push_earx()     "                               \n\t"
#define     pop_earx()      "                               \n\t"
#endif  /* __PICOSKY_HAVE_EARX__ */


#if defined(__PICOSKY_HAVE_EARY__) && __PICOSKY_HAVE_EARY__
#define     push_eary()     "dlq	r0, __EARY__	        \n\t"		\
                            "push	r0                      \n\t"		\
                            "dsq	__EARY__, r1            \n\t"

#define     pop_eary()      "pop r0                         \n\t"       \
                            "dsq	__EARY__, r0            \n\t"
#else
#define     push_eary()     "                               \n\t"
#define     pop_eary()      "                               \n\t"
#endif  /* __PICOSKY_HAVE_EARY__ */

#if defined(__PICOSKY_HAVE_EARZ__) && __PICOSKY_HAVE_EARZ__
#define     push_earz()     "dlq	r0, __EARZ__	        \n\t"		\
                            "push	r0                      \n\t"		\
                            "dsq	__EARZ__, r1			\n\t"

#define     pop_earz()      "pop r0                         \n\t"       \
                            "dsq	__EARZ__, r0            \n\t"
#else
#define     push_earz()     "                               \n\t"
#define     pop_earz()      "                               \n\t"
#endif  /* __PICOSKY_HAVE_EARZ__ */

#if defined(__PICOSKY_HAVE_EARD__) && __PICOSKY_HAVE_EARD__
#define     push_eard()     "dlq    r0, __EARD__            \n\t"		\
                            "push	r0                      \n\t"		\
                            "dsq	__EARD__, r1            \n\t"

#define     pop_eard()      "pop r0                         \n\t"       \
                            "dsq	__EARD__, r0            \n\t"
#else
#define     push_eard()     "                               \n\t"
#define     pop_eard()      "                               \n\t"
#endif  /* __PICOSKY_HAVE_EARD__ */

#if defined(__PICOSKY_HAVE_EARP__) && __PICOSKY_HAVE_EARP__
#define     push_earp()     "dlq	r0, __EARP__	        \n\t"		\
                            "push	r0                      \n\t"		\
                            "dsq	__EARP__, r1			\n\t"

#define     pop_earp()      "pop r0                         \n\t"       \
                            "dsq	__EARP__, r0            \n\t"
#else
#define     push_earp()     "                               \n\t"
#define     pop_earp()      "                               \n\t"
#endif  /* __PICOSKY_HAVE_EARP__ */

#define portSAVE_CONTEXT()										\
	__asm volatile (	/* Save SREG, r1 and r2 */				\
					"push	r0						\n\t"		\
					"dlq	r0, __SREG__			\n\t"		\
					"ci								\n\t"		\
					"push	r0						\n\t"		\
					"push	r1						\n\t"		\
					"clr	r1						\n\t"		\
					/* Save all 4 EAR	registers */			\
                    /* EARX - SPRF(0x39) */						\
                    push_earx()                                 \
                    /* EARY - SPRF(0x3a) */						\
                    push_eary()                                 \
                    /* EARZ - SPRF(0x3b) */						\
					push_earz()                                 \
					/* EARD - SPRF(0x38) */						\
					push_eard()                                 \
					/* EARP - SPRF(0x3c) */						\
					push_earp()                                 \
					/* Save all other registers */				\
					"push	r2						\n\t"		\
					"push	r3						\n\t"		\
					"push	r4						\n\t"		\
					"push	r5						\n\t"		\
					"push	r6						\n\t"		\
					"push	r7						\n\t"		\
					"push	r8						\n\t"		\
					"push	r9						\n\t"		\
					"push	r10						\n\t"		\
					"push	r11						\n\t"		\
					"push	r12						\n\t"		\
					"push	r13						\n\t"		\
					"push	r14						\n\t"		\
					"push	r15						\n\t"		\
					"push	r16						\n\t"		\
					"push	r17						\n\t"		\
					"push	r18						\n\t"		\
					"push	r19						\n\t"		\
					"push	r20						\n\t"		\
					"push	r21						\n\t"		\
					"push	r22						\n\t"		\
					"push	r23						\n\t"		\
					"push	r24						\n\t"		\
					"push	r25						\n\t"		\
					"push	r26						\n\t"		\
					"push	r27						\n\t"		\
					"push	r28						\n\t"		\
					"push	r29						\n\t"		\
					"push	r30						\n\t"		\
					"push	r31						\n\t"		\
					/* RTOS-dependant values */					\
					"dla	r26, pxCurrentTCB		\n\t"		\
					"dla	r27, pxCurrentTCB + 1	\n\t"		\
					"dlq		r0, __SP_L__		\n\t"		\
					"ds		x+, r0					\n\t"		\
					"dlq		r0, __SP_H__	    \n\t"		\
					"ds		x+, r0					\n\t"		\
				);

/*
 * Opposite to portSAVE_CONTEXT().  Interrupts will have been disabled during
 * the context save so we can write to the stack pointer.
 */

#define portRESTORE_CONTEXT()									\
	__asm volatile (	/* RTOS-dependant values */				\
                    "dla	r26, pxCurrentTCB		\n\t"		\
					"dla	r27, pxCurrentTCB + 1	\n\t"		\
					"dl		r28, x+					\n\t"		\
					"dsq	__SP_L__, r28			\n\t"		\
					"dl		r29, x+					\n\t"		\
					"dsq	__SP_H__, r29			\n\t"		\
					/* All registers except r0 and r1 */		\
					"pop	r31						\n\t"		\
					"pop	r30						\n\t"		\
					"pop	r29						\n\t"		\
					"pop	r28						\n\t"		\
					"pop	r27						\n\t"		\
					"pop	r26						\n\t"		\
					"pop	r25						\n\t"		\
					"pop	r24						\n\t"		\
					"pop	r23						\n\t"		\
					"pop	r22						\n\t"		\
					"pop	r21						\n\t"		\
					"pop	r20						\n\t"		\
					"pop	r19						\n\t"		\
					"pop	r18						\n\t"		\
					"pop	r17						\n\t"		\
					"pop	r16						\n\t"		\
					"pop	r15						\n\t"		\
					"pop	r14						\n\t"		\
					"pop	r13						\n\t"		\
					"pop	r12						\n\t"		\
					"pop	r11						\n\t"		\
					"pop	r10						\n\t"		\
					"pop	r9						\n\t"		\
					"pop	r8						\n\t"		\
					"pop	r7						\n\t"		\
					"pop	r6						\n\t"		\
					"pop	r5						\n\t"		\
					"pop	r4						\n\t"		\
					"pop	r3						\n\t"		\
					"pop	r2						\n\t"		\
					/* Restore all 5 EAR	registers */		\
					/* EARP - SPRF(0x3c) */						\
					pop_earp()	                                \
					/* EARD - SPRF(0x38) */						\
					pop_eard()	                                \
                    /* EARZ - SPRF(0x3b) */						\
					pop_earz()		                            \
                    /* EARY - SPRF(0x3a) */						\
					pop_eary()		                            \
					/* EARX - SPRF(0x39) */						\
					pop_earx()		                            \
					/* Restore other registers */				\
					"pop	r1						\n\t"		\
					"pop	r0						\n\t"		\
					"dsq	__SREG__, r0			\n\t"		\
					"pop	r0						\n\t"		\
				);

/*-----------------------------------------------------------*/

/*
 * Perform hardware setup to enable ticks from timer 1, compare match A.
 */
static void prvSetupTimerInterrupt( void );
/*-----------------------------------------------------------*/

/*
 * See header file for description.
 */
StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters )
{
uint16_t usAddress;

	/* Place a few bytes of known values on the bottom of the stack.
	This is just useful for debugging. */

	*pxTopOfStack = 0x11;
	pxTopOfStack--;
	*pxTopOfStack = 0x22;
	pxTopOfStack--;
	*pxTopOfStack = 0x33;
	pxTopOfStack--;

	/* Simulate how the stack would look after a call to vPortYield() generated by
	the compiler. */

	/*lint -e950 -e611 -e923 Lint doesn't like this much - but nothing I can do about it. */

	/* The start of the task code will be popped off the stack last, so place
	it on first. */
	usAddress = ( uint16_t ) pxCode;
	*pxTopOfStack = ( StackType_t ) ( usAddress & ( uint16_t ) 0x00ff );
	pxTopOfStack--;

	usAddress >>= 8;
	*pxTopOfStack = ( StackType_t ) ( usAddress & ( uint16_t ) 0x00ff );
	pxTopOfStack--;

#if defined(__PICOSKY_3_BYTE_PC__) && __PICOSKY_3_BYTE_PC__
    *pxTopOfStack = 0x00;
    pxTopOfStack--;
#endif  /* __PICOSKY_3_BYTE_PC__ */

	/* Next simulate the stack as if after a call to portSAVE_CONTEXT().
	portSAVE_CONTEXT places the flags on the stack immediately after r0
	to ensure the interrupts get disabled as soon as possible, and so ensuring
	the stack use is minimal should a context switch interrupt occur. */
	*pxTopOfStack = ( StackType_t ) 0x00;	/* R0 */
	pxTopOfStack--;
	*pxTopOfStack = portFLAGS_INT_ENABLED;  /*SREG*/
	pxTopOfStack--;
    /* The compiler expects R1 to be 0. */
	*pxTopOfStack = ( StackType_t ) 0x00;	/* R1 */
	pxTopOfStack--;

	/* Now the remaining registers.   The compiler expects R1 to be 0. */
#  if defined(__PICOSKY_HAVE_EARX__) && __PICOSKY_HAVE_EARX__
    *pxTopOfStack = ( StackType_t ) 0x00;	/* EARX */
	pxTopOfStack--;
#endif  /* __PICOSKY_HAVE_EARX__ */

#  if defined(__PICOSKY_HAVE_EARY__) && __PICOSKY_HAVE_EARY__
    *pxTopOfStack = ( StackType_t ) 0x00;	/* EARY */
	pxTopOfStack--;
#  endif  /* __PICOSKY_HAVE_EARY__ */

#  if defined(__PICOSKY_HAVE_EARZ__) && __PICOSKY_HAVE_EARZ__
    *pxTopOfStack = ( StackType_t ) 0x00;	/* EARY */
	pxTopOfStack--;
#  endif  /* __PICOSKY_HAVE_EARZ__ */

#  if defined(__PICOSKY_HAVE_EARD__) && __PICOSKY_HAVE_EARD__
    *pxTopOfStack = ( StackType_t ) 0x00;	/* EARD */
	pxTopOfStack--;
#  endif  /* __PICOSKY_HAVE_EARD__ */

#  if defined(__PICOSKY_HAVE_EARP__) && __PICOSKY_HAVE_EARP__
    *pxTopOfStack = ( StackType_t ) 0x00;	/* EARP */
	pxTopOfStack--;
#  endif  /* __PICOSKY_HAVE_EARP__ */

    /* Now the remaining registers. */
	*pxTopOfStack = ( StackType_t ) 0x02;	/* R2 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x03;	/* R3 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x04;	/* R4 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x05;	/* R5 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x06;	/* R6 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x07;	/* R7 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x08;	/* R8 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x09;	/* R9 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x10;	/* R10 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x11;	/* R11 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x12;	/* R12 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x13;	/* R13 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x14;	/* R14 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x15;	/* R15 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x16;	/* R16 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x17;	/* R17 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x18;	/* R18 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x19;	/* R19 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x20;	/* R20 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x21;	/* R21 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x22;	/* R22 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x23;	/* R23 */
	pxTopOfStack--;

	/* Place the parameter on the stack in the expected location. */
	usAddress = ( uint16_t ) pvParameters;
	*pxTopOfStack = ( StackType_t ) ( usAddress & ( uint16_t ) 0x00ff );
	pxTopOfStack--;

	usAddress >>= 8;
	*pxTopOfStack = ( StackType_t ) ( usAddress & ( uint16_t ) 0x00ff );
	pxTopOfStack--;

	*pxTopOfStack = ( StackType_t ) 0x26;	/* R26 X */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x27;	/* R27 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x28;	/* R28 Y */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x29;	/* R29 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x30;	/* R30 Z */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x031;	/* R31 */
	pxTopOfStack--;

	/*lint +e950 +e611 +e923 */

	return pxTopOfStack;
}
/*-----------------------------------------------------------*/

BaseType_t xPortStartScheduler( void )
{
	/* Setup the hardware to generate the tick. */
	prvSetupTimerInterrupt();

#ifdef NVIC
	nvic_set_irq_prio(NVIC, NVIC_IRQ_SWI, NVIC_MAX_PRIO);
#endif

	/* Restore the context of the first task that is going to run. */
	portRESTORE_CONTEXT();

	/* Simulate a function call end as generated by the compiler.  We will now
	jump to the start of the task the context of which we have just restored. */
	__asm volatile ( "rts" );

	/* Should not get here. */
	return pdTRUE;
}
/*-----------------------------------------------------------*/

void vPortEndScheduler( void )
{
	/* It is unlikely that the port will get stopped.  If required simply
	disable the tick interrupt here. */
	timer_set_interrupt(TIM0, false);
	timer_enable(TIM0, false);
}
/*-----------------------------------------------------------*/

#ifndef NVIC
/*
 * Manual context switch.  The first thing we do is save the registers so we
 * can use a naked attribute.
 */
void __attribute__((naked)) vPortYield( void )
{
	portSAVE_CONTEXT();
	vTaskSwitchContext();
	portRESTORE_CONTEXT();

	__asm volatile ( "rts" );
}
/*-----------------------------------------------------------*/

/*
 * Context switch function used by the tick.  This must be identical to
 * vPortYield() from the call to vTaskSwitchContext() onwards.  The only
 * difference from vPortYield() is the tick count is incremented as the
 * call comes from the tick ISR.
 */
void __attribute__((naked)) vPortYieldFromTick( void )
{
	portSAVE_CONTEXT();
	if( xTaskIncrementTick() != pdFALSE )
	{
		vTaskSwitchContext();
	}
	portRESTORE_CONTEXT();

	__asm volatile ( "rts" );
}
#endif
/*-----------------------------------------------------------*/

/*
 * Setup timer to generate a tick interrupt.
 */
static void prvSetupTimerInterrupt( void )
{
    timer_set_frequency(TIM0, configTICK_RATE_HZ);
    timer_set_interrupt(TIM0, true);
    timer_enable(TIM0, true);
}
/*-----------------------------------------------------------*/

#ifdef NVIC
uint8_t ulSetInterruptMaskFromISR( void )
{
	uint8_t ulMask;

	__asm__ volatile( "dlq %0, __SREG__" : "=r"(ulMask) ::);
	ci();

	return ulMask;
}

void vClearInterruptMaskFromISR( uint8_t ulMask )
{
	__asm__ volatile( "dsq __SREG__, %0" :: "r"(ulMask) :);
}

ISR(sti_handler, ISR_NAKED)
{
	portSAVE_CONTEXT();
	vTaskSwitchContext();
	portRESTORE_CONTEXT();
	rti();
}

ISR(timer0_handler)
{
	uint8_t ulPreviousMask;

	ulPreviousMask = portSET_INTERRUPT_MASK_FROM_ISR();
	{
		/* Increment the RTOS tick. */
		if( xTaskIncrementTick() != pdFALSE )
		{
			/* Pend a context switch. */
			portYIELD();
		}
	}
	portCLEAR_INTERRUPT_MASK_FROM_ISR( ulPreviousMask );
}
#else
#if configUSE_PREEMPTION == 1

	/*
	 * Tick ISR for preemptive scheduler.  We can use a naked attribute as
	 * the context is saved at the start of vPortYieldFromTick(). The tick
	 * count is incremented after the context is saved.
	 */

	ISR(timer0_handler, ISR_NAKED)
	{
		vPortYieldFromTick();
		rti();
	}

#else

	/*
	 * Tick ISR for the cooperative scheduler.  All this does is increment the
	 * tick count.  We don't need to switch context, this can only be done by
	 * manual calls to taskYIELD();
	 */
	ISR(timer0_handler)
	{
		xTaskIncrementTick();
	}
#endif
#endif



