#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f7xx_hal.h"

void Error_Handler(void);
void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_USART1_UART_Init(void);
void MX_ETH_Init(void);

// tasks
void StartDefaultTask(void *argument);
void StartTask1(void *argument);
void StartTask2(void *argument);

#endif /* __MAIN_H */
