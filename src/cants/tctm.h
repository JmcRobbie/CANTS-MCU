/**
 * @file tctm.h
 *
 */

/**
 * @addtogroup CAN-TS
 * @{
 */

#ifndef TCTM_H_
#define TCTM_H_

#include "cants.h"

/**
 * @name TC/TM Request/Acknowledge definitions
 *@{
 */
#define TCTM_RA_MASK    0x300U /**< Mask for RA field */
#define TCTM_RA_REQUEST 0x000U /**< RA request */
#define TCTM_RA_ACK     0x100U /**< RA ack */
#define TCTM_RA_NACK    0x200U /**< RA nack */
/**
 *@}
 */

/**
 * @brief Initialize TC/TM stack
 * @param [in] cfg Keep-alive configuration
 * @retval None
 */
void tctm_init(const struct cants_keepalive_cfg *cfg);

/**
 * @brief Process TC/TM message
 * @param [in] msg CAN-TS message
 * @retval Non-zero if message has beed accepted for processing, 0 otherwise
 */
uint8_t tctm_process(struct cants_msg *msg);

/**
 * @brief Modify message into TC/TM ack/nack
 * @param [in] msg TC/TM request message
 * @param [in] ack non-zero is ack, 0 is nack
 * @retval None
 */
void tctm_send_ack(struct cants_msg *msg, uint8_t ack);

#endif

/**
 * @}
 */
