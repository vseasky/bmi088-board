#include "imu_task.h"

#include "cmsis_os.h"

#include "ahrs.h"
#include "bsp_spi.h"
#include "spi.h"
#include "bmi088driver.h"
#include "ist8310driver.h"
#include "bsp_bmi088.h"

#include "bsp_spi.h"
#include "bsp_offset.h"

#include "bsp_init.h"
extern SPI_HandleTypeDef hspi1;
extern void BMI088_Drive_Init(void);
extern void BMI088_Read_update(void);
extern void gyro_offset_init(void);
extern void bim088_gyro_offset_update(void);

osThreadId imuTaskHandle;
void imu_task_creat(void)
{
  osThreadDef(imuTask, imu_task, osPriorityRealtime, 0, 512);
  imuTaskHandle = osThreadCreate(osThread(imuTask), NULL);
}
void imu_task(void const *pvParameters)
{
	TickType_t peroid = osKernelSysTick();
	/*开启任务延时时间(单位ms)*/
	osDelay(IMU_TASK_INIT_TIME1);
	/*初始化BMI088*/
   	while(BMI088_Init()){osDelay(100);}
	/*初始化ist8310*/
//    while(ist8310_init()){osDelay(100);}
	BMI088_Drive_Init();
	cali_param_init();//获取校准值，从FLASH读取，如果没有，则开启校准，并将校准值存入FLASH
	//获取当前任务的任务句柄，
	// imu_Task_handle = xTaskGetHandle(pcTaskGetName(NULL));
	osDelay(IMU_TASK_INIT_TIME2);
	while(1)
	{
		BMI088_Read_update();//读取BMI088数据
		calibrate_imu();//姿态校准，数据存放到片内FLASH
		osDelayUntil(&peroid,IMU_TASK_PERIOD);//任务延时
	}
}


