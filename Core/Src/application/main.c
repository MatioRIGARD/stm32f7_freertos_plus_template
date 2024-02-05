#include "app_freertos_tasks.h"
#include "app_network.h"
#include "app_stm32hal.h"
#include "app_mqtt.h"
#include "FreeRTOS.h"

extern void vLoggingPrintf( const char * pcFormatString, ... );

int main(void)
{
	app_halInit();
	osKernelInitialize();
	app_initNetwork();
	vStartSimpleMQTTDemo();
	// app_initMqtt();
	app_initTasks();
	
	vLoggingPrintf("Hello World!\r\n");

    vTaskStartScheduler();
	
	while (1);
}

