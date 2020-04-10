#ifndef IMU_Task_H
#define IMU_Task_H
#include "struct_typedef.h"
#include "FreeRtos.h"




#define IMU_TASK_INIT_TIME1 7 //开启任务延时时间(单位ms)
#define IMU_TASK_INIT_TIME2 50 //开启任务延时时间(单位ms)




void imu_task_creat(void);
void imu_task(void const *pvParameters);

#endif
