/**
 * @file telecommands.c
 *
 */

/**
 * @addtogroup cants_app CAN-TS application level
 * @{
 */

#include "cants.h"
#include "gpio.h"
#include "soc.h"
#include "telecommands.h"

uint8_t cants_telecommand_handler(uint8_t channel, uint8_t length, uint8_t *data)
{
	/* Set LEDs if correct command was received with correct length */
	if (channel == TC_SET_LED && length == 1) {
		gpio_set_port(GPIO5, data[0]);
		return 1;
	}

	return 0;
}

/**
 * @}
 */
