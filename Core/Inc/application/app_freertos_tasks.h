#ifndef APP_FREERTOS_TASKS_H
#define APP_FREERTOS_TASKS_H

#include "cmsis_os.h"

void app_initTasks(void);
void StartTask1(void *argument);
void StartTask2(void *argument);
void StartDefaultTask(void *argument);
void simpleMqttTestTask(void *argument);

#endif  // APP_FREERTOS_TASKS_H