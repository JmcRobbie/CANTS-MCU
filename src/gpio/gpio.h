/**
 * @file gpio.h
 * 
 */

/** 
 * @addtogroup GPIO
 * @{
 */

#ifndef GPIO_H_
#define GPIO_H_

#ifndef __ASSEMBLER__
#include <stdint.h>
#include <stdbool.h>
#endif

/**
 * @name GPIO pins
 *@{
 */
#define GPIO_PIN0    0x01    /**< GPIO pin 0 */
#define GPIO_PIN1    0x02    /**< GPIO pin 1 */
#define GPIO_PIN2    0x04    /**< GPIO pin 2 */
#define GPIO_PIN3    0x08    /**< GPIO pin 3 */
#define GPIO_PIN4    0x10    /**< GPIO pin 4 */
#define GPIO_PIN5    0x20    /**< GPIO pin 5 */
#define GPIO_PIN6    0x40    /**< GPIO pin 6 */
#define GPIO_PIN7    0x80    /**< GPIO pin 7 */
#define GPIO_PIN_ALL 0xff    /**< All GPIO pins */
/**
 *@}
 */

/**
 * @name GPIO pin direction
 *@{
 */
#define	GPIO_DIR_IN   0     /**< GPIO pin input direction flag */
#define	GPIO_DIR_OUT  1     /**< GPIO pin output direction flag */
/**
 *@}
 */

#ifdef __ASSEMBLER__

#define GPIO_IN(base)  (base + 0)
#define GPIO_DDR(base) (base + 1)
#define GPIO_OUT(base) (base + 2)
#define GPIO_POSEINT(base) (base + 3)
#define GPIO_NEGEINT(base) (base + 4)
#define GPIO_SRCINT(base)  (base + 5)

#else

/**
 * @enum edge
 * @brief  Possible interrupt sources
 */
enum edge {
	edge_none = 0, /**< Disable interrupt */
	edge_rising,   /**< Trigger interupt on rising edge */
	edge_falling,  /**< Trigger interupt on falling edge */
	edge_both,     /**< Trigger interupt on both edges */
};

/**
 * @struct gpio 
 * @brief  GPIO registers structure 
 */
struct gpio {
	volatile uint8_t in;  /**< Input signal register */
	volatile uint8_t ddr; /**< Direction control register */
	volatile uint8_t out; /**< Output signal register */
	volatile uint8_t poseint; /**< Positive edge interrupt register */
	volatile uint8_t negeint; /**< Negative edge interrupt register */
	volatile uint8_t srcint;  /**< Interrupt pending register */
	volatile uint8_t afsr0; /**< Alternative functions 0 register */
	volatile uint8_t afsr1; /**< Alternative functions 1 register */
	volatile uint8_t afse; /**< Alternative functions enable register */
};

/**
 * @brief Set I/O direction of digital pins.
 * @param [in] base GPIO port base address.
 * @param [in] pins Selected pins on port.
 * @param [in] direction I/O direction. Valid values: 
 * ::GPIO_DIR_IN, ::GPIO_DIR_OUT
 * @retval None
 */
void gpio_set_direction(struct gpio *base, uint8_t pins, uint8_t direction);

/**
 * @brief Set GPIO port with value.
 * @param [in] base GPIO port base address.
 * @param [in] state Value which will be set on port.
 * @retval None
 */
void gpio_set_port(struct gpio *base, uint8_t state);

/**
 * @brief Set selected pins on port.
 * @param [in] base GPIO port base address.
 * @param [in] pins Pins which will be set. Valid values are: 
 * ::GPIO_PIN0, ::GPIO_PIN1, ::GPIO_PIN2, 
 * ::GPIO_PIN3, ::GPIO_PIN4, ::GPIO_PIN5,
 * ::GPIO_PIN6, ::GPIO_PIN7, ::GPIO_PIN_ALL.
 * @retval None
 */
void gpio_set_pins(struct gpio *base, uint8_t pins);

/**
 * @brief Clear selected pins on port.
 * @param [in] base GPIO port base address.
 * @param [in] pins Pins which will be cleared. Valid values are: 
 * ::GPIO_PIN0, ::GPIO_PIN1, ::GPIO_PIN2, 
 * ::GPIO_PIN3, ::GPIO_PIN4, ::GPIO_PIN5,
 * ::GPIO_PIN6, ::GPIO_PIN7, ::GPIO_PIN_ALL.
 * @retval None
 */
void gpio_clr_pins(struct gpio *base, uint8_t pins);

/**
 * @brief Get value of GPIO port.
 * @param [in] base GPIO port base address.
 * @retval Value currently set on port.
 */
uint8_t gpio_get_port(struct gpio *base);

/**
 * @brief Get value of specific pin.
 * @param [in] base GPIO port base address.
 * @param [in] pin Pins which will be read. Valid values are: 
 * ::GPIO_PIN0, ::GPIO_PIN1, ::GPIO_PIN2, 
 * ::GPIO_PIN3, ::GPIO_PIN4, ::GPIO_PIN5,
 * ::GPIO_PIN6, ::GPIO_PIN7, ::GPIO_PIN_ALL.
 * @retval true
 */
bool gpio_get_pin(struct gpio *base, uint8_t pin);

/**
 * @brief Set external interrupt edge
 * @param [in] base GPIO port base address.
 * @param [in] pin Pin for which edge detection will be set. Valid values are:
 * ::GPIO_PIN0, ::GPIO_PIN1, ::GPIO_PIN2,
 * ::GPIO_PIN3, ::GPIO_PIN4, ::GPIO_PIN5,
 * ::GPIO_PIN6, ::GPIO_PIN7.
 * @param [in] edge On which edge interrupt will be triggered. Valid valus are:
 * ::edge_none::, ::edge_rising::,
 * ::edge_falling::, ::edge_both::.
 * @retval None
 */
void gpio_set_int(struct gpio *base, uint8_t pin, uint8_t edge);

/**
 * @brief Read interrupt status register. \warning Reading it clears register.
 * @param [in] base GPIO port base address.
 * @retval Bitmask
 */
uint8_t gpio_get_int_status(struct gpio *base);

/**
 * @brief Set pin functionality
 * @param [in] base GPIO port base address.
 * @param [in] pin Pin for which edge detection will be set. Valid values are:
 * ::GPIO_PIN0, ::GPIO_PIN1, ::GPIO_PIN2,
 * ::GPIO_PIN3, ::GPIO_PIN4, ::GPIO_PIN5,
 * ::GPIO_PIN6, ::GPIO_PIN7.
 * @param [in] function functionality. See soc.h for possible values
 * @retval None
 */
void gpio_set_function(struct gpio *base, uint8_t pin, uint8_t function);

#endif
#endif /* GPIO_H_ */

/**
 * @}
 */
