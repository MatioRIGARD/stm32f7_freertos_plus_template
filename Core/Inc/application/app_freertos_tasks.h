#ifndef APP_FREERTOS_TASKS_H
#define APP_FREERTOS_TASKS_H

#include "cmsis_os.h"

void app_initTasks(void);
void startTask1(void *argument);
void startTask2(void *argument);
void startDefaultTask(void *argument);
void simpleMqttTestTask(void *argument);

#endif  // APP_FREERTOS_TASKS_H