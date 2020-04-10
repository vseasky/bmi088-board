#include "bsp_buzzer.h"
#include "tim.h"

//84MHz/(11+1)=7MHz    
void buzzer_on(uint16_t per, uint16_t pwm)
{
    TIM4->ARR = per;
	TIM4->CCR3 = pwm;
}
void buzzer_off(void)
{
    TIM4->CCR3 = 0;
}

