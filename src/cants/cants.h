/**
 * @file cants.h
 *
 */

/**
 * @addtogroup CAN-TS CAN-TS stack
 * @{
 */

#ifndef CANTS_H_
#define CANTS_H_

#include <stdint.h>

#include "cants_config.h"

/**
 * @enum cants_type
 * @brief CAN-TS transfer types
 */
enum cants_type {
	cants_type_time_sync = 0, /**< Time Sync transfer type */
	cants_type_unsolicited_tm, /**< Unsolicited Telemetry transfer type */
	cants_type_telecommand, /**< Telecommand transfer type */
	cants_type_telemetry, /**< Telemetry transfer type */
	cants_type_set_block, /**< Set Block transfer type */
	cants_type_get_block, /**< Get Block transfer type */
};
/**
 *@}
 */

/**
 * @struct cants_msg
 * @brief CAN-TS message
 */
struct cants_msg {
	uint8_t destination; /**< CAN-TS destination ID */
	uint8_t type; /**< CAN-TS frame type */
	uint8_t source; /**< CAN-TS source ID */
	uint16_t command; /**< CAN-TS command */
	uint8_t length; /**< length of data array */
	uint8_t data[8]; /**< data in CAN-TS message */
};
/**
 *@}
 */

/**
 * @struct cants_keepalive_cfg
 * @brief CAN-TS keep-alive transmission configuration
 */
struct cants_keepalive_cfg {
	uint8_t utm_ch_min; /**< minimum telemetry channel */
	uint8_t utm_ch_max; /**< maximum telemetry channel */
	uint16_t period; /**< transmission period */
};
/**
 *@}
 */

/**
 * @brief Initialize CAN-TS stack
 * @param [in] cfg keek-alive transmission configuration
 * @retval None
 */
void cants_init(const struct cants_keepalive_cfg *cfg);

/**
 * @brief Put CAN-TS message in processing queue from ISR
 * @param [in] msg CAN-TS message to process
 * @retval 1 if message is queued for processing, 0 otherwise
 */
uint8_t cants_dispatch_isr(struct cants_msg *msg);

/* utility functions */

/**
 * @brief Parse CAN ID field to CAN-TS fields
 * @param [out] out CAN-TS structure which gets populated with parsed value
 * @param [in] id CAN ID to parse
 * @retval None
 */
void cants_parse_id(struct cants_msg *out, uint32_t id);

/**
 * @brief Converts CAN-TS message fileds to CAN ID
 * @param [in] in CAN-TS message
 * @retval constructed CAN ID
 */
uint32_t cants_construct_id(struct cants_msg *in);

/* these functions must be provided by FW */

/**
 * @brief Send CAN-TS message. End system specific implementation must be provided.
 * @param [in] msg CAN-TS message to send
 * @param [in] wait_allowed 0 if blocking is not allowed, any other value means allowed
 * @retval 0 if message was not send successfully, any other value means success
 */
uint8_t cants_send_msg(struct cants_msg *msg, uint8_t wait_allowed);

/**
 * @brief Time Sync handler. End system specific implementation must be provided.
 * @param [in] length length of data array
 * @param [in] data Time Sync data
 * @retval None
 */
void cants_time_sync_handler(uint8_t length, uint8_t *data);

/**
 * @brief Unsolicited Telemetry handler. End system specific implementation must be provided.
 * @param [in] source Source ID of Unsolicited Telemetry message
 * @param [in] channel Telemetry channel
 * @param [in] length Length of telemetry data
 * @param [in] data Telemetry data
 * @retval None
 */
void cants_unsolicited_handler(uint8_t source, uint8_t channel, uint8_t length, uint8_t *data);

/**
 * @brief Telecommand handler. End system specific implementation must be provided.
 * @param [in] channel Telecommand channel
 * @param [in] length Length of telecommand data
 * @param [in] data Telecommand data
 * @retval non-zero value if telecommand was processed successfully, 0 otherwise
 */
uint8_t cants_telecommand_handler(uint8_t channel, uint8_t length, uint8_t *data);

/**
 * @brief Telemetry handler. End system specific implementation must be provided.
 * @param [in] channel Telemetry channel
 * @param [in] length Length of telemetry data
 * @param [out] data Telemetry data
 * @retval non-zero value if telemetry was processed successfully, 0 otherwise
 */
uint8_t cants_telemetry_handler(uint8_t channel, uint8_t *length, uint8_t *data);

/**
 * @brief Set Block address validation function. End system specific implementation must be provided.
 * @param [in] address Set Block destination address
 * @param [in] size Amount of data to write at specified address
 * @retval non-zero value if address is valid, 0 otherwise
 */
uint8_t cants_validate_write_address(ADDRESS_TYPE address, uint16_t size);

/**
 * @brief Set Block handler. End system specific implementation must be provided.
 * @param [in] address Set Block destination address
 * @param [in] buffer Buffer, which holds data to write
 * @param [in] size Size of the buffer
 * @param [in] done Address of done flag, which must be set to non-zero value when data processing has been finished.
 * @retval non-zero value if data processing has started successfully, 0 otherwise
 */
uint8_t cants_write_block_handler(ADDRESS_TYPE address, uint8_t *buffer, uint16_t size, uint8_t *done);

/**
 * @brief Read Block handler. End system specific implementation must be provided.
 * @param [in] address Set Block source address
 * @param [out] buffer Buffer into which data is read
 * @param [out] size Size of the request data
 * @retval non-zero if data was read into buffer, 0 otherwise
 */
uint8_t cants_read_block_handler(ADDRESS_TYPE address, uint8_t *buffer, uint16_t size);

#endif

/**
 * @}
 */
