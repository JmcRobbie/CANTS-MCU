/**
 * @file block.h
 *
 */

/**
 * @addtogroup CAN-TS
 * @{
 */

#ifndef BLOCK_H_
#define BLOCK_H_

#include "cants.h"

/**
 * @name Block tranfer Request/Acknowledge definitions
 *@{
 */
#define BLOCK_RA_SHIFT    7U /**< position of first RA bit in command field */
#define BLOCK_RA_REQUEST  0U /**< RA request */
#define BLOCK_RA_ACK      2U /**< RA acknowledge */
#define BLOCK_RA_ABORT    3U /**< RA abort  */
#define BLOCK_RA_NACK     4U /**< RA nack */
/**
 *@}
 */

/**
 * @name Set Block tranfer specific Request/Acknowledge definitions
 *@{
 */
#define BLOCK_RA_SB_TRANSFER 1U /**< Set Block RA transfer  */
#define BLOCK_RA_SB_STATUS   6U /**< Set Block RA status */
#define BLOCK_RA_SB_REPORT   7U /**< Set Block RA report */
/**
 *@}
 */

/**
 * @name Get Block tranfer specific Request/Acknowledge definitions
 *@{
 */
#define BLOCK_RA_GB_START    6U /**< Get Block RA start  */
#define BLOCK_RA_GB_TRANSFER 7U /**< Get Block RA transfer */
/**
 *@}
 */

/**
 * @brief Initializes CAN-TS Block transfer related tasks
 * @retval None
 */
void block_init(void);

/**
 * @brief Process block transfer message
 * @param [in] msg Block transfer message to process
 * @retval Non-zero if message is accepted for processing, 0 otherwise
 */
uint8_t block_process(struct cants_msg *msg);

/**
 * @brief Modify message into block transfer ack/nack
 * @param [in] msg Block transfer message
 * @param [in] ack non-zero is ack, 0 is nack
 * @param [in] wait_allowed 0 if function is not allowed to block, anything else means allowed
 * @retval None
 */
void block_send_ack(struct cants_msg *msg, uint8_t ack, uint8_t wait_allowed);

#endif

/**
 * @}
 */
