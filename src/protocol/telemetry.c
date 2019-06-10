/**
 * @file telemetry.c
 *
 */

/**
 * @addtogroup cants_app CAN-TS application level
 * @{
 */

#include "cants.h"
#include "gpio.h"
#include "soc.h"
#include "telemetry.h"

uint8_t cants_telemetry_handler(uint8_t channel, uint8_t *length, uint8_t *data)
{
	/* report LED status */
	if (channel == TM_LED_STATUS) {
		data[0] = gpio_get_port(GPIO5);
		*length = 1;

		return 1;
	}

	return 0;
}

/**
 * @}
 */
