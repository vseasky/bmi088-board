#ifndef _BSP_INIT_H
#define _BSP_INIT_H
#include "bsp_bmi088.h"
#include "key_task.h"

typedef struct
{
	const imu_sensor *imu_info; 
	const mx_key_info *key_info;
}mx_info_sum;

void get_point_init(void);
extern mx_info_sum mx_info_sum_t;
#endif
