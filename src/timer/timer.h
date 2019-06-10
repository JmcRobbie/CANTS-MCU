/**
 * @file timer.h
 * 
 */

/** 
 * @addtogroup Timer
 * @{
 */

#ifndef TIMER_H_
#define TIMER_H_

#ifndef __ASSEMBLER__
#include <stdint.h>
#include <stdbool.h>
#include "bitops.h"
#endif

/**
 * @name Timer control flags
 *@{
 */
#define TIMER_TMCR_OVR     Bit(0)    /**< Overflow flag. */
#define TIMER_TMCR_IE      Bit(1)    /**< Timer interrupt enable flag. */
#define TIMER_TMCR_TE      Bit(2)    /**< Timer enable flag. */
/**
 *@}
 */

/**
 * @name PWM control flags
 */
#define TIMER_PWM_OC0_EN   Bit(0)	 /**< Output compare channel 0 enable. */
#define TIMER_PWM_OC1_EN   Bit(1)	 /**< Output compare channel 1 enable. */
#define TIMER_PWM_OC2_EN   Bit(2)	 /**< Output compare channel 2 enable. */
#define TIMER_PWM_OC0_POL  Bit(3)	 /**< Output compare channel 0 polarity. */
#define TIMER_PWM_OC1_POL  Bit(4)	 /**< Output compare channel 1 polarity. */
#define TIMER_PWM_OC2_POL  Bit(5)	 /**< Output compare channel 2 polarity. */
/**
 *@}
 */

/**
 * @name PWM channels
 */
#define TIMER_PWM_CH0      0x00		 /**< PWM channel 0. */
#define TIMER_PWM_CH1      0x01		 /**< PWM channel 1. */
#define TIMER_PWM_CH2      0x02		 /**< PWM channel 2. */
/**
 *@}
 */

#ifdef __ASSEMBLER__

#define TIMER_TMCR(base)  (base + 0)
#define TIMER_TMSR(base)  (base + 1)
#define TIMER_TMSRL(base) (base + 1)
#define TIMER_TMSRH(base) (base + 2)
#define TIMER_TMPR(base)  (base + 3)
#define TIMER_TMPRL(base) (base + 3)
#define TIMER_TMPRH(base) (base + 4)
#define TIMER_TMTR(base)  (base + 5)
#define TIMER_TMTRL(base) (base + 5)
#define TIMER_TMTRH(base) (base + 6)
#define TIMER_OCC0L(base) (base + 7)
#define TIMER_OCC0H(base) (base + 8)
#define TIMER_OCC1L(base) (base + 9)
#define TIMER_OCC1H(base) (base + 10)
#define TIMER_OCC2L(base) (base + 11)
#define TIMER_OCC2H(base) (base + 12)
#define TIMER_PWMCR(base) (base + 13)

#else

/**
 * @brief Timer registers.
 */
struct timer {
	volatile uint8_t tmcr;     /**< Control register. */
	volatile uint16_t tmsr;    /**< Scale register. */
	volatile uint16_t tmpr;    /**< Period register. */
	volatile uint16_t tmtr;    /**< Time register. */
	volatile uint16_t occ0;    /**< Output compare 0 register. */
	volatile uint16_t occ1;    /**< Output compare 1 register. */
	volatile uint16_t occ2;    /**< Output compare 2 register. */
	volatile uint8_t pwmcr;    /**< PWM control register. */
};

/**
 * @brief Set overflow frequency of a timer.
 * @param [in] base Timer base address.
 * @param [in] frequency Frequency of a timer in Hz.
 * @retval None
 */
void timer_set_frequency(struct timer *base, uint32_t frequency);

/**
 * @brief Set timer interrupts.
 * @param [in] base Timer base address.
 * @param [in] enabled If true, interrupts are enabled, else disabled.
 * @retval None
 */
void timer_set_interrupt(struct timer *base, bool enabled);

/**
 * @brief Checks if timer overflows.
 * @param [in] base Timer base address.
 * @retval true Timer did overflow.
 * @retval false Timer didn't overflow.
 */
bool timer_is_overflow(struct timer *base);

/**
 * @brief Enable or disable timer.
 * @param [in] base Timer base address.
 * @param [in] enabled Enable or disable timer.
 * @retval None
 */
void timer_enable(struct timer *base, bool enabled);

/**
 * @brief Set PWM output flags.
 * @param [in] base Timer base address.
 * @param [in] flags PWM settings. Valid flags are
 * ::TIMER_PWM_OC0_EN, ::TIMER_PWM_OC1_EN, ::TIMER_PWM_OC2_EN,
 * ::TIMER_PWM_OC0_POL, ::TIMER_PWM_OC1_POL, ::TIMER_PWM_OC2_POL
 * @retval None
 */
void timer_set_pwm_output(struct timer *base, uint8_t flags);

/**
 * @brief Set PWM duty cycle on selected channel.
 * @param [in] base Timer base address.
 * @param [in] channel PWM channel. Valid values are ::TIMER_PWM_CH0, ::TIMER_PWM_CH1, ::TIMER_PWM_CH2.
 * @param [in] duty PWM duty cycle. 0xFFFF is 100% duty cycle, 0x0000 is 0% duty cycle.
 * @retval None
 */
void timer_set_pwm_duty_cycle(struct timer *base, uint8_t channel, uint16_t duty);

#endif
#endif /* TIMER_H_ */

/**
 * @}
 */
