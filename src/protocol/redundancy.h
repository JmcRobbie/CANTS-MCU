/**
 * @file redundancy.h
 *
 */

/**
 * @addtogroup cants_app CAN-TS application level
 * @{
 */

#ifndef REDUNDANCY_H_
#define REDUNDANCY_H_

/** Redundancy task stack size */
#define REDUNDANCY_STACK_SIZE configMINIMAL_STACK_SIZE
/** Redundancy task priority */
#define REDUNDANCY_PRIORITY (tskIDLE_PRIORITY + 1)

/** redundancy master ID */
#define REDUNDANCY_MASTER_ID    0xaa

/**
 *  This parameter tells how many redundancy master keepalive
 *  messages must be missed in sequence in order to switch bus.
 */
#define REDUNDANCY_MAX_MISSES   2

/** Maximum number of bus switches. */
#define REDUNDANCY_MAX_SWITCHES 7

/** Time period in which we expect redundancy master keepalive messages. */
#define REDUNDANCY_PERIOD       2000

/**
 * @brief Initialize redundancy management task.
 * @retval None
 */
void redundancy_init(void);

#endif

/**
 * @}
 */
