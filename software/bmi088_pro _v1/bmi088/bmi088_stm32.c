#include "bmi08x.h"
#include "bmi088.h"
#include "bmi088_stm32.h"
#include "stm32f4xx_hal.h"


extern SPI_HandleTypeDef hspi1;
int8_t stm32_spi_write(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t reg_addr, uint8_t *data, uint16_t len)
{
	reg_addr &= 0x7f;
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, &reg_addr, 1, 50);
	while(HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_BUSY);
	HAL_SPI_Transmit(&hspi1, data, len, 50);
	while(HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_BUSY);
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
	return 0;
}

int8_t stm32_spi_read(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin, uint8_t reg_addr, uint8_t *data, uint16_t len)
{
	reg_addr |= 0x80;
	HAL_GPIO_WritePin(GPIOx,GPIO_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, &reg_addr, 1, 50);
	HAL_SPI_Receive(&hspi1, data, len, 50);
	HAL_GPIO_WritePin(GPIOx,GPIO_Pin,GPIO_PIN_SET);
	return 0;
}

