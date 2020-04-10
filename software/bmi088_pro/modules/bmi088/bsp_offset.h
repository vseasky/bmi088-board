#ifndef _BSP_OFFSET_H
#define _BSP_OFFSET_H
#include "struct_typedef.h"
#define CALIED_FLAG  0x50  //已校验状态标志
#define CALI_SENSOR_HEAD_LEGHT  1

#define CALI_FUNC_CMD_ON        1                   //设置校准
#define CALI_FUNC_CMD_INIT      0                   //已经校准过,设置校准值
#define GYRO_CALIBRATE_TIME         10000   		//陀螺仪校准时间

#define gyro_cali_fun(cali_scale, cali_offset, time_count)  imu_cali_gyro((cali_scale), (cali_offset), (time_count))
#define gyro_set_cali(cali_scale, cali_offset)              imu_set_cali_gyro((cali_scale), (cali_offset))

#define cali_flash_read(address, buf, len)  flash_read((address), (buf), (len))                     //flash read function, flash 读取函数
#define cali_flash_write(address, buf, len) flash_write_single_address((address), (buf), (len))     //flash write function,flash 写入函数
#define cali_flash_erase(address, page_num) flash_erase_address((address), (page_num))

typedef enum
{
    CALI_GYRO = 0,
    CALI_ACC = 1,
    CALI_MAG = 2,
    CALI_LENGHT,
} cali_id_t;

typedef __packed struct
{
    uint8_t name[3];
    uint8_t cali_done;
    uint8_t flash_len :7;
    uint8_t cali_cmd :1;
    uint32_t *flash_buf;
    bool_t (*cali_hook)(uint32_t *point, bool_t cmd);
} cali_sensor_t;

typedef struct
{
    fp32 off_gyro_set[3]; 
	fp32 gyro_scale[3];
} imu_gyro_cali_t;
typedef struct
{
    fp32 off_accel_set[3];
	fp32 accel_scale[3];
} imu_accel_cali_t;
typedef struct
{
    fp32 off_mag_set[3]; 
	fp32 mag_scale[3];
} imu_mag_cali_t;

void cali_param_init(void);
void calibrate_imu(void);
void cali_cmd(uint8_t i,uint8_t cmd);
#endif
