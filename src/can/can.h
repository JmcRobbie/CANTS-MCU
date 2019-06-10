/**
 * @file can.h
 */

/** 
 * @addtogroup CAN
 * @{
 */

#ifndef CAN_H_
#define CAN_H_

#ifndef __ASSEMBLER__
#include <stdint.h>
#include <stdbool.h>
#endif

#include "bitops.h"

/**
 * @name CAN interrupt bits
 *@{
 */
#define CAN_IRQ_BEI Bit(7) /**< Bus error interrupt */
#define CAN_IRQ_ALI Bit(6) /**< Arbitration lost interrupt */
#define CAN_IRQ_EPI Bit(5) /**< Error passive interrupt */
#define CAN_IRQ_DOI Bit(3) /**< Data overrun interrupt */
#define CAN_IRQ_EI  Bit(2) /**< Error interrupt */
#define CAN_IRQ_TI  Bit(1) /**< Transmit interrupt */
#define CAN_IRQ_RI  Bit(0) /**< Receive interrupt */
/**
 *@}
 */

/**
 * @name CAN status register bits
 *@{
 */
#define CAN_SR_BS  Bit(7) /**< Bus status */
#define CAN_SR_ES  Bit(6) /**< Error status */
#define CAN_SR_TS  Bit(5) /**< Transmit status */
#define CAN_SR_RS  Bit(4) /**< Receive status */
#define CAN_SR_TCS Bit(3) /**< Transmission complete */
#define CAN_SR_TBS Bit(2) /**< Transmit buffer */
#define CAN_SR_DOS Bit(1) /**< Data overrun */
#define CAN_SR_RBS Bit(0) /**< Receive buffer */
/**
 *@}
 */

/**
 * @name CAN commands
 *@{
 */
#define CAN_CMR_SRR  Bit(4) /**< Self reception request */
#define CAN_CMR_CDO  Bit(3) /**< Clear data overrun */
#define CAN_CMR_RRB  Bit(2) /**< Release receive buffer */
#define CAN_CMR_AT   Bit(1) /**< Abort transmission */
#define CAN_CMR_TR   Bit(0) /**< Transmission request */
/**
 *@}
 */

/**
 * @name CAN modes
 *@{
 */
#define CAN_MOD_SM  Bit(4) /**< Sleep mode */
#define CAN_MOD_AFM Bit(3) /**< Acceptance filter mode */
#define CAN_MOD_STM Bit(2) /**< Self test mode */
#define CAN_MOD_LOM Bit(1) /**< Listen only mode */
#define CAN_MOD_RM  Bit(0) /**< Reset mode */
/**
 *@}
 */

/**
 * @name CAN frame bits
 *@{
 */
#define CAN_FRM_FF       Bit(7) /**< Frame format (extended, normal) */
#define CAN_FRM_RTR      Bit(6) /**< Remote transmission request */
#define CAN_FRM_DLC_MSK  0x0F   /**< Data length code mask */
/**
 *@}
 */

#ifdef __ASSEMBLER__

#define CAN_MOD(base)    (base + 0)
#define CAN_CMR(base)    (base + 1)
#define CAN_SR(base)     (base + 2)
#define CAN_IR(base)     (base + 3)
#define CAN_IER(base)    (base + 4)
#define CAN_BTR0(base)   (base + 6)
#define CAN_BTR1(base)   (base + 7)
#define CAN_ALC(base)    (base + 11)
#define CAN_ECC(base)    (base + 12)
#define CAN_EWLR(base)   (base + 13)
#define CAN_RXERR(base)  (base + 14)
#define CAN_TXERR(base)  (base + 15)
#define CAN_ACR0(base)   (base + 16)
#define CAN_ACR1(base)   (base + 17)
#define CAN_ACR2(base)   (base + 18)
#define CAN_ACR3(base)   (base + 19)
#define CAN_AMR0(base)   (base + 20)
#define CAN_AMR1(base)   (base + 21)
#define CAN_AMR2(base)   (base + 22)
#define CAN_AMR3(base)   (base + 23)
#define CAN_FRM(base)    (base + 16)
#define CAN_DATA0(base)  (base + 17)
#define CAN_DATA1(base)  (base + 18)
#define CAN_DATA2(base)  (base + 19)
#define CAN_DATA3(base)  (base + 20)
#define CAN_DATA4(base)  (base + 21)
#define CAN_DATA5(base)  (base + 22)
#define CAN_DATA6(base)  (base + 23)
#define CAN_DATA7(base)  (base + 24)
#define CAN_DATA8(base)  (base + 25)
#define CAN_DATA9(base)  (base + 26)
#define CAN_DATA10(base) (base + 27)
#define CAN_DATA11(base) (base + 28)
#define CAN_RMC(base)    (base + 29)
#define CAN_CDR(base)    (base + 31)

#else

/**
 * @struct can
 * @brief CAN (PeliCAN) registers structure
 */
struct can {
	volatile uint8_t mod; /**< Mode register */
	volatile uint8_t cmr; /**< Command register */
	volatile uint8_t sr; /**< Status register */
	volatile uint8_t ir; /**< Interrupt register */
	volatile uint8_t ier; /**< Interrupt enable register */
	volatile uint8_t r1; /**< Reserved */
	volatile uint8_t btr0; /**< Bus timing 0 register */
	volatile uint8_t btr1; /**< Bus timing 1 register */
	volatile uint8_t r2[3]; /**< Reserved */
	volatile uint8_t alc; /**< Arbitration lost capture register */
	volatile uint8_t ecc; /**< Error code capture register */
	volatile uint8_t ewlr; /**< Error warning limit register */
	volatile uint8_t rxerr; /**< RX error counter */
	volatile uint8_t txerr; /**< TX error counter */
	union {
		struct {
			volatile uint8_t acr[4]; /**< Acceptance code registers */
			volatile uint8_t amr[4]; /**< Acceptance mask registers */
		} rst;
		struct {
			volatile uint8_t frm; /**< Frame register */
			volatile uint8_t data[12]; /**< Data buffer */
		} op;
	} u;
	volatile uint8_t rmc; /**< RX message counter */
	volatile uint8_t r3; /**< Reserved */
	volatile uint8_t cdr; /**< Clock divider register */
};
/**
 *@}
 */

/**
 * @brief Set CAN baud rate.
 * @param [in] base CAN base address.
 * @param [in] rate Wanted baud rate
 * @param [in] sp Wanted sampling point in 1/1000th. If 0, default 875 (87.5%) is assumed.
 * @retval None
 */
void can_set_baudrate(struct can *base, uint32_t rate, uint16_t sp);

/**
 * @brief Set CAN RX filter.
 * @param [in] base CAN base address.
 * @param [in] id Identifier to be filtered
 * @param [in] mask Bits set to 1 in mask represents which bits in id have to match.
 * @param [in] extended Extended identifier flag. True mean 29 bit ID and false 11 bit.
 * @retval None
 */
void can_set_rx_filter(struct can *base, uint32_t id, uint32_t mask, bool extended);

/**
 * @brief Set dual CAN RX filter.
 * @param [in] base CAN base address.
 * @param [in] id1 Identifier to be filtered
 * @param [in] mask1 Bits set to 1 in mask represents which bits in id have to match.
 * @param [in] id2 Identifier to be filtered
 * @param [in] mask2 Bits set to 1 in mask represents which bits in id have to match.
 * @param [in] extended Extended identifier flag. True mean 29 bit ID and false 11 bit.
 * \note If extended ID is used, only bit 29-13 are checked.
 * @retval None
 */
void can_set_dual_rx_filter(struct can *base, uint32_t id1, uint32_t mask1, uint32_t id2, uint32_t mask2, bool extended);

/**
 * @brief Enable or disable CAN interrupts.
 * @param [in] base CAN base address.
 * @param [in] irq IRQs to set. Combination of:
 * ::CAN_IRQ_BEI::, ::CAN_IRQ_ALI::,
 * ::CAN_IRQ_EPI::, ::CAN_IRQ_DOI::,
 * ::CAN_IRQ_EI::, ::CAN_IRQ_TI::,
 * ::CAN_IRQ_RI::.
 * @param [in] enabled True to enable interrupt, false to disable.
 * @retval None
 */
void can_enable_interrupt(struct can *base, uint8_t irq, bool enabled);

/**
 * @brief Send CAN packet.
 * @param [in] base CAN base address.
 * @param [in] id Packet identifier
 * @param [in] len Packet length
 * @param [in] data Pointer to packet data
 * @param [in] rtr Remote transmission request flag
 * @param [in] extended Extended identifier flag. True mean 29 bit ID and false 11 bit.
 * @retval None
 */
void can_send_packet(struct can *base, uint32_t id, uint8_t len, uint8_t *data, bool rtr, bool extended);

/**
 * @brief Receive CAN packet.
 * @param [in] base CAN base address.
 * @param [out] id Pointer to hold packet identifier
 * @param [out] len Pointer to hold packet length
 * @param [out] data Pointer to hold packet data
 * @param [out] rtr Pointer to hold remote transmission request flag
 * @param [out] extended Pointer to hold extended identifier flag. True mean 29 bit ID and false 11 bit.
 * @retval None
 */
void can_recv_packet(struct can *base, uint32_t *id, uint8_t *len, uint8_t *data, bool *rtr, bool *extended);

/**
 * @brief Get CAN status flags.
 * @param [in] base CAN base address.
 * @retval Flags from status register
 */
uint8_t can_get_status(struct can *base);

/**
 * @brief Get CAN interrupt status flags.
 * @param [in] base CAN base address.
 * @retval Flags from interrupt status register
 */
uint8_t can_get_int_status(struct can *base);

/**
 * @brief Put CAN in/out from reset mode.
 * @param [in] base CAN base address.
 * @param [in] enabled True to go to reset mode, false to go out of it.
 * @retval None
 */
void can_reset_mode(struct can *base, bool enabled);

/**
 * @brief Get number of receive errors
 * @param [in] base CAN base address.
 * @retval Number of errors
 */
uint8_t can_get_rx_error_count(struct can *base);

/**
 * @brief Get number of transmit errors
 * @param [in] base CAN base address.
 * @retval Number of errors
 */
uint8_t can_get_tx_error_count(struct can *base);

/**
 * @brief Execute CAN command
 * @param [in] base CAN base address.
 * @param [in] cmd CAN command. Must be one of the following:
 * ::CAN_CMR_SRR::, ::CAN_CMR_CDO::,
 * ::CAN_CMR_RRB::, ::CAN_CMR_AT::,
 * ::CAN_CMR_TR::.
 * @retval None
 */
void can_command(struct can *base, uint8_t cmd);

#endif
#endif /* CAN_H_ */

/**
 * @}
 */
