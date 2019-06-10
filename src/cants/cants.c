/**
 * @file cants.h
 *
 */

/**
 * @defgroup CAN-TS
 * @brief CAN-TS stack
 * @{
 */

#include "block.h"
#include "cants.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "tctm.h"

/* dispatcher queue related variables */
static QueueHandle_t dispatcher_queue = NULL;
static StaticQueue_t dispatcher_queue_struct;
static uint8_t dispatcher_queue_buffer[DISPATCHER_QUEUE_LEN * sizeof(struct cants_msg)];

/* dispatcher task related variables */
static StaticTask_t dispatcher_task_buffer;
static StackType_t dispatcher_task_stack[DISPATCHER_STACK_SIZE];

uint8_t cants_dispatch_isr(struct cants_msg *msg)
{
	BaseType_t yield = pdFALSE;

#if !CAN_HW_FILTERING
	/* validate destination IDs */
	if (msg->destination != CANTS_NODE_ID &&
		(msg->destination != CANTS_TIME_ID || msg->type != cants_type_time_sync) &&
		(msg->destination != CANTS_KEEPALIVE_ID || msg->type != cants_type_unsolicited_tm))
		return 0;
#endif

	xQueueSendToBackFromISR(dispatcher_queue, msg, &yield);

	return !!yield;
}

/**
 * @brief Dispatcher task. Dispatches messages to appropriate handlers.
 * @param [in] arg ignored
 * @retval None
 */
static void cants_dispatcher(void *arg)
{
	uint8_t tctm_nack, block_nack;
	struct cants_msg msg;

	(void)arg;

	while (1) {
		if (xQueueReceive(dispatcher_queue, &msg, portMAX_DELAY)) {
			tctm_nack = 0;
			block_nack = 0;

			/*
			 * Dispatch messages to appropriate handlers. Only UTM and TS
			 * messages are handled directly in this task.
			 */
			switch (msg.type) {
			case cants_type_time_sync:
				cants_time_sync_handler(msg.length, msg.data);
				break;
			case cants_type_unsolicited_tm:
				cants_unsolicited_handler(msg.source, msg.command, msg.length, msg.data);
				break;
			case cants_type_telecommand:
			case cants_type_telemetry:
				tctm_nack = !tctm_process(&msg);
				break;
			case cants_type_set_block:
			case cants_type_get_block:
				block_nack = !block_process(&msg);
				break;
			}

			/* if appropriate queue is full, send nack directly */
			if (tctm_nack)
				tctm_send_ack(&msg, 0);

			if (block_nack)
				block_send_ack(&msg, 0, 1);
		}
	}
}


void cants_init(const struct cants_keepalive_cfg *cfg)
{
	/* initialize TC, TM and block transfer part of stack */
	block_init();
	tctm_init(cfg);

	/* initialize dispatcher queue */
	dispatcher_queue = xQueueCreateStatic(DISPATCHER_QUEUE_LEN, sizeof(struct cants_msg),
		dispatcher_queue_buffer, &dispatcher_queue_struct);

	/* initialize dispatcher task */
	xTaskCreateStatic(cants_dispatcher, "CANTSDISP", ARRAY_SIZE(dispatcher_task_stack),
		0, DISPATCHER_PRIORITY, dispatcher_task_stack, &dispatcher_task_buffer);
}

/**
 * @}
 */
