/**
 * @file soc.h
 *
 */

#ifndef _SOC_H
#define _SOC_H

/** SoC base clock frequency */
#define SOC_CLOCK 18000000UL

#ifdef __ASSEMBLER__

#define GPIO0 0x60
#define GPIO1 0x69
#define GPIO2 0x72
#define GPIO3 0x7b
#define GPIO4 0x84
#define GPIO5 0x8d
#define TIM0  0x96
#define TIM1  0xa4
#define TIM2  0xb2
#define TIM3  0xc0
#define TIM4  0xce
#define TIM5  0xdc
#define UART0 0xea
#define UART1 0xef
#define UART2 0xf4
#define UART3 0xf9
#define CAN0  0xfe
#define CAN1  0x11e
#define SPI0  0x13e
#define SPI1  0x145
#define TWI0  0x14c
#define TWI1  0x151
#define EMMC  0x156
#define ADC   0x17f
#define DAC   0x199
#define WDT   0x1a2
#define RSTC  0x1a7
#define SID   0x1a8

#else

struct gpio;
struct timer;
struct uart;
struct can;
struct spi;
struct twi;
struct emmc;
struct adc;
struct dac;
struct wdt;
struct reset;
struct sid;

/* location of each peripheral */
#define GPIO0 ((struct gpio *)(0x60))
#define GPIO1 ((struct gpio *)(0x69))
#define GPIO2 ((struct gpio *)(0x72))
#define GPIO3 ((struct gpio *)(0x7b))
#define GPIO4 ((struct gpio *)(0x84))
#define GPIO5 ((struct gpio *)(0x8d))
#define TIM0  ((struct timer *)(0x96))
#define TIM1  ((struct timer *)(0xa4))
#define TIM2  ((struct timer *)(0xb2))
#define TIM3  ((struct timer *)(0xc0))
#define TIM4  ((struct timer *)(0xce))
#define TIM5  ((struct timer *)(0xdc))
#define UART0 ((struct uart *)(0xea))
#define UART1 ((struct uart *)(0xef))
#define UART2 ((struct uart *)(0xf4))
#define UART3 ((struct uart *)(0xf9))
#define CAN0  ((struct can *)(0xfe))
#define CAN1  ((struct can *)(0x11e))
#define SPI0  ((struct spi *)(0x13e))
#define SPI1  ((struct spi *)(0x145))
#define TWI0  ((struct twi *)(0x14c))
#define TWI1  ((struct twi *)(0x151))
#define EMMC  ((struct emmc *)(0x156))
#define ADC   ((struct adc *)(0x17f))
#define DAC   ((struct dac *)(0x199))
#define WDT   ((struct wdt *)(0x1a2))
#define RSTC  ((struct reset *)(0x1a7))
#define SID   ((struct sid *)(0x1a8))

#endif

/* all GPIO pin muxes */
#define GPIO0_PIN0_FUNC_GPIO      0
#define GPIO0_PIN0_FUNC_CAN0_N    1
#define GPIO0_PIN0_FUNC_SPI1_MISO 2
#define GPIO0_PIN0_FUNC_TWI1_SDA  3
#define GPIO0_PIN0_FUNC_UART2_RX  4

#define GPIO0_PIN1_FUNC_GPIO     0
#define GPIO0_PIN1_FUNC_CAN1_N   1
#define GPIO0_PIN1_FUNC_TWI0_SDA 2
#define GPIO0_PIN1_FUNC_UART1_RX 3
#define GPIO0_PIN1_FUNC_SPI1_CLK 4

#define GPIO0_PIN2_FUNC_GPIO     0
#define GPIO0_PIN2_FUNC_CAN0_P   1
#define GPIO0_PIN2_FUNC_SPI1_CE  2
#define GPIO0_PIN2_FUNC_TWI1_SCL 3
#define GPIO0_PIN2_FUNC_UART2_TX 4

#define GPIO0_PIN3_FUNC_GPIO      0
#define GPIO0_PIN3_FUNC_CAN1_P    1
#define GPIO0_PIN3_FUNC_TWI0_SCL  2
#define GPIO0_PIN3_FUNC_UART1_TX  3
#define GPIO0_PIN3_FUNC_SPI1_MOSI 4

#define GPIO0_PIN4_FUNC_GPIO     0
#define GPIO0_PIN4_FUNC_UART1_RX 1
#define GPIO0_PIN4_FUNC_TWI0_SDA 2
#define GPIO0_PIN4_FUNC_SPI0_CLK 3
#define GPIO0_PIN4_FUNC_CAN0_N   4

#define GPIO0_PIN5_FUNC_GPIO     0
#define GPIO0_PIN5_FUNC_TIM2_CH0 1
#define GPIO0_PIN5_FUNC_TIM3_CH0 2
#define GPIO0_PIN5_FUNC_TIM4_CH0 3
#define GPIO0_PIN5_FUNC_TIM5_CH0 4

#define GPIO0_PIN6_FUNC_GPIO      0
#define GPIO0_PIN6_FUNC_UART1_TX  1
#define GPIO0_PIN6_FUNC_TWI0_SCL  2
#define GPIO0_PIN6_FUNC_SPI0_MOSI 3
#define GPIO0_PIN6_FUNC_CAN0_P    4

#define GPIO0_PIN7_FUNC_GPIO     0
#define GPIO0_PIN7_FUNC_TIM2_CH1 1
#define GPIO0_PIN7_FUNC_TIM3_CH1 2
#define GPIO0_PIN7_FUNC_TIM4_CH1 3
#define GPIO0_PIN7_FUNC_TIM5_CH1 4

#define GPIO1_PIN0_FUNC_GPIO      0
#define GPIO1_PIN0_FUNC_UART2_RX  1
#define GPIO1_PIN0_FUNC_TWI1_SDA  2
#define GPIO1_PIN0_FUNC_SPI0_MISO 3
#define GPIO1_PIN0_FUNC_CAN1_P    4

#define GPIO1_PIN1_FUNC_GPIO     0
#define GPIO1_PIN1_FUNC_TIM2_CH2 1
#define GPIO1_PIN1_FUNC_TIM3_CH2 2
#define GPIO1_PIN1_FUNC_TIM4_CH2 3
#define GPIO1_PIN1_FUNC_TIM5_CH2 4

#define GPIO1_PIN2_FUNC_GPIO     0
#define GPIO1_PIN2_FUNC_UART2_TX 1
#define GPIO1_PIN2_FUNC_TWI1_SCL 2
#define GPIO1_PIN2_FUNC_SPI0_CE  3
#define GPIO1_PIN2_FUNC_CAN1_N   4

#define GPIO1_PIN3_FUNC_GPIO     0
#define GPIO1_PIN3_FUNC_TIM0_CH0 1
#define GPIO1_PIN3_FUNC_TIM1_CH0 2
#define GPIO1_PIN3_FUNC_TIM2_CH0 3
#define GPIO1_PIN3_FUNC_TIM3_CH0 4

#define GPIO1_PIN4_FUNC_GPIO     0
#define GPIO1_PIN4_FUNC_CAN0_P   1
#define GPIO1_PIN4_FUNC_UART3_RX 2
#define GPIO1_PIN4_FUNC_TWI0_SDA 3
#define GPIO1_PIN4_FUNC_SPI1_CLK 4

#define GPIO1_PIN5_FUNC_GPIO     0
#define GPIO1_PIN5_FUNC_TIM0_CH1 1
#define GPIO1_PIN5_FUNC_TIM1_CH1 2
#define GPIO1_PIN5_FUNC_TIM2_CH1 3
#define GPIO1_PIN5_FUNC_TIM3_CH1 4

#define GPIO1_PIN6_FUNC_GPIO      0
#define GPIO1_PIN6_FUNC_CAN1_N    1
#define GPIO1_PIN6_FUNC_UART3_TX  2
#define GPIO1_PIN6_FUNC_TWI0_SCL  3
#define GPIO1_PIN6_FUNC_SPI1_MOSI 4

#define GPIO1_PIN7_FUNC_GPIO     0
#define GPIO1_PIN7_FUNC_SPI0_CE  1
#define GPIO1_PIN7_FUNC_CAN1_P   2
#define GPIO1_PIN7_FUNC_UART3_RX 3
#define GPIO1_PIN7_FUNC_TWI0_SDA 4

#define GPIO2_PIN0_FUNC_GPIO      0
#define GPIO2_PIN0_FUNC_CAN1_P    1
#define GPIO2_PIN0_FUNC_TWI0_SDA  2
#define GPIO2_PIN0_FUNC_UART2_RX  3
#define GPIO2_PIN0_FUNC_SPI1_MISO 4

#define GPIO2_PIN1_FUNC_GPIO      0
#define GPIO2_PIN1_FUNC_UART3_RX  1
#define GPIO2_PIN1_FUNC_SPI1_MISO 2
#define GPIO2_PIN1_FUNC_TWI0_SDA  3
#define GPIO2_PIN1_FUNC_CAN0_N    4

#define GPIO2_PIN2_FUNC_GPIO     0
#define GPIO2_PIN2_FUNC_UART3_RX 1
#define GPIO2_PIN2_FUNC_TWI0_SCL 2
#define GPIO2_PIN2_FUNC_UART2_TX 3
#define GPIO2_PIN2_FUNC_SPI1_CE  4

#define GPIO2_PIN3_FUNC_GPIO     0
#define GPIO2_PIN3_FUNC_UART2_TX 1
#define GPIO2_PIN3_FUNC_SPI1_CE  2
#define GPIO2_PIN3_FUNC_TWI0_SCL 3
#define GPIO2_PIN3_FUNC_CAN0_P   4

#define GPIO2_PIN4_FUNC_GPIO     0
#define GPIO2_PIN4_FUNC_SPI1_CLK 1
#define GPIO2_PIN4_FUNC_TWI1_SDA 2
#define GPIO2_PIN4_FUNC_CAN0_N   3
#define GPIO2_PIN4_FUNC_UART3_RX 4

#define GPIO2_PIN5_FUNC_GPIO      0
#define GPIO2_PIN5_FUNC_SPI0_MISO 1
#define GPIO2_PIN5_FUNC_CAN1_N    2
#define GPIO2_PIN5_FUNC_TWI1_SDA  3
#define GPIO2_PIN5_FUNC_UART1_RX  4

#define GPIO2_PIN6_FUNC_GPIO      0
#define GPIO2_PIN6_FUNC_SPI1_MOSI 1
#define GPIO2_PIN6_FUNC_TWI1_SCL  2
#define GPIO2_PIN6_FUNC_CAN0_P    3
#define GPIO2_PIN6_FUNC_UART3_TX  4

#define GPIO2_PIN7_FUNC_GPIO     0
#define GPIO2_PIN7_FUNC_SPI0_CLK 1
#define GPIO2_PIN7_FUNC_CAN1_P   2
#define GPIO2_PIN7_FUNC_TWI1_SCL 3
#define GPIO2_PIN7_FUNC_UART1_TX 4

#define GPIO3_PIN0_FUNC_GPIO      0
#define GPIO3_PIN0_FUNC_SPI0_MOSI 1
#define GPIO3_PIN0_FUNC_CAN1_N    2
#define GPIO3_PIN0_FUNC_UART3_TX  3
#define GPIO3_PIN0_FUNC_TWI0_SCL  4

#define GPIO3_PIN1_FUNC_GPIO     0
#define GPIO3_PIN1_FUNC_TIM0_CH0 1
#define GPIO3_PIN1_FUNC_TIM1_CH0 2
#define GPIO3_PIN1_FUNC_TIM2_CH0 3
#define GPIO3_PIN1_FUNC_TIM3_CH0 4

#define GPIO3_PIN2_FUNC_GPIO     0
#define GPIO3_PIN2_FUNC_UART1_RX 1
#define GPIO3_PIN2_FUNC_CAN1_N   2
#define GPIO3_PIN2_FUNC_TWI1_SDA 3
#define GPIO3_PIN2_FUNC_UART2_RX 4

#define GPIO3_PIN3_FUNC_GPIO     0
#define GPIO3_PIN3_FUNC_UART1_TX 1
#define GPIO3_PIN3_FUNC_CAN1_P   2
#define GPIO3_PIN3_FUNC_TWI1_SCL 3
#define GPIO3_PIN3_FUNC_UART2_TX 4

#define GPIO3_PIN4_FUNC_GPIO     0
#define GPIO3_PIN4_FUNC_TIM0_CH1 1
#define GPIO3_PIN4_FUNC_TIM1_CH1 2
#define GPIO3_PIN4_FUNC_TIM2_CH1 3
#define GPIO3_PIN4_FUNC_TIM3_CH1 4

#define GPIO3_PIN5_FUNC_GPIO     0
#define GPIO3_PIN5_FUNC_TIM0_CH2 1
#define GPIO3_PIN5_FUNC_TIM1_CH2 2
#define GPIO3_PIN5_FUNC_TIM2_CH2 3
#define GPIO3_PIN5_FUNC_TIM3_CH2 4

#define GPIO3_PIN6_FUNC_GPIO     0
#define GPIO3_PIN6_FUNC_TWI0_SDA 1
#define GPIO3_PIN6_FUNC_SPI0_CLK 2
#define GPIO3_PIN6_FUNC_CAN0_N   3
#define GPIO3_PIN6_FUNC_UART1_RX 4

#define GPIO3_PIN7_FUNC_GPIO      0
#define GPIO3_PIN7_FUNC_TWI0_SCL  1
#define GPIO3_PIN7_FUNC_SPI0_MOSI 2
#define GPIO3_PIN7_FUNC_CAN0_P    3
#define GPIO3_PIN7_FUNC_UART1_TX  4

#define GPIO4_PIN0_FUNC_GPIO     0
#define GPIO4_PIN0_FUNC_TIM2_CH0 1
#define GPIO4_PIN0_FUNC_TIM3_CH0 2
#define GPIO4_PIN0_FUNC_TIM4_CH0 3
#define GPIO4_PIN0_FUNC_TIM5_CH0 4

#define GPIO4_PIN1_FUNC_GPIO      0
#define GPIO4_PIN1_FUNC_TWI1_SDA  1
#define GPIO4_PIN1_FUNC_SPI0_MISO 2
#define GPIO4_PIN1_FUNC_CAN1_N    3
#define GPIO4_PIN1_FUNC_UART2_RX  4

#define GPIO4_PIN2_FUNC_GPIO     0
#define GPIO4_PIN2_FUNC_TIM2_CH1 1
#define GPIO4_PIN2_FUNC_TIM3_CH1 2
#define GPIO4_PIN2_FUNC_TIM4_CH1 3
#define GPIO4_PIN2_FUNC_TIM5_CH1 4

#define GPIO4_PIN3_FUNC_GPIO     0
#define GPIO4_PIN3_FUNC_TWI1_SCL 1
#define GPIO4_PIN3_FUNC_SPI0_CE  2
#define GPIO4_PIN3_FUNC_CAN1_P   3
#define GPIO4_PIN3_FUNC_UART2_TX 4

#define GPIO5_PIN0_FUNC_GPIO     0
#define GPIO5_PIN0_FUNC_TIM3_CH1 1
#define GPIO5_PIN0_FUNC_TIM2_CH1 2
#define GPIO5_PIN0_FUNC_TIM1_CH1 3
#define GPIO5_PIN0_FUNC_TIM0_CH1 4

#define GPIO5_PIN1_FUNC_GPIO     0
#define GPIO5_PIN1_FUNC_TIM0_CH2 1
#define GPIO5_PIN1_FUNC_TIM1_CH2 2
#define GPIO5_PIN1_FUNC_TIM2_CH0 3
#define GPIO5_PIN1_FUNC_TIM3_CH0 4

#define GPIO5_PIN2_FUNC_GPIO     0
#define GPIO5_PIN2_FUNC_TIM4_CH1 1
#define GPIO5_PIN2_FUNC_TIM3_CH1 2
#define GPIO5_PIN2_FUNC_TIM5_CH1 3
#define GPIO5_PIN2_FUNC_TIM1_CH1 4

#define GPIO5_PIN3_FUNC_GPIO     0
#define GPIO5_PIN3_FUNC_TIM5_CH2 1
#define GPIO5_PIN3_FUNC_TIM4_CH2 2
#define GPIO5_PIN3_FUNC_TIM3_CH2 3
#define GPIO5_PIN3_FUNC_TIM1_CH2 4

#define GPIO5_PIN4_FUNC_GPIO     0
#define GPIO5_PIN4_FUNC_TIM1_CH0 1
#define GPIO5_PIN4_FUNC_TIM5_CH1 2
#define GPIO5_PIN4_FUNC_TIM4_CH1 3
#define GPIO5_PIN4_FUNC_TIM2_CH1 4

#define GPIO5_PIN5_FUNC_GPIO     0
#define GPIO5_PIN5_FUNC_TIM1_CH2 1
#define GPIO5_PIN5_FUNC_TIM2_CH2 2
#define GPIO5_PIN5_FUNC_TIM0_CH2 3
#define GPIO5_PIN5_FUNC_TIM3_CH1 4

#define GPIO5_PIN6_FUNC_GPIO     0
#define GPIO5_PIN6_FUNC_TIM3_CH2 1
#define GPIO5_PIN6_FUNC_TIM4_CH1 2
#define GPIO5_PIN6_FUNC_TIM1_CH0 3
#define GPIO5_PIN6_FUNC_TIM0_CH0 4

#define GPIO5_PIN7_FUNC_GPIO     0
#define GPIO5_PIN7_FUNC_TIM3_CH0 1
#define GPIO5_PIN7_FUNC_TIM4_CH2 2
#define GPIO5_PIN7_FUNC_TIM2_CH2 3
#define GPIO5_PIN7_FUNC_TIM5_CH0 4

#endif

/**
 * @}
 */
