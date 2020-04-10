#include "bsp_offset.h"
#include "bsp_flash.h"
#include "bsp_bmi088.h"
#include "string.h"

static void cali_data_read(void);
static void cali_imu_data_write(void);
static bool_t cali_gyro_hook(uint32_t *cali, bool_t cmd);
static bool_t cali_accel_hook(uint32_t *cali, bool_t cmd);
static bool_t cali_mag_hook(uint32_t *cali, bool_t cmd);


#define FLASH_USER_ADDR      ADDR_FLASH_SECTOR_9

#define FLASH_WRITE_BUF_LENGHT (sizeof(imu_gyro_cali_t)+sizeof(imu_accel_cali_t)+sizeof(imu_mag_cali_t)+CALI_LENGHT * 4)

static uint8_t flash_write_buf[FLASH_WRITE_BUF_LENGHT];

cali_sensor_t imu_cali_offset[CALI_LENGHT];
static imu_gyro_cali_t gyro_cali;
static imu_accel_cali_t accel_cali;
static imu_mag_cali_t mag_cali;
static const uint8_t cali_flash_head[CALI_LENGHT][3] = {"GYR","ACC","MAG"};
static uint8_t cali_sensor_size[CALI_LENGHT] = {sizeof(imu_gyro_cali_t)/4,
                                                sizeof(imu_accel_cali_t)/4,
                                                sizeof(imu_mag_cali_t)/4
                                               };

static uint32_t *cali_sensor_buf[CALI_LENGHT] = {(uint32_t *)&gyro_cali,
                                                 (uint32_t *)&accel_cali,
                                                 (uint32_t *)&mag_cali
                                                };
//函数
void *cali_hook_fun[CALI_LENGHT] = {cali_gyro_hook,cali_accel_hook,cali_mag_hook};

//校验初始化
void cali_param_init(void)
{
    uint8_t i = 0;
    for (i = 0; i < CALI_LENGHT; i++)
        {
            imu_cali_offset[i].flash_len = cali_sensor_size[i];
            imu_cali_offset[i].flash_buf = cali_sensor_buf[i];
            imu_cali_offset[i].cali_hook = (bool_t(*)(uint32_t *, bool_t))cali_hook_fun[i];//初始化函数指针
        }
    cali_data_read();
    for (i = 0; i < CALI_LENGHT; i++)
        {
            if (imu_cali_offset[i].cali_done == CALIED_FLAG)
                {
                    if (imu_cali_offset[i].cali_hook != NULL)
                        {
                            //如果已经校准,设置为初始化
                            imu_cali_offset[i].cali_hook(cali_sensor_buf[i],CALI_FUNC_CMD_INIT);
                        }
                }
        }
}
//imu校验控制
void calibrate_imu(void)
{
    for(uint8_t i=0; i<CALI_LENGHT; i++)
        {
            if (imu_cali_offset[i].cali_cmd)
                {
                    if (imu_cali_offset[i].cali_hook != NULL)
                        {
                            if (imu_cali_offset[i].cali_hook(cali_sensor_buf[i], CALI_FUNC_CMD_ON))
                                {
                                    imu_cali_offset[i].name[0] = cali_flash_head[i][0];
                                    imu_cali_offset[i].name[1] = cali_flash_head[i][1];
                                    imu_cali_offset[i].name[2] = cali_flash_head[i][2];
                                    imu_cali_offset[i].cali_done = CALIED_FLAG;
                                    imu_cali_offset[i].cali_cmd = 0;
                                    cali_imu_data_write();
                                }
                        }
                }
        }
}
//从flash读取校准数据
static void cali_data_read(void)
{
    uint8_t flash_read_buf[CALI_SENSOR_HEAD_LEGHT * 4];
    uint8_t i = 0;
    uint16_t offset = 0;
    for (i = 0; i < CALI_LENGHT; i++)
        {
            //在闪存中读取数据
            cali_flash_read(FLASH_USER_ADDR + offset,imu_cali_offset[i].flash_buf, imu_cali_offset[i].flash_len);

            offset += imu_cali_offset[i].flash_len * 4;

            //读名字和校验标志
            cali_flash_read(FLASH_USER_ADDR + offset, (uint32_t *)flash_read_buf,CALI_SENSOR_HEAD_LEGHT);

            imu_cali_offset[i].name[0] = flash_read_buf[0];
            imu_cali_offset[i].name[1] = flash_read_buf[1];
            imu_cali_offset[i].name[2] = flash_read_buf[2];
            imu_cali_offset[i].cali_done = flash_read_buf[3];
            offset += CALI_SENSOR_HEAD_LEGHT * 4;
            if (imu_cali_offset[i].cali_done != CALIED_FLAG && imu_cali_offset[i].cali_hook != NULL)
                {
                    imu_cali_offset[i].cali_cmd = 1;
                }
        }
}
//往flash写入校准数据
static void cali_imu_data_write(void)
{
    uint8_t i = 0;
    uint16_t offset = 0;
    for (i = 0; i < CALI_LENGHT; i++)
        {
            //复制设备校准数据的数据
            memcpy((void *)(flash_write_buf + offset),(void *)imu_cali_offset[i].flash_buf,imu_cali_offset[i].flash_len * 4);
            offset += imu_cali_offset[i].flash_len * 4;

            //复制设备的名称和“CALI_FLAG”
            memcpy((void *)(flash_write_buf + offset),(void *)imu_cali_offset[i].name,CALI_SENSOR_HEAD_LEGHT * 4);
            offset += CALI_SENSOR_HEAD_LEGHT * 4;
        }
    //擦除页面
    cali_flash_erase(FLASH_USER_ADDR,1);
    //写入数据
    cali_flash_write(FLASH_USER_ADDR, (uint32_t *)flash_write_buf, (FLASH_WRITE_BUF_LENGHT + 3) / 4);
}


//GYRO校准
static bool_t cali_gyro_hook(uint32_t *cali, bool_t cmd)
{
    static uint16_t count_time = 0;
    imu_gyro_cali_t *local_cali = (imu_gyro_cali_t *)cali;
    if (cmd == CALI_FUNC_CMD_INIT)//如果已经设置过校准
        {
            gyro_set_cali(local_cali->gyro_scale,local_cali->off_gyro_set);
        }
    else if(cmd == CALI_FUNC_CMD_ON)//开启校准
        {
            gyro_cali_fun(local_cali->gyro_scale,local_cali->off_gyro_set,&count_time);
            if (count_time > GYRO_CALIBRATE_TIME)
                {
                    count_time = 0;
                    return 1;
                }
            else
                {
                    return 0;
                }
        }
    return 0;
}
//ACC校准
static bool_t cali_accel_hook(uint32_t *cali, bool_t cmd)
{
    return 0;
    imu_accel_cali_t *local_cali = (imu_accel_cali_t *)cali;
}
//MAG校准
static bool_t cali_mag_hook(uint32_t *cali, bool_t cmd)
{
    return 0;
    imu_mag_cali_t *local_cali = (imu_mag_cali_t *)cali;
}
void cali_cmd(uint8_t i,uint8_t cmd)
{
	imu_cali_offset[i].cali_cmd = cmd;
}
