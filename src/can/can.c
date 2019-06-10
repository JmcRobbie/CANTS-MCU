/**
 * @file can.c
 * 
 */

/**
 * @defgroup CAN CAN
 * @brief CAN driver module
 * @{
 */

#include "can.h"
#include "bitops.h"
#include "soc.h"

#include <stdlib.h>
#include <string.h>

#define TSEG1_MIN 1		/**< TSEG1 minimum value. */
#define TSEG1_MAX 16	/**< TSEG1 maximum value. */
#define TSEG2_MIN 1		/**< TSEG2 minimum value. */
#define TSEG2_MAX 8		/**< TSEG2 maximum value. */
#define BRP_MIN   1		/**< BRP minimum value. */
#define BRP_MAX   64	/**< BRP maximum value. */
#define SJW_MAX   4		/**< SJW maximum value. */

void can_set_baudrate(struct can *base, uint32_t rate, uint16_t sp)
{
	uint32_t bitrate, br_error, best_br_error = UINT32_MAX;
	uint16_t real_sp, sp_error, best_sp_error = UINT16_MAX;
	uint8_t tseg1, best_tseg1 = 0;
	uint8_t tseg2, best_tseg2 = 0;
	uint8_t brp, best_brp = 0;
	uint8_t i, full, tseg, sjw;

	if (!sp)
		sp = 875;

	/* it is preferred to have high tseg value */
	for (tseg = (TSEG1_MAX + TSEG2_MAX); tseg >= (TSEG1_MIN + TSEG2_MIN); tseg--) {
		/* account for sync quanta */
		full = 1 + tseg;

		brp = SOC_CLOCK / 2 / (full * rate);
		if ((brp < BRP_MIN) || (brp > BRP_MAX))
			continue;

		bitrate = SOC_CLOCK / 2 / (brp * full);
		br_error = abs(rate - bitrate);

		/* prefer lower bitrate error over sampling point error */
		if (br_error > best_br_error)
			continue;

		/* reset sample point error if bitrate changes */
		if (br_error < best_br_error)
			best_sp_error = UINT16_MAX;

		/* find closest sampling point */
		/* test for round up and round down */
		for (i = 0; i < 2; i++) {
			tseg2 = full - (sp * full) / 1000 - i;
			tseg2 = Clamp(tseg2, TSEG2_MIN, TSEG2_MAX);
			tseg1 = tseg - tseg2;
			if (tseg1 > TSEG1_MAX) {
				tseg1 = TSEG1_MAX;
				tseg2 = tseg - tseg1;
			}

			real_sp = 1000 * (full - tseg2) / full;
			sp_error = abs(sp - real_sp);

			if ((real_sp <= sp) && (sp_error < best_sp_error)) {
				best_sp_error = sp_error;
				best_br_error = br_error;
				best_tseg1 = tseg1;
				best_tseg2 = tseg2;
				best_brp = brp;
			}
		}

		if (best_br_error == 0 && best_sp_error == 0)
			break;
	}

	sjw = best_tseg2 > SJW_MAX ? SJW_MAX : best_tseg2;

	base->btr0 = ((sjw - 1) << 6) | (best_brp - 1);
	base->btr1 = ((best_tseg2 - 1) << 4) | (best_tseg1 - 1);
}

void can_set_rx_filter(struct can *base, uint32_t id, uint32_t mask, bool extended)
{
	/* mask logic is inverted in HW, 1 - don't care, 0 - has to match */
	mask =~ mask;

	SetBitsTyped(base->mod, CAN_MOD_AFM, uint8_t);

	if (extended) {
		base->u.rst.acr[0] = (uint8_t)(id >> 21);
		base->u.rst.acr[1] = (uint8_t)(id >> 13);
		base->u.rst.acr[2] = (uint8_t)(id >> 5);
		base->u.rst.acr[3] = (uint8_t)(id << 3);
		base->u.rst.amr[0] = (uint8_t)(mask >> 21);
		base->u.rst.amr[1] = (uint8_t)(mask >> 13);
		base->u.rst.amr[2] = (uint8_t)(mask >> 5);
		base->u.rst.amr[3] = (uint8_t)((mask << 3) | 0x07);
	} else {
		base->u.rst.acr[0] = (uint8_t)(id >> 3);
		base->u.rst.acr[1] = (uint8_t)(id << 5);
		base->u.rst.amr[0] = (uint8_t)(mask >> 3);
		base->u.rst.amr[1] = (uint8_t)((mask << 5) | 0x1f);
		base->u.rst.amr[2] = 0xff;
		base->u.rst.amr[3] = 0xff;
	}
}

void can_set_dual_rx_filter(struct can *base, uint32_t id1, uint32_t mask1, uint32_t id2, uint32_t mask2, bool extended)
{
	/* mask logic is inverted in HW, 1 - don't care, 0 - has to match */
	mask1 =~ mask1;
	mask2 =~ mask2;

	ClrBitsTyped(base->mod, CAN_MOD_AFM, uint8_t);

	if (extended) {
		base->u.rst.acr[0] = (uint8_t)(id1 >> 21);
		base->u.rst.acr[1] = (uint8_t)(id1 >> 13);
		base->u.rst.amr[0] = (uint8_t)(mask1 >> 21);
		base->u.rst.amr[1] = (uint8_t)(mask1 >> 13);

		base->u.rst.acr[2] = (uint8_t)(id2 >> 21);
		base->u.rst.acr[3] = (uint8_t)(id2 >> 13);
		base->u.rst.amr[2] = (uint8_t)(mask2 >> 21);
		base->u.rst.amr[3] = (uint8_t)(mask2 >> 13);
	} else {
		base->u.rst.acr[0] = (uint8_t)(id1 >> 3);
		base->u.rst.acr[1] = (uint8_t)(id1 << 5);
		base->u.rst.amr[0] = (uint8_t)(mask1 >> 3);
		base->u.rst.amr[1] = (uint8_t)((mask1 << 5) | 0x1f);

		base->u.rst.acr[2] = (uint8_t)(id2 >> 3);
		base->u.rst.acr[3] = (uint8_t)(id2 << 5);
		base->u.rst.amr[2] = (uint8_t)(mask2 >> 3);
		base->u.rst.amr[3] = (uint8_t)((mask2 << 5) | 0x1f);
	}
}

void can_enable_interrupt(struct can *base, uint8_t irq, bool enabled)
{
	WriteBitsTyped(base->ier, irq, enabled, uint8_t);
}

void can_send_packet(struct can *base, uint32_t id, uint8_t len, uint8_t *data, bool rtr, bool extended)
{
	uint8_t frm;

	if (len > 8)
		len = 8;

	frm = len;
	WriteBitsTyped(frm, CAN_FRM_FF, extended, uint8_t);
	WriteBitsTyped(frm, CAN_FRM_RTR, rtr, uint8_t);
	base->u.op.frm = frm;

	if (extended) {
		base->u.op.data[0] = (uint8_t)(id >> 21);
		base->u.op.data[1] = (uint8_t)(id >> 13);
		base->u.op.data[2] = (uint8_t)(id >> 5);
		base->u.op.data[3] = (uint8_t)(id << 3);
	} else {
		base->u.op.data[0] = (uint8_t)(id >> 3);
		base->u.op.data[1] = (uint8_t)(id << 5);
	}

	if (!rtr) {
		if (extended)
			memcpy((void *)&base->u.op.data[4], data, len);
		else
			memcpy((void *)&base->u.op.data[2], data, len);
	}

	base->cmr = CAN_CMR_TR;
}

void can_recv_packet(struct can *base, uint32_t *id, uint8_t *len, uint8_t *data, bool *rtr, bool *extended)
{
	uint8_t frm = base->u.op.frm;

	*extended = frm & CAN_FRM_FF;
	*rtr = frm & CAN_FRM_RTR;
	*len = frm & CAN_FRM_DLC_MSK;

	if (*extended)
		*id = ((uint32_t)base->u.op.data[0] << 21) | ((uint32_t)base->u.op.data[1] << 13) |
			  ((uint32_t)base->u.op.data[2] << 5) | (base->u.op.data[3] >> 3);
	else
		*id = ((uint32_t)base->u.op.data[0] << 3) | (base->u.op.data[1] >> 5);

	if (!*rtr) {
		if (*extended)
			memcpy(data, (void *)&base->u.op.data[4], *len);
		else
			memcpy(data, (void *)&base->u.op.data[2], *len);
	}

	base->cmr = CAN_CMR_RRB;
}

uint8_t can_get_status(struct can *base)
{
	return base->sr;
}

uint8_t can_get_int_status(struct can *base)
{
	return base->ir;
}

void can_reset_mode(struct can *base, bool enabled)
{
	WriteBitsTyped(base->mod, CAN_MOD_RM, enabled, uint8_t);
}

uint8_t can_get_rx_error_count(struct can *base)
{
	return base->rxerr;
}

uint8_t can_get_tx_error_count(struct can *base)
{
	return base->txerr;
}

void can_command(struct can *base, uint8_t cmd)
{
	base->cmr = cmd;
}

/**
 * @}
 */
