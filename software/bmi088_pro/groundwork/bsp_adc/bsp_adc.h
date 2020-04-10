#ifndef BSP_ADC_H
#define BSP_ADC_H
#include "struct_typedef.h"
void init_vrefint_reciprocal(void);
fp32 get_temprate(void);
fp32 get_battery_voltage(void);
#endif
