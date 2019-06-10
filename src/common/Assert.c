/**
 * @file Assert.c
 *
 */

/**
 * @addtogroup Common
 * @{
 */

#include "Assert.h"

/**
 * @brief Stops execution of firmware.
 * @retval None
 */
static void assert_abort(void)
{
	/* execute HW breakpoint */
	__asm__ volatile("brk");
}

void CheckAssert(bool condition)
{
	/* abort execution if condition is false */
	if (!condition)
		assert_abort();
}

/**
 * @}
 */
