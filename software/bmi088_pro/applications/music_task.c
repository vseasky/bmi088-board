#include "music_task.h"

#include "bsp_buzzer.h"
#include "cmsis_os.h"
#include "mx_music_config.h"

osThreadId musicTaskHandle;
void music_task_creat(void)
{
    osThreadDef(musicTask,musicPlay_task,osPriorityRealtime,0,128);
    musicTaskHandle = osThreadCreate(osThread(musicTask), NULL);
}
//蜂鸣器发出声音
//useFreq即发声频率
void music_update(unsigned short useFreq)   //useFreq是发声频率，即真实世界一个音调的频率。
{
    uint32_t period;
    if((useFreq<=BUZZER_HARDWARE_MAX_FREQ/65536UL)||(useFreq>20000))
        {
            buzzer_off();
        }
    else
        {
            period=(BUZZER_HARDWARE_MAX_FREQ/useFreq)-1;//系统时钟除以现实世界频率，装入ARR
            buzzer_on(period,period/10000);
        }
}
//Play Music
void musicPlay_task(void const *pvParameters)
{
    static uint16_t i=0;
    while(1)
        {
            if(i<Always_with_me_length)
                {
                    if(Always_with_me[i].mTime>0)
                        {
                            music_update(Always_with_me[i].mName);
                            osDelay(Always_with_me[i].mTime);
                            i++;
                        }
                }
            else if(i>=Always_with_me_length)
                {
                    osDelay(500);
                    i=0;
                }
        }
}
