/**
 * @file gpio.c
 * 
 */

/**
 * @defgroup GPIO GPIO
 * @brief GPIO driver module
 * @{
 */

#include "gpio.h"
#include "bitops.h"

void gpio_set_direction(struct gpio *base, uint8_t pins, uint8_t direction)
{
	WriteBitsTyped(base->ddr, pins, direction == GPIO_DIR_OUT, uint8_t);
}

void gpio_set_port(struct gpio *base, uint8_t state)
{
	base->out = state;
}

void gpio_set_pins(struct gpio *base, uint8_t pins)
{
	SetBitsTyped(base->out, pins, uint8_t);
}

void gpio_clr_pins(struct gpio *base, uint8_t pins)
{
	ClrBitsTyped(base->out, pins, uint8_t);
}

uint8_t gpio_get_port(struct gpio *base)
{
	return base->in;
}

bool gpio_get_pin(struct gpio *base, uint8_t pin)
{
	return !!(base->in & pin);
}

void gpio_set_int(struct gpio *base, uint8_t pin, uint8_t edge)
{
	WriteBitsTyped(base->poseint, pin, edge & edge_rising, uint8_t);
	WriteBitsTyped(base->negeint, pin, edge & edge_falling, uint8_t);
}

uint8_t gpio_get_int_status(struct gpio *base)
{
	return base->srcint;
}

void gpio_set_function(struct gpio *base, uint8_t pin, uint8_t function)
{
	uint8_t pos = 0;

	while (pin >>= 1) ++pos;

	if (function > 0) {
		uint8_t shift;

		if (pos < 4) {
			shift = pos * 2;
			ClrSetBitsTyped(base->afsr0, 3 << shift,
					(function - 1) << shift, uint8_t);
		} else {
			shift = (pos - 4) * 2;
			ClrSetBitsTyped(base->afsr1, 3 << shift,
					(function - 1) << shift, uint8_t);
		}
	}

	WriteBitsTyped(base->afse, Bit(pos), function != 0, uint8_t);
}

/**
 * @}
 */
