/**
 * @file vectors.S
 *
 */

#if (__PICOSKY_HAVE_JP_JS__)
#define XJMP jp
#else
#define XJMP jpr
#endif

	.section .vectors,"ax",@progbits
	.global	__vectors
	.func	__vectors
__vectors:
	XJMP __init
	XJMP gpio0_handler
	XJMP gpio1_handler
	XJMP gpio2_handler
	XJMP gpio3_handler
	XJMP gpio4_handler
	XJMP gpio5_handler
	XJMP timer0_handler
	XJMP timer1_handler
	XJMP timer2_handler
	XJMP timer3_handler
	XJMP timer4_handler
	XJMP timer5_handler
	XJMP uart0tx_handler
	XJMP uart0rx_handler
	XJMP uart1tx_handler
	XJMP uart1rx_handler
	XJMP uart2tx_handler
	XJMP uart2rx_handler
	XJMP uart3tx_handler
	XJMP uart3rx_handler
	XJMP can0_handler
	XJMP can1_handler
	XJMP spi0tx_handler
	XJMP spi0rx_handler
	XJMP spi0err_handler
	XJMP spi1tx_handler
	XJMP spi1rx_handler
	XJMP spi1err_handler
	XJMP twi0_handler
	XJMP twi1_handler
	XJMP emmc_handler
	XJMP adc_handler
	.endfunc

	.weak gpio0_handler
	.set gpio0_handler, default_handler
	.weak gpio1_handler
	.set gpio1_handler, default_handler
	.weak gpio2_handler
	.set gpio2_handler, default_handler
	.weak gpio3_handler
	.set gpio3_handler, default_handler
	.weak gpio4_handler
	.set gpio4_handler, default_handler
	.weak gpio5_handler
	.set gpio5_handler, default_handler

	.weak timer0_handler
	.set timer0_handler, default_handler
	.weak timer1_handler
	.set timer1_handler, default_handler
	.weak timer2_handler
	.set timer2_handler, default_handler
	.weak timer3_handler
	.set timer3_handler, default_handler
	.weak timer4_handler
	.set timer4_handler, default_handler
	.weak timer5_handler
	.set timer5_handler, default_handler

	.weak uart0tx_handler
	.set uart0tx_handler, default_handler
	.weak uart0rx_handler
	.set uart0rx_handler, default_handler
	.weak uart1tx_handler
	.set uart1tx_handler, default_handler
	.weak uart1rx_handler
	.set uart1rx_handler, default_handler
	.weak uart2tx_handler
	.set uart2tx_handler, default_handler
	.weak uart2rx_handler
	.set uart2rx_handler, default_handler
	.weak uart3tx_handler,
	.set uart3tx_handler, default_handler
	.weak uart3rx_handler
	.set uart3rx_handler, default_handler

	.weak can0_handler
	.set can0_handler, default_handler
	.weak can1_handler
	.set can1_handler, default_handler

	.weak spi0tx_handler
	.set spi0tx_handler, default_handler
	.weak spi0rx_handler
	.set spi0rx_handler, default_handler
	.weak spi0err_handler
	.set spi0err_handler, default_handler
	.weak spi1tx_handler
	.set spi1tx_handler, default_handler
	.weak spi1rx_handler
	.set spi1rx_handler, default_handler
	.weak spi1err_handler
	.set spi1err_handler, default_handler

	.weak twi0_handler
	.set twi0_handler, default_handler
	.weak twi1_handler
	.set twi1_handler, default_handler

	.weak emmc_handler
	.set emmc_handler, default_handler

	.weak adc_handler
	.set adc_handler, default_handler

	.text
	.global	default_handler
	.func	default_handler
default_handler:
	; Handle unexpected interrupts (enabled and no handler), which
	; usually indicate a bug.  Jump to the reset address.
	;
	; This must be in a different section, otherwise the assembler
	; will resolve "jpr" offsets and there will be no relocs.

#if DEBUG
	brk
#endif
	XJMP __vectors
	.endfunc

/**
 * @}
 */
