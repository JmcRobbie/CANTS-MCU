/**
 * @file candrv.h
 *
 */

#ifndef CANDRV_H_
#define CANDRV_H_

/**
 * @addtogroup CanIntf
 * @{
 */

#include <stdint.h>

/* length of CAN send queue */
#define CAN_SEND_QUEUE_LEN 16

/**
 * @brief Initialize CAN-TS stack and CAN controller
 * @retval None
 */
void candrv_init(void);

/**
 * @brief Switch currently active CAN bus
 * @param [in] bus 0 means primary bus, anything else means secondary bus
 * @retval None
 */
void candrv_set_bus(uint8_t bus);

#endif

/**
 * @}
 */
