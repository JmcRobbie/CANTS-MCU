/**
 * @file redundancy.c
 *
 */

/**
 * @addtogroup cants_app CAN-TS application level
 * @{
 */

#include "candrv.h"
#include "cants.h"
#include "FreeRTOS.h"
#include "redundancy.h"
#include "task.h"

/* task related globals */
static StaticTask_t redundancy_task_buffer;
static StackType_t redundancy_task_stack[REDUNDANCY_STACK_SIZE];
static TaskHandle_t task_hdl;

void cants_unsolicited_handler(uint8_t source, uint8_t channel, uint8_t length, uint8_t *data)
{
	(void)channel;
	(void)length;
	(void)data;

	/* notify task we received redundancy master keep-alive message */
	if (source == REDUNDANCY_MASTER_ID)
		xTaskNotifyGive(task_hdl);

	/* If UTM was send directly to us, we have to process it. */
}

/**
 * @brief Redundancy task. It takes care for switching between CAN buses.
 * @retval None
 */
static void redundancy_task(void *arg)
{
	uint8_t misses = 0, switches = 0, bus = 0;
	(void)arg;

	while (1) {
		if (ulTaskNotifyTake(pdTRUE, REDUNDANCY_PERIOD)) {
			/* clear counters if keep-alive message is received */
			misses = 0;
			switches = 0;
		} else if (++misses >= REDUNDANCY_MAX_MISSES) {
			if (switches < REDUNDANCY_MAX_SWITCHES) {
				/* switch CAN bus */
				bus = !bus;
				candrv_set_bus(bus);
			}
			misses = 0;
		}
	}
}

void redundancy_init(void)
{
	/* initialize redundancly handling task */
	task_hdl = xTaskCreateStatic(redundancy_task, "RED", ARRAY_SIZE(redundancy_task_stack),
				NULL, REDUNDANCY_PRIORITY, redundancy_task_stack, &redundancy_task_buffer);
}

/**
 * @}
 */
