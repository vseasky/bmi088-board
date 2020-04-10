/*
	

*/
#include "bmi08x_defs.h"

#include "main.h"

int8_t stm32_spi_write(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t reg_addr, uint8_t *data, uint16_t len);

int8_t stm32_spi_read(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin, uint8_t reg_addr, uint8_t *data, uint16_t len);

