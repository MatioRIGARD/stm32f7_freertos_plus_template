#include "app_freertos_tasks.h"
#include "app_network.h"
#include "app_stm32hal.h"
#include "FreeRTOS.h"

int main(void)
{
	app_halInit();
	osKernelInitialize();
	app_initNetwork();
	app_initTasks();

    vTaskStartScheduler();
	
	while (1);
}

