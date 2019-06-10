/**
 * @file utils.c
 *
 */

/**
 * @addtogroup CAN-TS
 * @{
 */

#include "cants.h"

/* helper union for CAN ID conversion */
union id_convert {
	uint32_t id;
	struct {
		uint32_t cmd  : 10;
		uint32_t src  : 8;
		uint32_t type : 3;
		uint32_t dst  : 8;
	} fields;
};

void cants_parse_id(struct cants_msg *out, uint32_t id)
{
	union id_convert tmp = { id };

	cants_assert(out);

	/* convert CAN ID to CAN-TS fields using union */
	out->command = tmp.fields.cmd;
	out->source = tmp.fields.src;
	out->type = tmp.fields.type;
	out->destination = tmp.fields.dst;
}

uint32_t cants_construct_id(struct cants_msg *in)
{
	union id_convert tmp;

	cants_assert(in);

	/* convert CAN-TS fields to CAN ID using union */
	tmp.fields.cmd = in->command;
	tmp.fields.src = in->source;
	tmp.fields.type = in->type;
	tmp.fields.dst = in->destination;

	return tmp.id;
}

/**
 * @}
 */
