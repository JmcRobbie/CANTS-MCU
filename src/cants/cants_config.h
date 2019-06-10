/**
 * @file cants_config.h
 *
 */

/**
 * @addtogroup CAN-TS
 * @{
 */

#ifndef CANTS_CONFIG_H_
#define CANTS_CONFIG_H_

#include "Assert.h"

#define cants_assert Assert

#define CANTS_NODE_ID 0x80 /**< ID of this node */
#define CANTS_KEEPALIVE_ID 0x01 /**< broadcast ID on which keep-alive messages are sent */
#define CANTS_TIME_ID 0 /**< broadcast ID on which time sync messages are sent */
#define CAN_HW_FILTERING 1 /**< 1 if CAN controller will do filtering, 0 otherwise */
#define CANTS_SEND_KEEPALIVE 1 /**< 1 if keep-alive messages should be sent */

#define MAX_SB_SESSIONS 2 /**< number of maximum supported simultaneous SB sessions */
#define MAX_GB_SESSIONS 2 /**< number of maximum supported simultaneous GB sessions */
#define SB_TIMEOUT 5000 /**< SB session timeout from last valid packet received */
#define SB_WRITING_INTERVAL 100 /**< how often to check if data processing was completed */
#define GB_TIMEOUT 1000 /**< GB session timeout from last valid packet received */
#define GB_BURST_SIZE 8 /**< maximum number of data transfer frames sent in one burst */
#define GB_BURST_INTERVAL 100 /**< how often to send burst of GB data transfer frames */
#define ADDRESS_TYPE uint32_t /**< GB/SB address type */

/* task stack sizes and priorities */
#define DISPATCHER_STACK_SIZE configMINIMAL_STACK_SIZE
#define DISPATCHER_PRIORITY (tskIDLE_PRIORITY + 2)

#define TC_STACK_SIZE configMINIMAL_STACK_SIZE
#define TC_PRIORITY (tskIDLE_PRIORITY + 1)

#define TM_STACK_SIZE configMINIMAL_STACK_SIZE
#define TM_PRIORITY (tskIDLE_PRIORITY + 1)

#define SETBLOCK_STACK_SIZE configMINIMAL_STACK_SIZE
#define SETBLOCK_PRIORITY (tskIDLE_PRIORITY + 2)

#define GETBLOCK_STACK_SIZE configMINIMAL_STACK_SIZE
#define GETBLOCK_PRIORITY (tskIDLE_PRIORITY + 1)

/* queue lengths */
#define DISPATCHER_QUEUE_LEN 64
#define TC_QUEUE_LEN 5
#define TM_QUEUE_LEN 5
#define SETBLOCK_QUEUE_LEN 64
#define GETBLOCK_QUEUE_LEN 5

#endif

/**
 * @}
 */
