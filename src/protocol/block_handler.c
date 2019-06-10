/**
 * @file block_handler.c
 *
 */

#include <string.h>
#include "cants.h"

static uint8_t data[0x100]; /**< buffer for holding data written by Set Block transfer */

uint8_t cants_read_block_handler(ADDRESS_TYPE address, uint8_t *buffer, uint16_t size)
{
	/* validate address */
	if (address + size <= 0x100) {
		/* copy requested data */
		memcpy(buffer, &data[address], size);

		return 1;
	}

	return 0;
}

uint8_t cants_validate_write_address(ADDRESS_TYPE address, uint16_t size)
{
	/* validate write address */
	return address + size <= 0x100;
}

uint8_t cants_write_block_handler(ADDRESS_TYPE address, uint8_t *buffer, uint16_t size, uint8_t *done)
{
	/* address was already validated */

	/*
	 * Copy data to our storage buffer. In reality, we can do anything we
	 * want with them.
	 */
	memcpy(&data[address], buffer, size);

	/*
	 * In this example, we set done flag immediatelly. However, we could
	 * delegate data processing to another task, in which case it would
	 * be responsible to set this flag when it is finished processing the
	 * data.
	 */
	*done = 1;

	return 1;
}

/**
 * @}
 */
