#ifndef _BSP_MUSIC_H
#define _BSP_MUSIC_H

#include "struct_typedef.h"
#include "FreeRtos.h"

#define BUZZER_HARDWARE_MAX_FREQ 7000000

void music_task_creat(void);
void musicPlay_task(void const *pvParameters);

#endif
