#include "led_task.h"

#include "cmsis_os.h"
#include "main.h"

void led_task(void const *pvParameters);

osThreadId ledTaskHandle;
void led_task_creat(void)
{
  osThreadDef(ledTask,led_task,osPriorityRealtime,0,128);
  ledTaskHandle = osThreadCreate(osThread(ledTask), NULL);
}
void led_task(void const *pvParameters)
{

	while(1)
	{
		HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
		HAL_GPIO_TogglePin(LED2_GPIO_Port,LED2_Pin);
		osDelay(500);
	}
}

