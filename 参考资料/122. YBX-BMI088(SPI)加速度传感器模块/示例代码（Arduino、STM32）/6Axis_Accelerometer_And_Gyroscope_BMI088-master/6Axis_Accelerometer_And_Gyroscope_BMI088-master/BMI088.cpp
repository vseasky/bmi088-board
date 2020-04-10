/*    
 * A library for Grove - 6-Axis Accelerometer&Gyroscope（BMI088）
 *   
 * Copyright (c) 2018 seeed technology co., ltd.  
 * Author      : Wayen Weng  
 * Create Time : June 2018
 * Change Log  : 
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "BMI088.h"

BMI088::BMI088(void)
{
    devAddrAcc = BMI088_ACC_ADDRESS;
    devAddrGyro = BMI088_GYRO_ADDRESS;
}

void BMI088::initialize(void)
{
    setAccScaleRange(RANGE_6G);
    setAccOutputDataRate(ODR_100);
    setAccPoweMode(ACC_ACTIVE);
    
    setGyroScaleRange(RANGE_2000);
    setGyroOutputDataRate(ODR_2000_BW_532);
    setGyroPoweMode(GYRO_NORMAL);
}

bool BMI088::isConnection(void)
{
    return ((getAccID() == 0x1E) && (getGyroID() == 0x0F));
}

void BMI088::resetAcc(void)
{
    write8(ACC, BMI088_ACC_SOFT_RESET, 0xB6);
}

void BMI088::resetGyro(void)
{
    write8(GYRO, BMI088_GYRO_SOFT_RESET, 0xB6);
}

uint8_t BMI088::getAccID(void)
{
    return read8(ACC, BMI088_GYRO_CHIP_ID);
}

uint8_t BMI088::getGyroID(void)
{
    return read8(GYRO, BMI088_GYRO_CHIP_ID);
}

void BMI088::setAccPoweMode(acc_power_type_t mode)
{
    if(mode == ACC_ACTIVE)
    {
        write8(ACC, BMI088_ACC_PWR_CTRl, 0x04);
        write8(ACC, BMI088_ACC_PWR_CONF, 0x00);
    }
    else if(mode == ACC_SUSPEND)
    {
        write8(ACC, BMI088_ACC_PWR_CONF, 0x03);
        write8(ACC, BMI088_ACC_PWR_CTRl, 0x00);
    }
}

void BMI088::setGyroPoweMode(gyro_power_type_t mode)
{
    if(mode == GYRO_NORMAL)
    {
        write8(GYRO, BMI088_GYRO_LPM_1, (uint8_t)GYRO_NORMAL);
    }
    else if(mode == GYRO_SUSPEND)
    {
        write8(GYRO, BMI088_GYRO_LPM_1, (uint8_t)GYRO_SUSPEND);
    }
    else if(mode == GYRO_DEEP_SUSPEND)
    {
        write8(GYRO, BMI088_GYRO_LPM_1, (uint8_t)GYRO_DEEP_SUSPEND);
    }
}

void BMI088::setAccScaleRange(acc_scale_type_t range)
{
    if(range == RANGE_3G) accRange = 3000;
    else if(range == RANGE_6G) accRange = 6000;
    else if(range == RANGE_12G) accRange = 12000;
    else if(range == RANGE_24G) accRange = 24000;
    
    write8(ACC, BMI088_ACC_RANGE, (uint8_t)range);
}

void BMI088::setAccOutputDataRate(acc_odr_type_t odr)
{
    uint8_t data = 0;
    
    data = read8(ACC, BMI088_ACC_CONF);
    data = data & 0xf0;
    data = data | (uint8_t)odr;
    
    write8(ACC, BMI088_ACC_CONF, data);
}

void BMI088::setGyroScaleRange(gyro_scale_type_t range)
{
    if(range == RANGE_2000) gyroRange = 2000;
    else if(range == RANGE_1000) gyroRange = 1000;
    else if(range == RANGE_500) gyroRange = 500;
    else if(range == RANGE_250) gyroRange = 250;
    else if(range == RANGE_125) gyroRange = 125;
    
    write8(GYRO, BMI088_GYRO_RANGE, (uint8_t)range);
}

void BMI088::setGyroOutputDataRate(gyro_odr_type_t odr)
{
    write8(GYRO, BMI088_GYRO_BAND_WIDTH, (uint8_t)odr);
}

void BMI088::getAcceleration(float* x, float* y, float* z)
{
    uint8_t buf[6] = {0};
    uint16_t ax = 0, ay = 0, az = 0;
    float value = 0;
    
    read(ACC, BMI088_ACC_X_LSB, buf, 6);
    
    ax = buf[0] | (buf[1] << 8);
    ay = buf[2] | (buf[3] << 8);
    az = buf[4] | (buf[5] << 8);
    
    value = (int16_t)ax;
    *x = accRange * value / 32768;
    
    value = (int16_t)ay;
    *y = accRange * value / 32768;
    
    value = (int16_t)az;
    *z = accRange * value / 32768;
}

float BMI088::getAccelerationX(void)
{
    uint16_t ax = 0;
    float value = 0;
    
    ax = read16(ACC, BMI088_ACC_X_LSB);
    
    value = (int16_t)ax;
    value = accRange * value / 32768;
    
    return value;
}

float BMI088::getAccelerationY(void)
{
    uint16_t ay = 0;
    float value = 0;
    
    ay = read16(ACC, BMI088_ACC_Y_LSB);
    
    value = (int16_t)ay;
    value = accRange * value / 32768;
    
    return value;
}

float BMI088::getAccelerationZ(void)
{
    uint16_t az = 0;
    float value = 0;
    
    az = read16(ACC, BMI088_ACC_Z_LSB);
    
    value = (int16_t)az;
    value = accRange * value / 32768;
    
    return value;
}

void BMI088::getGyroscope(float* x, float* y, float* z)
{
    uint8_t buf[6] = {0};
    uint16_t gx = 0, gy = 0, gz = 0;
    float value = 0;
    
    read(GYRO, BMI088_GYRO_RATE_X_LSB, buf, 6);
    
    gx = buf[0] | (buf[1] << 8);
    gy = buf[2] | (buf[3] << 8);
    gz = buf[4] | (buf[5] << 8);
    
    value = (int16_t)gx;
    *x = gyroRange * value / 32768;
    
    value = (int16_t)gy;
    *y = gyroRange * value / 32768;
    
    value = (int16_t)gz;
    *z = gyroRange * value / 32768;
}

float BMI088::getGyroscopeX(void)
{
    uint16_t gx = 0;
    float value = 0;
    
    gx = read16(GYRO, BMI088_GYRO_RATE_X_LSB);
    
    value = (int16_t)gx;
    value = gyroRange * value / 32768;
    
    return value;
}

float BMI088::getGyroscopeY(void)
{
    uint16_t gy = 0;
    float value = 0;
    
    gy = read16(GYRO, BMI088_GYRO_RATE_Y_LSB);
    
    value = (int16_t)gy;
    value = gyroRange * value / 32768;
    
    return value;
}

float BMI088::getGyroscopeZ(void)
{
    uint16_t gz = 0;
    float value = 0;
    
    gz = read16(GYRO, BMI088_GYRO_RATE_Z_LSB);
    
    value = (int16_t)gz;
    value = gyroRange * value / 32768;
    
    return value;
}

int16_t BMI088::getTemperature(void)
{
    uint16_t data = 0;
    
    data = read16Be(ACC, BMI088_ACC_TEMP_MSB);
    data = data >> 5;
    
    if(data > 1023) data = data - 2048;
    
    return (int16_t)(data / 8 + 23);
}

void BMI088::write8(device_type_t dev, uint8_t reg, uint8_t val)
{
    uint8_t addr = 0;
    
    if(dev) addr = devAddrGyro;
    else addr = devAddrAcc;
    
    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.write(val);
    Wire.endTransmission();
}

uint8_t BMI088::read8(device_type_t dev, uint8_t reg)
{
    uint8_t addr = 0, data = 0;
    
    if(dev) addr = devAddrGyro;
    else addr = devAddrAcc;

    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.endTransmission();

    Wire.requestFrom(addr, 1);
    while(Wire.available())
    {
        data = Wire.read();
    }
    
    return data;
}

uint16_t BMI088::read16(device_type_t dev, uint8_t reg)
{
    uint8_t addr = 0;
    uint16_t msb = 0, lsb = 0;
    
    if(dev) addr = devAddrGyro;
    else addr = devAddrAcc;

    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.endTransmission();
    
    Wire.requestFrom(addr, 2);
    while(Wire.available())
    {
        lsb = Wire.read();
        msb = Wire.read();
    }

    return (lsb | (msb << 8));
}

uint16_t BMI088::read16Be(device_type_t dev, uint8_t reg)
{
    uint8_t addr = 0;
    uint16_t msb = 0, lsb = 0;
    
    if(dev) addr = devAddrGyro;
    else addr = devAddrAcc;

    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.endTransmission();
    
    Wire.requestFrom(addr, 2);
    while(Wire.available())
    {
        msb = Wire.read();
        lsb = Wire.read();
    }

    return (lsb | (msb << 8));
}

uint32_t BMI088::read24(device_type_t dev, uint8_t reg)
{
    uint8_t addr = 0;
    uint32_t hsb = 0, msb = 0, lsb = 0;
    
    if(dev) addr = devAddrGyro;
    else addr = devAddrAcc;

    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.endTransmission();
    
    Wire.requestFrom(addr, 3);
    while(Wire.available())
    {
        lsb = Wire.read();
        msb = Wire.read();
        hsb = Wire.read();
    }

    return (lsb | (msb << 8) | (hsb << 16));
}

void BMI088::read(device_type_t dev, uint8_t reg, uint8_t *buf, uint16_t len)
{
    uint8_t addr = 0;
    
    if(dev) addr = devAddrGyro;
    else addr = devAddrAcc;
    
    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.endTransmission();
    
    Wire.requestFrom(addr, len);
    while(Wire.available())
    {
        for(uint16_t i = 0; i < len; i ++) buf[i] = Wire.read();
    }
}

BMI088 bmi088;
