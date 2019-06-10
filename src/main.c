/**
 * @file main.c
 *
 */

/**
 * @defgroup Main Main
 * @brief Main functions
 * @{
 */

#include "candrv.h"
#include "FreeRTOS.h"
#include "gpio.h"
#include "soc.h"
#include "task.h"

/* idle task related variables */
static StaticTask_t xIdleTaskTCB;
static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

int main(void)
{
	/* set LED pins to output direction and turn them off */
	gpio_set_port(GPIO5, 0);
	gpio_set_direction(GPIO5, GPIO_PIN_ALL, GPIO_DIR_OUT);

	/* Initialize CAN-TS stack and CAN controller */
	candrv_init();

	/* run tasks */
	vTaskStartScheduler();

	return 0;
}

/**
 * @}
 */
