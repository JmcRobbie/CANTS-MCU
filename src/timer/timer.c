/**
 * @file timer.c
 */

/** 
 * @defgroup Timer Timer
 * @brief Timer driver module
 * @{
 */

#include "timer.h"
#include "soc.h"
#include "bitops.h"

void timer_set_frequency(struct timer *base, uint32_t frequency)
{
	uint32_t period = SOC_CLOCK / frequency;

	base->tmsr = 0;
	base->tmpr = 0;
	base->tmtr = 0;

	/* find best prescaler & period for given frequency */
	if (period > 0xFFFF) {
		uint16_t div = (period >> 16) + 1;
		uint16_t remainder = period / div;
		uint16_t best = div;

		for (; (remainder != 0) && (div < 0xffff); div++) {
			uint16_t tmp = period % div;
			if (tmp < remainder) {
				remainder = tmp;
				best = div;
			}
		}

		base->tmsr = best;
		base->tmpr = (period / best) - 1;
	} else {
		base->tmsr = 0;
		base->tmpr = period - 1;
	}
}

void timer_set_interrupt(struct timer *base, bool enabled)
{
	WriteBitsTyped(base->tmcr, TIMER_TMCR_IE, enabled, uint8_t);
}

bool timer_is_overflow(struct timer *base)
{
	return IsBitSet(base->tmcr, TIMER_TMCR_OVR);
}

void timer_enable(struct timer *base, bool enabled)
{
	WriteBitsTyped(base->tmcr, TIMER_TMCR_TE, enabled, uint8_t);
}

void timer_set_pwm_output(struct timer *base, uint8_t flags)
{
	WriteBitsTyped(base->pwmcr, 0x3F, flags, uint8_t);
}

void timer_set_pwm_duty_cycle(struct timer *base, uint8_t channel, uint16_t duty)
{
	uint32_t val = ((uint32_t)base->tmpr * (uint32_t)duty) / 0xFFFFul;
	*(&base->occ0 + channel) = val;
}

/**
 * @}
 */
