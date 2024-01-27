#include "app_freertos_tasks.h"
#include "stm32f7xx_hal.h"

// FreeRTOS
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
	.name = "defaultTask",
	.stack_size = 128 * 4,
	.priority = (osPriority_t)osPriorityNormal,
};

osThreadId_t task1Handle;
const osThreadAttr_t task1_attributes = {
	.name = "task1Task",
	.stack_size = 128 * 4,
	.priority = (osPriority_t)osPriorityNormal,
};

osThreadId_t task2Handle;
const osThreadAttr_t task2_attributes = {
	.name = "task2Task",
	.stack_size = 128 * 4,
	.priority = (osPriority_t)osPriorityNormal,
};

void app_initTasks(void) {
	defaultTaskHandle = osThreadNew(startDefaultTask, NULL, &defaultTask_attributes);
	task1Handle = osThreadNew(startTask1, NULL, &task1_attributes);
	task2Handle = osThreadNew(startTask2, NULL, &task2_attributes);
}

void startTask1(void *argument)
{
	for (;;)
	{
		HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_0, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_1, GPIO_PIN_SET);
		osDelay(500);
		HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_0, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_1, GPIO_PIN_RESET);
		osDelay(500);
	}
}

void startTask2(void *argument)
{
	for (;;)
	{
		osDelay(500);
		HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_3, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOI, GPIO_PIN_15, GPIO_PIN_SET);
		osDelay(500);
		HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_3, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOI, GPIO_PIN_15, GPIO_PIN_RESET);
	}
}

void startDefaultTask(void *argument)
{
	for (;;)
	{
		osDelay(1);
	}
}

void simpleMqttTestTask(void *argument)
{
	for (;;)
	{
		osDelay(1);
	}
}