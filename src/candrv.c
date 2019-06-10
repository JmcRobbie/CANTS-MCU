/**
 * @file candrv.c
 *
 */

/**
 * @defgroup CanIntf CAN Interface
 * @brief Takes care for CAN configuration and interfacing it with CAN-TS stack
 * @{
 */

#include <picosky/interrupt.h>

#include "can.h"
#include "candrv.h"
#include "cants.h"
#include "FreeRTOS.h"
#include "redundancy.h"
#include "gpio.h"
#include "queue.h"
#include "soc.h"
#include "task.h"

/*
 * Some SoC variants have two CAN busses and two CAN controllers,
 * while others have two CAN busses and single cAN controller. In
 * such cases bus can be selected usually by toggling GPIO pin or
 * bit in SFCR register. Basic SoC have dual CAN controllers.
 */
#define DUAL_CAN_CONTROLLER 1

/* CAN-TS keep-alive TX settings */
static const struct cants_keepalive_cfg cants_cfg = {
	0,    /* utm_ch_min */
	0,    /* utm_ch_max */
	2000, /* period */
};

/* CAN trasmission queue related variables */
static QueueHandle_t can_send_queue = NULL;
static StaticQueue_t can_send_queue_struct;
static uint8_t can_send_queue_buffer[CAN_SEND_QUEUE_LEN * sizeof(struct cants_msg)];

/* holds reference to currently active CAN controller */
static struct can *current_ctrl;

/**
 * @brief Initializes CAN controller
 * @param [in] base base address of CAN controller
 * @retval None
 */
static void candrv_init_ctrl(struct can *base)
{
	can_reset_mode(base, true);
	can_set_baudrate(base, 1000000, 0);
	can_set_rx_filter(base, 0, 0x7EUL << 21, true);
	can_enable_interrupt(base, CAN_IRQ_RI | CAN_IRQ_TI | CAN_IRQ_BEI | CAN_IRQ_EPI | CAN_IRQ_EI, true);
	can_reset_mode(base, false);
}

/**
 * @brief CAN interrupt handler
 * @param [in] base base address of CAN controller
 * @retval None
 */
static void can_int_handler(struct can *base)
{
	uint8_t active = current_ctrl == base;
	uint8_t ir = can_get_int_status(base);
	BaseType_t yield = pdFALSE;
	struct cants_msg msg;

	/* Transmit buffer in CAN controller is empty, so we can send new message */
	if (active && (ir & CAN_IRQ_TI)) {
		if (xQueueReceiveFromISR(can_send_queue, &msg, &yield) == pdTRUE)
			can_send_packet(base, cants_construct_id(&msg), msg.length, msg.data, 0, true);
	}

	/* Message from CAN bus has been received */
	if (ir & CAN_IRQ_RI) {
		bool ext, rtr;
		uint32_t id;

		/* read all messages from FIFO */
		while (can_get_status(base) & CAN_SR_RBS) {
			can_recv_packet(base, &id, &msg.length, msg.data, &rtr, &ext);

			/* validate CAN frame */
			if (active && ext && !rtr) {
				cants_parse_id(&msg, id);
				yield |= cants_dispatch_isr(&msg);
			}
		}
	}

	/* reinitialize controller in case of error */
	if (ir & (CAN_IRQ_BEI | CAN_IRQ_EPI | CAN_IRQ_EI))
		candrv_init_ctrl(base);

	portEND_SWITCHING_ISR(yield);
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS

ISR(can0_handler) {
	can_int_handler(CAN0);
}

#if DUAL_CAN_CONTROLLER
ISR(can1_handler) {
	can_int_handler(CAN1);
}
#endif

#endif

void candrv_set_bus(uint8_t bus)
{
#if DUAL_CAN_CONTROLLER
	/* critical section is necessary because pointer size is bigger than word size */
	taskENTER_CRITICAL();
	current_ctrl = bus ? CAN1 : CAN0;
	taskEXIT_CRITICAL();
#else
#error "Add logic to switch to another bus. This is usually done by toggling appropriate GPIO or SFCR bit."
#endif
}

uint8_t cants_send_msg(struct cants_msg *msg, uint8_t wait_allowed)
{
	TickType_t wait_time = wait_allowed ? pdMS_TO_TICKS(10) : 0;
	uint8_t ret = 1;

	/* if TX buffer is empty, send CAN frame directly, otherwise put in queue */
	taskENTER_CRITICAL();
	if (can_get_status(current_ctrl) & CAN_SR_TBS)
		can_send_packet(current_ctrl, cants_construct_id(msg), msg->length, msg->data, 0, true);
	else if (xQueueSendToBack(can_send_queue, msg, wait_time) == errQUEUE_FULL)
		ret = 0;
	taskEXIT_CRITICAL();

	return ret;
}

void candrv_init(void)
{
	/* initialize CAN output queue */
	can_send_queue = xQueueCreateStatic(CAN_SEND_QUEUE_LEN, sizeof(struct cants_msg),
						 can_send_queue_buffer, &can_send_queue_struct);

	/* initialize redundancy mechanism */
	redundancy_init();

	/* init CAN-TS stack */
	cants_init(&cants_cfg);

	/* init CAN pins */
	gpio_set_function(GPIO0, GPIO_PIN0, GPIO0_PIN0_FUNC_CAN0_N);
	gpio_set_function(GPIO0, GPIO_PIN2, GPIO0_PIN2_FUNC_CAN0_P);
	gpio_set_function(GPIO0, GPIO_PIN1, GPIO0_PIN1_FUNC_CAN1_N);
	gpio_set_function(GPIO0, GPIO_PIN3, GPIO0_PIN3_FUNC_CAN1_P);

	/* set primary bus */
	candrv_set_bus(0);

#if !DUAL_CAN_CONTROLLER
	current_ctrl = CAN0;
#endif

	/* initialize CAN controller(s) */
	candrv_init_ctrl(CAN0);
	candrv_init_ctrl(CAN1);
}

/**
 * @}
 */
