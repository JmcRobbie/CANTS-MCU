/**
 * @file tctm.c
 *
 */

/**
 * @addtogroup CAN-TS
 * @{
 */

#include "tctm.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

/* TC & TM task related buffers */
static QueueHandle_t tc_queue = NULL;
static StaticQueue_t tc_queue_struct;
static uint8_t tc_queue_buffer[TC_QUEUE_LEN * sizeof(struct cants_msg)];

static QueueHandle_t tm_queue = NULL;
static StaticQueue_t tm_queue_struct;
static uint8_t tm_queue_buffer[TM_QUEUE_LEN * sizeof(struct cants_msg)];

/* TC & TM queue related buffers */
static StaticTask_t tc_task_buffer;
static StackType_t tc_task_stack[TC_STACK_SIZE];

static StaticTask_t tm_task_buffer;
static StackType_t tm_task_stack[TM_STACK_SIZE];

#if CANTS_SEND_KEEPALIVE
/** next UTM channel to send */
static uint8_t utm_ch;

/** keep-alive related configuration */
static struct cants_keepalive_cfg config;

/**
 * @brief Send keep-alive message
 * @retval None
 */
static void cants_send_keepalive(void)
{
	struct cants_msg msg;
	uint8_t ack = 0;

	/* read next telemetry channel to send, skip unavailable/invalid */
	while (!ack) {
		ack = cants_telemetry_handler(utm_ch, &msg.length, msg.data);
		msg.command = utm_ch;
		if (++utm_ch > config.utm_ch_max)
			utm_ch = config.utm_ch_min;
	}

	/* destination address is predefined */
	msg.destination = CANTS_KEEPALIVE_ID;
	msg.source = CANTS_NODE_ID;
	msg.type = cants_type_unsolicited_tm;

	cants_send_msg(&msg, 1);
}
#endif

/**
 * @brief Telemetry task. Handles telemetry requests.
 * @param [in] arg ignored
 * @retval None
 */
static void cants_tm(void *arg)
{
#if CANTS_SEND_KEEPALIVE
    TickType_t wait_time = config.period;
    TimeOut_t timeout;
#endif
	struct cants_msg msg;
	uint8_t ack, channel;

	(void)arg;

#if CANTS_SEND_KEEPALIVE
    vTaskSetTimeOutState(&timeout);
#endif

	while (1) {
#if CANTS_SEND_KEEPALIVE
        if (xQueueReceive(tm_queue, &msg, wait_time)) {
#else
		if (xQueueReceive(tm_queue, &msg, portMAX_DELAY)) {
#endif
			/* Ignore non-telemetry requests or those with data */
			if (msg.type == cants_type_telemetry && msg.length == 0) {
					channel = msg.command & 0xff;
					ack = cants_telemetry_handler(channel, &msg.length, msg.data);
					tctm_send_ack(&msg, ack);
			}
		}

#if CANTS_SEND_KEEPALIVE
		/* send keep-alive packet only if period time expired */
        if (xTaskCheckForTimeOut(&timeout, &wait_time) != pdFALSE) {
        	cants_send_keepalive();

        	/* reset timeout */
            wait_time = config.period;
            vTaskSetTimeOutState(&timeout);
        }
#endif
	}
}

/**
 * @brief Telecommand task. Handles telecommand requests.
 * @param [in] arg ignored
 * @retval None
 */
static void cants_tc(void *arg)
{
	struct cants_msg msg;
	uint8_t ack, channel;

	(void)arg;

	while (1) {
		if (xQueueReceive(tc_queue, &msg, portMAX_DELAY)) {
			/* Ignore non-telecommand messages. At this point, there shouldn't be any. */
			if (msg.type == cants_type_telecommand) {
				channel = msg.command & 0xff;
				ack = cants_telecommand_handler(channel, msg.length, msg.data);
				tctm_send_ack(&msg, ack);
			}
		}
	}
}

void tctm_init(const struct cants_keepalive_cfg *cfg)
{
	config = *cfg;
	utm_ch = config.utm_ch_min;

	/* initialize TC and TM queues */
	tc_queue = xQueueCreateStatic(TC_QUEUE_LEN, sizeof(struct cants_msg),
			tc_queue_buffer, &tc_queue_struct);

	tm_queue = xQueueCreateStatic(TM_QUEUE_LEN, sizeof(struct cants_msg),
			tm_queue_buffer, &tm_queue_struct);

	/* initialize TC and TM tasks */
	xTaskCreateStatic(cants_tc, "CANTSTC", ARRAY_SIZE(tc_task_stack),
			0, TC_PRIORITY, tc_task_stack, &tc_task_buffer);

	xTaskCreateStatic(cants_tm, "CANTSTM", ARRAY_SIZE(tm_task_stack),
			0, TM_PRIORITY, tm_task_stack, &tm_task_buffer);
}

uint8_t tctm_process(struct cants_msg *msg)
{
	/* ignore if it's not TC/TM request */
	if ((msg->command & TCTM_RA_MASK) != TCTM_RA_REQUEST)
		return 0;

	/* dispatch to correct task */
	if (msg->type == cants_type_telemetry)
		return xQueueSendToBack(tm_queue, msg, pdMS_TO_TICKS(10)) != errQUEUE_FULL;

	if (msg->type == cants_type_telecommand)
		return xQueueSendToBack(tc_queue, msg, pdMS_TO_TICKS(10)) != errQUEUE_FULL;

	return 0;
}

void tctm_send_ack(struct cants_msg *msg, uint8_t ack)
{
	/* set source and destination address */
	msg->destination = msg->source;
	msg->source = CANTS_NODE_ID;

	/* set ack or nack flag */
	msg->command &= ~TCTM_RA_MASK;
	msg->command |= ack ? TCTM_RA_ACK : TCTM_RA_NACK;

	/* set size to 0 for telecommands or nack */
	if (msg->type == cants_type_telecommand || !ack)
		msg->length = 0;

	cants_send_msg(msg, 1);
}

/**
 * @}
 */
