/**
 * @file time.c
 *
 */

/**
 * @addtogroup cants_app CAN-TS application level
 * @{
 */

#include "cants.h"

void cants_time_sync_handler(uint8_t length, uint8_t *data)
{
	(void)length;
	(void)data;

	/* TODO: handle received time packet */
}

/**
 * @}
 */
