/**
 * @file Assert.h
 *
 */

/**
 * @defgroup Common Common
 * @brief Common functions and macros
 * @{
 */

#ifndef ASSERT_H_
#define ASSERT_H_

#include <stdbool.h>

#if TARGET != Debug
#define Assert(cond) (void)(cond)
#else
#define Assert CheckAssert
#endif

/**
 * @brief Stops execution if condition is evaluated to false
 * @param [in] condition condition to evaluate
 * @retval None
 */
void CheckAssert(bool condition);

#endif

/**
 * @}
 */
