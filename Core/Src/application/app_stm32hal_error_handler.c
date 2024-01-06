#include "app_stm32hal_error_handler.h"
#include "stm32f7xx_hal.h"


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM6)
	{
		HAL_IncTick();
	}
}

void Error_Handler(void)
{
	__disable_irq();
	while (1)
	{
	}
}
