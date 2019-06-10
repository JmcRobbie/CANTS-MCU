/**
 * @file block.c
 *
 */

/**
 * @addtogroup CAN-TS
 * @{
 */

#include <string.h>

#include "block.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

/**
 * @enum sb_state
 * @brief Set Block states
 */
enum sb_state {
	sb_state_idle = 0, /**< Idle state, new request is possible */
	sb_state_receiving, /**< Data packets are being received */
	sb_state_writing, /**< Buffer is being write to destination */
	sb_state_done, /**< Set Block transfer is finished */
};
/**
 *@}
 */

/**
 * @enum gb_state
 * @brief Get Block states
 */
enum gb_state {
	gb_state_idle = 0, /**< Idle state, new request is possible */
	gb_state_wait_on_start, /**< Session open, waiting on start packet */
	gb_state_transmitting, /**< Transmitting state */
};
/**
 *@}
 */

/**
 * @struct sb_session
 * @brief Holds state of one Set Block session
 */
struct sb_session {
	uint8_t buffer[512]; /**< intermediate buffer to hold data during trasmission */
	uint8_t mask[8]; /**< bitmap, represents which packet has been received */
	ADDRESS_TYPE address; /**< destination address */
	TimeOut_t timeout_state; /**< time of last valid received packet */
	TickType_t timeout; /**< remaining time before session expires */
	uint8_t source; /**< node ID of initiator of this Set Block session */
	uint8_t state; /**< current state of this session, one of ::sb_state */
	uint8_t max_seq; /**< Maximum sequence number in this session */
	uint8_t last_blk_size; /**< Size of last block */
	uint8_t done; /**< Marks if data has been written */
};
/**
 *@}
 */

/**
 * @struct gb_session
 * @brief Holds state of one Get Block session
 */
struct gb_session {
	uint8_t buffer[512]; /**< intermediate buffer to hold data during trasmission */
	uint8_t mask[8]; /**<  bitmap, represents which packet to send */
	TimeOut_t timeout_state; /**< time of last valid received packet */
	TickType_t timeout; /**< remaining time before session expires or when to send new data burst */
	uint8_t source; /**< node ID of initiator of this get Block session */
	uint8_t state; /**< current state of this session, one of ::gb_state */
	uint8_t max_seq; /**< Maximum sequence number in this session */
	uint8_t cur_seq; /**< Sequence number of next block sent */
};
/**
 *@}
 */

/* Set & Get block queue related buffers */
static QueueHandle_t setblock_queue = NULL;
static StaticQueue_t setblock_queue_struct;
static uint8_t setblock_queue_buffer[SETBLOCK_QUEUE_LEN * sizeof(struct cants_msg)];

static QueueHandle_t getblock_queue = NULL;
static StaticQueue_t getblock_queue_struct;
static uint8_t getblock_queue_buffer[GETBLOCK_QUEUE_LEN * sizeof(struct cants_msg)];

/* Set & Get block task related buffers */
static StaticTask_t getblock_task_buffer;
static StackType_t getblock_task_stack[GETBLOCK_STACK_SIZE];

static StaticTask_t setblock_task_buffer;
static StackType_t setblock_task_stack[SETBLOCK_STACK_SIZE];

/* globals which hold session states */
static struct sb_session sb_sessions[MAX_SB_SESSIONS];
static struct gb_session gb_sessions[MAX_GB_SESSIONS];

/**
 * @brief Validates received address and copy it into session state
 * @param [in] msg Received CAN-TS frame
 * @param [out] address destination variable, to which address will be copied into
 * @retval 0 on failure, 1 on success
 */
static uint8_t block_copy_address(struct cants_msg *msg, ADDRESS_TYPE *address)
{
	if (msg->length == 0 || msg->length > sizeof(*address))
		return 0;

	*address = 0;
	memcpy(address, msg->data, msg->length);

	return 1;
}

/**
 * @brief Sets bit in a bitmap
 * @param [in] mask Bitmap array
 * @param [in] position Bit position to set to 1
 * @retval None
 */
static void block_update_mask(uint8_t *mask, uint8_t position)
{
	mask[position / 8] |= 1U << (position % 8);
}

/**
 * @brief Checks if bit is set in a bitmap
 * @param [in] mask Bitmap array
 * @param [in] sequence Number of the bit to check
 * @retval 0 if bit is not set, any other value means bit is set
 */
static uint8_t block_check_seq_in_mask(uint8_t *mask, uint8_t sequence)
{
	return mask[sequence / 8] & (1U << (sequence % 8));
}

/**
 * @brief Checks if all bits from 0 to blk_count-1 are set in a mask
 * @param [in] mask Bitmap array
 * @param [in] blk_count Number of blocks to check
 * @retval 1 if all blocks are received, 0 otherwise
 */
static uint8_t block_check_fully_received(uint8_t *mask, uint8_t blk_count)
{
	uint8_t tmp;

	tmp = blk_count / 8;
	while (tmp--)
		if (*mask++ != 0xff)
			return 0;

	tmp = blk_count % 8;
	if (tmp && *mask != ((1U << tmp) - 1))
		return 0;

	return 1;
}

/**
 * @brief Validate received mask
 * @param [in] data Buffer with mask data
 * @param [in] length Length of received mask array in bytes
 * @param [in] blk_count Block count in this session
 * @retval 1 if mask is valid, 0 otherwise
 */
static uint8_t block_validate_mask(uint8_t *data, uint8_t length, uint8_t blk_count)
{
	uint8_t tmp, i;

	/* check if mask is too short */
	if (length < (blk_count + 7) / 8)
		return 0;

	tmp = blk_count % 8;
	i = blk_count / 8;

	/* check if bits higher than max. block number are set */
	if (tmp) {
		if (data[i] & ~((1U << tmp) - 1))
			return 0;
		++i;
	}

	/* If mask is longer than it needs to be, higher bytes have to be 0. */
	while (i++ < length)
		if (data[i])
			return 0;

	return 1;
}

/**
 * @brief Find Set Block session state based on source ID
 * @param [in] msg CAN-TS message
 * @retval pointer to a session state if existing session is found, NULL otherwise
 */
static struct sb_session *block_get_sb_session(struct cants_msg *msg)
{
	uint8_t i;

	for (i = 0; i < MAX_SB_SESSIONS; ++i) {
		struct sb_session *session = &sb_sessions[i];

		/* match session based on source ID */
		if (session->source == msg->source &&
			session->state != sb_state_idle)
			return session;
	}

	return NULL;
}

/**
 * @brief Find free slot for new Set Block session
 * @retval pointer to a new session state if available, NULL otherwise
 */
static struct sb_session *block_new_sb_session(void)
{
	uint8_t i;

	for (i = 0; i < MAX_SB_SESSIONS; ++i) {
		struct sb_session *session = &sb_sessions[i];

		if (session->state == sb_state_idle)
			return session;
	}

	return NULL;
}

/**
 * @brief Sets new timeout for Set Block session, based on state
 * @param [in] session session state
 * @retval None
 */
static void block_sb_reset_timeout(struct sb_session *session)
{
	vTaskSetTimeOutState(&session->timeout_state);
	/* when writing data, value of "done" field has to be checked more often */
	session->timeout = (session->state == sb_state_writing) ?
		pdMS_TO_TICKS(SB_WRITING_INTERVAL) : pdMS_TO_TICKS(SB_TIMEOUT);
}

/**
 * @brief Handle Set Block timeouts
 * @param [in] session session state
 * @retval None
 */
static void block_sb_timeout(struct sb_session *session)
{
	if (session->state == sb_state_writing) {
		if (session->done)
			session->state = sb_state_done;
		block_sb_reset_timeout(session);
	} else {
		session->state = sb_state_idle;
	}
}

/**
 * @brief Set Block handling task
 * @param [in] arg ignored
 * @retval None
 */
static void cants_setblock_task(void *arg)
{
	TickType_t timeout = portMAX_DELAY;
	struct sb_session *session;
	uint8_t seq, i, nack, ra;
	struct cants_msg msg;
	int8_t tindex = -1;

	(void)arg;

	while (1) {
		if (xQueueReceive(setblock_queue, &msg, timeout)) {
			ra = msg.command >> BLOCK_RA_SHIFT;

			/* find session state coresponding to message source id */
			session = block_get_sb_session(&msg);

			/*
			 *  Session must always be found, unless it's a request frame,
			 *  in which case it must not be found.
			 */
			if ((session && ra == BLOCK_RA_REQUEST) ||
				(!session && ra != BLOCK_RA_REQUEST)) {
				block_send_ack(&msg, 0, 1);
				continue;
			}

			nack = 1;

			switch (msg.command >> BLOCK_RA_SHIFT) {
			/* process request message */
			case BLOCK_RA_REQUEST:
				seq = msg.command & 0x3f;
				session = block_new_sb_session();
				/* validate session request */
				nack = !session ||
					   !block_copy_address(&msg, &session->address) ||
					   !cants_validate_write_address(session->address,
							   (uint16_t)(seq + 1) * 8);
				if (!nack) {
					/* initialize session state */
					session->source = msg.source;
					session->max_seq = seq;
					memset(session->mask, 0, sizeof(session->mask));
					session->state = sb_state_receiving;
					block_send_ack(&msg, 1, 1);
				}
				break;
			/* process abort message */
			case BLOCK_RA_ABORT:
				if (msg.length == 0) {
					nack = 0;
					session->state = sb_state_idle;
					block_send_ack(&msg, 1, 1);
				}
				break;
			/* process status message */
			case BLOCK_RA_SB_STATUS:
				if (msg.length == 0) {
					nack = 0;
					msg.destination = msg.source;
					msg.source = CANTS_NODE_ID;
					msg.command = BLOCK_RA_SB_REPORT << BLOCK_RA_SHIFT;
					if ((session->state == sb_state_writing && session->done) ||
						(session->state == sb_state_done))
						msg.command |= 1U << 6;
					msg.length = (session->max_seq + 1 + 7) / 8;
					memcpy(msg.data, session->mask, msg.length);
					cants_send_msg(&msg, 1);
				}
				break;
			/* process transfer message */
			case BLOCK_RA_SB_TRANSFER:
				if (session->state == sb_state_receiving) {
					seq = msg.command & 0x3f;

					/* validate sequence number and message length */
					if ((seq < session->max_seq && msg.length == 8) ||
						(seq == session->max_seq && msg.length > 0)) {
						nack = 0;

						/* last block may be of different length */
						if (seq == session->max_seq)
							session->last_blk_size = msg.length;

						/* mark block as received and copy it's data */
						block_update_mask(session->mask, seq);
						memcpy(&session->buffer[(uint16_t)seq * 8], msg.data, msg.length);

						/* if all data transfer has been received, start processing the data */
						if (block_check_fully_received(session->mask, session->max_seq + 1)) {
							uint16_t size = session->max_seq * 8 + session->last_blk_size;
							session->done = 0;
							session->state = sb_state_writing;
							cants_write_block_handler(session->address, session->buffer, size, &session->done);
						}
					}
				}
				break;
			default:
				/* nothing to do */
				break;
			}

			if (nack)
				block_send_ack(&msg, 0, 1);
			else
				/* all valid packets reset timeout */
				block_sb_reset_timeout(session);
		} else if (tindex > -1) {
			/* process timeout */
			cants_assert(tindex < MAX_SB_SESSIONS);
			block_sb_timeout(&sb_sessions[tindex]);
		}

		/* search for the next timeout */
		timeout = portMAX_DELAY;
		tindex = -1;
		for (i = 0; i < MAX_SB_SESSIONS; i++) {
			session = &sb_sessions[i];

			/* skip inactive sessions */
			if (session->state == sb_state_idle)
				continue;

			/* immediately handle expired timeouts */
			if (xTaskCheckForTimeOut(&session->timeout_state, &session->timeout) != pdFALSE)
				block_sb_timeout(session);

			/* above "if" changed either state or timeout, so state have to be checked again */
			if (session->state != sb_state_idle && session->timeout < timeout) {
				/* store info about next timeout */
				timeout = session->timeout;
				tindex = i;
			}
		}
	}
}

/**
 * @brief Find Get Block session state based on source ID
 * @param [in] msg CAN-TS message
 * @retval pointer to a session state if existing session is found, NULL otherwise
 */
static struct gb_session *block_get_gb_session(struct cants_msg *msg)
{
	uint8_t i;

	for (i = 0; i < MAX_GB_SESSIONS; ++i) {
		struct gb_session *session = &gb_sessions[i];

		/* match session based on source ID */
		if (session->source == msg->source &&
			session->state != gb_state_idle)
			return session;
	}

	return NULL;
}

/**
 * @brief Find free slot for new Get Block session
 * @retval pointer to a new session state if available, NULL otherwise
 */
static struct gb_session *block_new_gb_session(void)
{
	uint8_t i;

	for (i = 0; i < MAX_GB_SESSIONS; ++i) {
		struct gb_session *session = &gb_sessions[i];

		if (session->state == gb_state_idle)
			return session;
	}

	return NULL;
}

/**
 * @brief Sets new timeout for Get Block session, based on state
 * @param [in] session session state
 * @retval None
 */
static void block_gb_reset_timeout(struct gb_session *session)
{
	vTaskSetTimeOutState(&session->timeout_state);
	session->timeout = (session->state == gb_state_transmitting) ?
		pdMS_TO_TICKS(GB_BURST_INTERVAL) : pdMS_TO_TICKS(GB_TIMEOUT);
}

/**
 * @brief Sends burst of data frames
 * @param [in] session session state
 * @retval None
 */
static void block_gb_burst(struct gb_session *session)
{
	struct cants_msg msg;
	uint8_t cnt = 0;

	/* prepare static fields for messages */
	msg.destination = session->source;
	msg.source = CANTS_NODE_ID;
	msg.length = 8;
	msg.type = cants_type_get_block;

	/* Send burst of data frames. It will send GB_BURST_SIZE packets at most. */
	while (session->cur_seq <= session->max_seq && cnt < GB_BURST_SIZE) {
		if (block_check_seq_in_mask(session->mask, session->cur_seq)) {
			/* set sequence number and copy data */
			msg.command = (BLOCK_RA_GB_TRANSFER << BLOCK_RA_SHIFT) | session->cur_seq;
			memcpy(msg.data, &session->buffer[(uint16_t)(session->cur_seq) * 8], 8);
			if (!cants_send_msg(&msg, 0))
				return;
			cnt++;
		}
		session->cur_seq++;
	}

	if (session->cur_seq > session->max_seq)
		session->state = gb_state_wait_on_start;
}

/**
 * @brief Handle Get Block timeouts
 * @param [in] session session state
 * @retval None
 */
static void block_gb_timeout(struct gb_session *session)
{
	if (session->state == gb_state_transmitting) {
		block_gb_burst(session);
		block_gb_reset_timeout(session);
	} else {
		session->state = gb_state_idle;
	}
}

/**
 * @brief Get Block handling task
 * @param [in] arg ignored
 * @retval None
 */
static void cants_getblock(void *arg)
{
	TickType_t timeout = portMAX_DELAY;
	struct gb_session *session;
	uint8_t seq, nack, i, ra;
	ADDRESS_TYPE address;
	struct cants_msg msg;
	int8_t tindex = -1;

	(void)arg;

	while (1) {
		if (xQueueReceive(getblock_queue, &msg, timeout)) {
			ra = msg.command >> BLOCK_RA_SHIFT;

			/* find session state coresponding to message source id */
			session = block_get_gb_session(&msg);

			/*
			 *  Session must always be found, unless it's a request frame,
			 *  in which case it must not be found.
			 */
			if ((session && ra == BLOCK_RA_REQUEST) ||
				(!session && ra != BLOCK_RA_REQUEST)) {
				block_send_ack(&msg, 0, 1);
				continue;
			}

			nack = 1;

			switch (ra) {
			/* process request */
			case BLOCK_RA_REQUEST:
				seq = msg.command & 0x3f;
				session = block_new_gb_session();
				/* validate session request */
				nack = !session ||
					   !block_copy_address(&msg, &address) ||
					   /* TODO: make async */
					   !cants_read_block_handler(address, session->buffer,
							   (uint16_t)(seq + 1) * 8);
				if (!nack) {
					/* initialize session state */
					session->source = msg.source;
					session->max_seq = seq;
					session->state = gb_state_wait_on_start;
					block_send_ack(&msg, 1, 1);
				}
				break;
			/* process abort message */
			case BLOCK_RA_ABORT:
				if (msg.length == 0) {
					nack = 0;
					session->state = gb_state_idle;
					block_send_ack(&msg, 1, 1);
				}
				break;
			/* process start message */
			case BLOCK_RA_GB_START:
				if (block_validate_mask(msg.data, msg.length, session->max_seq + 1)) {
					nack = 0;
					memcpy(&session->mask, msg.data, msg.length);
					session->cur_seq = 0;
					session->state = gb_state_transmitting;
					block_gb_burst(session);
				}
				break;
			default:
				/* nothing to do */
				break;
			}

			if (nack)
				block_send_ack(&msg, 0, 1);
			else
				/* all valid packets reset timeout */
				block_gb_reset_timeout(session);
		} else if (tindex > -1) {
			/* process timeout */
			cants_assert(tindex < MAX_GB_SESSIONS);
			block_gb_timeout(&gb_sessions[tindex]);
			tindex = -1;
		}

		/* search for the next timeout */
		timeout = portMAX_DELAY;
		tindex = -1;
		for (i = 0; i < MAX_GB_SESSIONS; i++) {
			session = &gb_sessions[i];

			/* skip inactive sessions */
			if (session->state == gb_state_idle)
				continue;

			/* immediately handle expired timeouts */
			if (xTaskCheckForTimeOut(&session->timeout_state, &session->timeout) != pdFALSE)
				block_gb_timeout(session);

			/* above "if" changed either state or timeout, so state have to be checked again */
			if (session->state != gb_state_idle && session->timeout < timeout) {
				/* store info about next timeout */
				timeout = session->timeout;
				tindex = i;
			}
		}
	}
}

void block_init(void)
{
	/* initialize Set and Get Block queues */
	setblock_queue = xQueueCreateStatic(SETBLOCK_QUEUE_LEN, sizeof(struct cants_msg),
			setblock_queue_buffer, &setblock_queue_struct);
	getblock_queue = xQueueCreateStatic(GETBLOCK_QUEUE_LEN, sizeof(struct cants_msg),
			getblock_queue_buffer, &getblock_queue_struct);

	/* initialize Set and Get Block tasks */
	xTaskCreateStatic(cants_getblock, "CANTSGB", ARRAY_SIZE(getblock_task_stack),
			0, GETBLOCK_PRIORITY, getblock_task_stack, &getblock_task_buffer);
	xTaskCreateStatic(cants_setblock_task, "CANTSSB", ARRAY_SIZE(setblock_task_stack),
			0, SETBLOCK_PRIORITY, setblock_task_stack, &setblock_task_buffer);
}

uint8_t block_process(struct cants_msg *msg)
{
	/* dispatch message to appropriate task */
	if (msg->type == cants_type_set_block)
		return xQueueSendToBack(setblock_queue, msg, pdMS_TO_TICKS(10)) != errQUEUE_FULL;

	if (msg->type == cants_type_get_block)
		return xQueueSendToBack(getblock_queue, msg, pdMS_TO_TICKS(10)) != errQUEUE_FULL;

	return 0;
}

void block_send_ack(struct cants_msg *msg, uint8_t ack, uint8_t wait_allowed)
{
	/* set source and destination address */
	msg->destination = msg->source;
	msg->source = CANTS_NODE_ID;

	/* set ack or nack flag */
	if (ack) {
		msg->command &= ~(7 << BLOCK_RA_SHIFT);
		msg->command |= BLOCK_RA_ACK << BLOCK_RA_SHIFT;
	} else {
		msg->command = BLOCK_RA_NACK << BLOCK_RA_SHIFT;
		/* nack frame hasn't any data */
		msg->length = 0;
	}

	cants_send_msg(msg, wait_allowed);
}

/**
 * @}
 */
