#include "bsp_init.h"
mx_info_sum mx_info_sum_t;
void get_point_init(void)
{
	mx_info_sum_t.imu_info = get_imu_point();
	mx_info_sum_t.key_info = get_key_info_pointer();
}