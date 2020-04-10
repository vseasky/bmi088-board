#ifndef _BSP_KEY_H
#define _BSP_KEY_H
#include "stm32f4xx.h"
#include "struct_typedef.h"
#include "main.h"


//#define TIME_P_TIME 20	//单位ms
//#define TIME_L_TIME 2500	//单位ms
#define TIME_CYCLE  5	    //单位ms
#define TIME_P_COUNT 4      //TIME_P_TIME/TIME_CYCLE
#define TIME_L_COUNT 400	//TIME_L_TIME/TIME_CYCLE

#define KEY0 (KEY_GPIO_Port->IDR&KEY_Pin)
#define KEY0_PRESS 1


#define PRESS_LEVEL 0 //定义按键按下时的电平
typedef enum
{
	KEY_UPSPRING,    //按键弹起 
	KEY_PRESS,		 //按键按下
	KEY_PRESS_L_TIME,//按键长按
}key_states;

typedef struct
{
	key_states states;	//按键状态
	uint16_t time_count;		//按键按下时间
}key_time_info;

typedef struct
{	
	key_time_info key_info;
	uint8_t res_count;
}mx_key_info;


const mx_key_info* get_key_info_pointer(void);
void mx_key_init(void);
uint8_t key_scan(void);
void key_task_creat(void);
void key_scan_task(void const *pvParameters);
extern mx_key_info key_info_status;

#endif
