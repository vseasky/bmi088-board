# BMI088-STM32
STM32 with BMI088 demo

<img src="https://github.com/bao-eng/BMI088-STM32/blob/master/bmi088_stm32.png" alt="STM32 with BMI088" width="500">

# Description
BMI088 in synchronized mode with 2kHz ODR for NUCLEO-F767ZI board.
Sensor connected to Zio headers according to https://github.com/BoschSensortec/BMI08x-Sensor-API/blob/master/DataSync.md

<img src="https://github.com/bao-eng/BMI088-STM32/blob/master/bmi088_stm32_sch.png" alt="STM32 with BMI088" width="500">
STM32-specific SPI functions(bmi088_stm32.c) taken from this post: https://github.com/BoschSensortec/BMI08x-Sensor-API/issues/3#issue-352179010

Project created in System Workbench(SW4STM32).

# Usage
Device will blink LD3[RED] every 1000 host data ready interrupt.
LD1[GREEN] will turn on if x-axis accelearation near to zero.
LD2[BLUE] will turn on if y-axis accelearation near to zero.
