/*
    FreeRTOS V8.2.0 - Copyright (C) 2015 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>!AND MODIFIED BY!<< the FreeRTOS exception.

	***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
	***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
	the FAQ page "My application does not run, what could be wrong?".  Have you
	defined configASSERT()?

	http://www.FreeRTOS.org/support - In return for receiving this top quality
	embedded software for free we request you assist our global community by
	participating in the support forum.

	http://www.FreeRTOS.org/training - Investing in training allows your team to
	be as productive as possible as early as possible.  Now you can receive
	FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
	Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html.
 *----------------------------------------------------------*/

#include "soc.h"
#include "Assert.h"

#define configENABLE_BACKWARD_COMPATIBILITY 0

#define configUSE_PREEMPTION		     1
#define configUSE_IDLE_HOOK			     0
#define configUSE_TICK_HOOK			     0
#define configCHECK_FOR_STACK_OVERFLOW   0
#define configUSE_MALLOC_FAILED_HOOK     0
#define configSUPPORT_STATIC_ALLOCATION  1
#define configSUPPORT_DYNAMIC_ALLOCATION 0

#define configCPU_CLOCK_HZ			              SOC_CLOCK
#define configTICK_RATE_HZ			              1000
#define configMAX_PRIORITIES		              3
#define configMINIMAL_STACK_SIZE	              400
#define configTOTAL_HEAP_SIZE		              0
#define configMAX_TASK_NAME_LEN		              8
#define configUSE_TRACE_FACILITY	              0
#define configUSE_16_BIT_TICKS		              0
#define configIDLE_SHOULD_YIELD		              0
#define configQUEUE_REGISTRY_SIZE	              0
#define configUSE_MUTEXES                         0
#define configUSE_TASK_NOTIFICATIONS              1
#define configUSE_QUEUE_SETS                      0
#define configUSE_POSIX_ERRNO                     0
#define configUSE_LIST_DATA_INTEGRITY_CHECK_BYTES 0

/* Define to trap errors during development. */
#define configASSERT Assert
#define traceQUEUE_SEND_FROM_ISR_FAILED(x) Assert(0)
#define traceQUEUE_SEND_FAILED(x) Assert(0)

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES 		    0
#define configMAX_CO_ROUTINE_PRIORITIES 2

/* Software timer related definitions. */
#define configUSE_TIMERS             0
#define configTIMER_TASK_PRIORITY    2
#define configTIMER_QUEUE_LENGTH     10
#define configTIMER_TASK_STACK_DEPTH configMINIMAL_STACK_SIZE

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet             0
#define INCLUDE_uxTaskPriorityGet            0
#define INCLUDE_vTaskDelete                  0
#define INCLUDE_vTaskSuspend                 0
#define INCLUDE_vTaskDelayUntil              0
#define INCLUDE_vTaskDelay                   0
#define INCLUDE_xTaskGetIdleTaskHandle       0
#define INCLUDE_xTaskAbortDelay              0
#define INCLUDE_xQueueGetMutexHolder         0
#define INCLUDE_xSemaphoreGetMutexHolder     0
#define INCLUDE_xTaskGetHandle               0
#define INCLUDE_uxTaskGetStackHighWaterMark  0
#define INCLUDE_uxTaskGetStackHighWaterMark2 0
#define INCLUDE_eTaskGetState                0
#define INCLUDE_xTaskResumeFromISR           0
#define INCLUDE_xTimerPendFunctionCall       0
#define INCLUDE_xTaskGetSchedulerState       0
#define INCLUDE_xTaskGetCurrentTaskHandle    0

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

#endif /* FREERTOS_CONFIG_H */
