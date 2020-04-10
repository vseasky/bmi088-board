/*
* Brian R Taylor
* brian.taylor@bolderflight.com
* 
* Copyright (c) 2018 Bolder Flight Systems
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
* and associated documentation files (the "Software"), to deal in the Software without restriction, 
* including without limitation the rights to use, copy, modify, merge, publish, distribute, 
* sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is 
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all copies or 
* substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
* BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef BMI088_h
#define BMI088_h

#include "Arduino.h"
#include "Wire.h"  // I2C library
#include "SPI.h"   // SPI library

class Bmi088Accel {
  public:
    enum Range {
      RANGE_3G = 0x00,
      RANGE_6G = 0x01,
      RANGE_12G = 0x02,
      RANGE_24G = 0x03
    };
    enum Odr {
      ODR_1600HZ_BW_280HZ,
      ODR_1600HZ_BW_234HZ,
      ODR_1600HZ_BW_145HZ,
      ODR_800HZ_BW_230HZ,
      ODR_800HZ_BW_140HZ,
      ODR_800HZ_BW_80HZ,
      ODR_400HZ_BW_145HZ,
      ODR_400HZ_BW_75HZ,
      ODR_400HZ_BW_40HZ,
      ODR_200HZ_BW_80HZ,
      ODR_200HZ_BW_38HZ,
      ODR_200HZ_BW_20HZ,
      ODR_100HZ_BW_40HZ,
      ODR_100HZ_BW_19HZ,
      ODR_100HZ_BW_10HZ,
      ODR_50HZ_BW_20HZ,
      ODR_50HZ_BW_9HZ,
      ODR_50HZ_BW_5HZ,
      ODR_25HZ_BW_10HZ,
      ODR_25HZ_BW_5HZ,
      ODR_25HZ_BW_3HZ,
      ODR_12_5HZ_BW_5HZ,
      ODR_12_5HZ_BW_2HZ,
      ODR_12_5HZ_BW_1HZ
    };
    enum PinMode {
      PUSH_PULL,
      OPEN_DRAIN
    };
    enum PinLevel {
      ACTIVE_HIGH,
      ACTIVE_LOW
    };
    Bmi088Accel(TwoWire &bus,uint8_t address);
    Bmi088Accel(SPIClass &bus,uint8_t csPin);
    int begin();
    bool setOdr(Odr odr);
    bool setRange(Range range);
    bool pinModeInt1(PinMode mode, PinLevel level);
    bool pinModeInt2(PinMode mode, PinLevel level);
    bool mapDrdyInt1(bool enable);
    bool mapDrdyInt2(bool enable);
    bool getDrdyStatus();
    void readSensor();
    float getAccelX_mss();
    float getAccelY_mss();
    float getAccelZ_mss();
    float getTemperature_C();
    uint64_t getTime_ps();
    void estimateBias();
  private:
    // allow class Bmi088 access to private members 
    friend class Bmi088;
    // int pin input or output selection
    enum PinIO {
      PIN_INPUT,
      PIN_OUTPUT
    };
    // i2c
    uint8_t _address;
    TwoWire *_i2c;
    const uint32_t _i2cRate = 400000; // 400 kHz
    // spi
    uint8_t _csPin;
    SPIClass *_spi;
    bool _useSPI;
    const uint8_t SPI_READ = 0x80;
    const uint32_t SPI_CLOCK = 10000000; // 10 MHz
    // buffer for reading from sensor
    uint8_t _buffer[9];
    // constants
    static const uint8_t ACC_CHIP_ID = 0x1E;
    static const uint8_t ACC_RESET_CMD = 0xB6;
    static const uint8_t ACC_ENABLE_CMD = 0x04;
    static const uint8_t ACC_DISABLE_CMD = 0x00;
    static const uint8_t ACC_SUSPEND_MODE_CMD = 0x03;
    static const uint8_t ACC_ACTIVE_MODE_CMD = 0x00;
    static const uint8_t ACC_INT_INPUT = 0x11;
    static const uint8_t ACC_INT_OUTPUT = 0x08;
    static const uint8_t ACC_INT_OPENDRAIN = 0x04;
    static const uint8_t ACC_INT_PUSHPULL = 0x00;
    static const uint8_t ACC_INT_LVL_HIGH = 0x02;
    static const uint8_t ACC_INT_LVL_LOW = 0x00;
    static const uint8_t ACC_POS_SELF_TEST = 0x0D;
    static const uint8_t ACC_NEG_SELF_TEST = 0x09;
    static const uint8_t ACC_DIS_SELF_TEST = 0x00;
    // registers
    static const uint8_t ACC_CHIP_ID_ADDR = 0x00;
    static const uint8_t ACC_CHIP_ID_MASK = 0xFF;
    static const uint8_t ACC_CHIP_ID_POS = 0;
    static const uint8_t ACC_FATAL_ERR_ADDR = 0x02;
    static const uint8_t ACC_FATAL_ERR_MASK = 0x01;
    static const uint8_t ACC_FATAL_ERR_POS = 0;
    static const uint8_t ACC_ERR_CODE_ADDR = 0x02;
    static const uint8_t ACC_ERR_CODE_MASK = 0x1C;
    static const uint8_t ACC_ERR_CODE_POS = 2;
    static const uint8_t ACC_DRDY_ADDR = 0x03;
    static const uint8_t ACC_DRDY_MASK = 0x80;
    static const uint8_t ACC_DRDY_POS = 7;
    static const uint8_t ACC_ODR_ADDR = 0x40;
    static const uint8_t ACC_ODR_MASK = 0xFF;
    static const uint8_t ACC_ODR_POS = 0;
    static const uint8_t ACC_RANGE_ADDR = 0x41;
    static const uint8_t ACC_RANGE_MASK = 0x03;
    static const uint8_t ACC_RANGE_POS = 0;
    static const uint8_t ACC_INT1_IO_CTRL_ADDR = 0x53;
    static const uint8_t ACC_INT1_IO_CTRL_MASK = 0x1F;
    static const uint8_t ACC_INT1_IO_CTRL_POS = 0;
    static const uint8_t ACC_INT2_IO_CTRL_ADDR = 0x54;
    static const uint8_t ACC_INT2_IO_CTRL_MASK = 0x1F;
    static const uint8_t ACC_INT2_IO_CTRL_POS = 0;
    static const uint8_t ACC_INT1_DRDY_ADDR = 0x58;
    static const uint8_t ACC_INT1_DRDY_MASK = 0x04;
    static const uint8_t ACC_INT1_DRDY_POS = 2;
    static const uint8_t ACC_INT2_DRDY_ADDR = 0x58;
    static const uint8_t ACC_INT2_DRDY_MASK = 0x40;
    static const uint8_t ACC_INT2_DRDY_POS = 6;
    static const uint8_t ACC_SELF_TEST_ADDR = 0x6D;
    static const uint8_t ACC_SELF_TEST_MASK = 0xFF;
    static const uint8_t ACC_SELF_TEST_POS = 0;
    static const uint8_t ACC_PWR_CONF_ADDR = 0x7C;
    static const uint8_t ACC_PWR_CONF_MASK = 0xFF;
    static const uint8_t ACC_PWR_CONF_POS = 0;
    static const uint8_t ACC_PWR_CNTRL_ADDR = 0x7D;
    static const uint8_t ACC_PWR_CNTRL_MASK = 0xFF;
    static const uint8_t ACC_PWR_CNTRL_POS = 0;
    static const uint8_t ACC_SOFT_RESET_ADDR = 0x7E;
    static const uint8_t ACC_SOFT_RESET_MASK = 0xFF;
    static const uint8_t ACC_SOFT_RESET_POS = 0;
    static const uint8_t ACC_ACCEL_DATA_ADDR = 0x12;
    static const uint8_t ACC_TEMP_DATA_ADDR = 0x22;
    // transformation from sensor frame to right hand coordinate system
    const int16_t tX[3] = {1, 0, 0};
    const int16_t tY[3] = {0, -1, 0};
    const int16_t tZ[3] = {0, 0, -1};
    // convert G to m/s/s
    const float G = 9.807f;
    // accel full scale range
    float accel_range_mss;
    // accel data
    float accel_mss[3];
    // temperature data
    float temp_c;
    // sensor time
    uint32_t current_time_counter, prev_time_counter = 0;
    uint64_t time_counter;
    // interrupt pin setup
    bool pinModeInt1(PinIO io, PinMode mode, PinLevel level);
    bool pinModeInt2(PinIO io, PinMode mode, PinLevel level);
    // self test
    bool selfTest();
    // power and mode settings
    bool setMode(bool active);
    bool setPower(bool enable);
    // command soft reset
    void softReset();
    // error checking
    bool isConfigErr();
    bool isFatalErr();
    // check id
    bool isCorrectId();
    // write / read registers
    void writeRegister(uint8_t subAddress, uint8_t data);
    void writeRegisters(uint8_t subAddress, uint8_t count, const uint8_t* data);
    void readRegisters(uint8_t subAddress, uint8_t count, uint8_t* dest);
};

class Bmi088Gyro {
  public:
    enum Range {
      RANGE_2000DPS = 0x00,
      RANGE_1000DPS = 0x01,
      RANGE_500DPS = 0x02,
      RANGE_250DPS = 0x03,
      RANGE_125DPS = 0x04
    };
    enum Odr {
      ODR_2000HZ_BW_532HZ = 0x80,
      ODR_2000HZ_BW_230HZ = 0x81,
      ODR_1000HZ_BW_116HZ = 0x82,
      ODR_400HZ_BW_47HZ = 0x83,
      ODR_200HZ_BW_23HZ = 0x84,
      ODR_100HZ_BW_12HZ = 0x85,
      ODR_200HZ_BW_64HZ = 0x86,
      ODR_100HZ_BW_32HZ = 0x87
    };
    enum PinMode {
      PUSH_PULL,
      OPEN_DRAIN
    };
    enum PinLevel {
      ACTIVE_HIGH,
      ACTIVE_LOW
    };
    Bmi088Gyro(TwoWire &bus,uint8_t address);
    Bmi088Gyro(SPIClass &bus,uint8_t csPin);
    int begin();
    bool setOdr(Odr odr);
    bool setRange(Range range);
    bool pinModeInt3(PinMode mode, PinLevel level);
    bool pinModeInt4(PinMode mode, PinLevel level);
    bool mapDrdyInt3(bool enable);
    bool mapDrdyInt4(bool enable);
    bool getDrdyStatus();
    void readSensor();
    float getGyroX_rads();
    float getGyroY_rads();
    float getGyroZ_rads();
  private:
    // available power settings
    enum PowerMode {
      PWR_NORMAL = 0x00,
      PWR_SUSPEND = 0x80,
      PWR_DEEP_SUSPEND = 0x20
    };
    // i2c
    uint8_t _address;
    TwoWire *_i2c;
    const uint32_t _i2cRate = 400000; // 400 kHz
    // spi
    uint8_t _csPin;
    SPIClass *_spi;
    bool _useSPI;
    const uint8_t SPI_READ = 0x80;
    const uint32_t SPI_CLOCK = 10000000; // 10 MHz
    // buffer for reading from sensor
    uint8_t _buffer[9];
    // constants
    static const uint8_t GYRO_CHIP_ID = 0x0F;
    static const uint8_t GYRO_RESET_CMD = 0xB6;
    static const uint8_t GYRO_ENABLE_DRDY_INT = 0x80;
    static const uint8_t GYRO_DIS_DRDY_INT = 0x00;
    static const uint8_t GYRO_INT_OPENDRAIN = 0x02;
    static const uint8_t GYRO_INT_PUSHPULL = 0x00;
    static const uint8_t GYRO_INT_LVL_HIGH = 0x01;
    static const uint8_t GYRO_INT_LVL_LOW = 0x00;
    // registers
    static const uint8_t GYRO_CHIP_ID_ADDR = 0x00;
    static const uint8_t GYRO_CHIP_ID_MASK = 0xFF;
    static const uint8_t GYRO_CHIP_ID_POS = 0;
    static const uint8_t GYRO_DRDY_ADDR = 0x0A;
    static const uint8_t GYRO_DRDY_MASK = 0x80;
    static const uint8_t GYRO_DRDY_POS = 7;
    static const uint8_t GYRO_RANGE_ADDR = 0x0F;
    static const uint8_t GYRO_RANGE_MASK = 0xFF;
    static const uint8_t GYRO_RANGE_POS = 0;
    static const uint8_t GYRO_ODR_ADDR = 0x10;
    static const uint8_t GYRO_ODR_MASK = 0xFF;
    static const uint8_t GYRO_ODR_POS = 0;
    static const uint8_t GYRO_SOFT_RESET_ADDR = 0x14;
    static const uint8_t GYRO_SOFT_RESET_MASK = 0xFF;
    static const uint8_t GYRO_SOFT_RESET_POS = 0;
    static const uint8_t GYRO_INT_CNTRL_ADDR = 0x15;
    static const uint8_t GYRO_INT_CNTRL_MASK = 0xFF;
    static const uint8_t GYRO_INT_CNTRL_POS = 0;
    static const uint8_t GYRO_INT3_IO_CTRL_ADDR = 0x16;
    static const uint8_t GYRO_INT3_IO_CTRL_MASK = 0x03;
    static const uint8_t GYRO_INT3_IO_CTRL_POS = 0;
    static const uint8_t GYRO_INT4_IO_CTRL_ADDR = 0x16;
    static const uint8_t GYRO_INT4_IO_CTRL_MASK = 0x0C;
    static const uint8_t GYRO_INT4_IO_CTRL_POS = 2;
    static const uint8_t GYRO_INT3_DRDY_ADDR = 0x18;
    static const uint8_t GYRO_INT3_DRDY_MASK = 0x01;
    static const uint8_t GYRO_INT3_DRDY_POS = 0;
    static const uint8_t GYRO_INT4_DRDY_ADDR = 0x18;
    static const uint8_t GYRO_INT4_DRDY_MASK = 0x80;
    static const uint8_t GYRO_INT4_DRDY_POS = 7;
    static const uint8_t GYRO_DATA_ADDR = 0x02;
    // transformation from sensor frame to right hand coordinate system
    const int16_t tX[3] = {1, 0, 0};
    const int16_t tY[3] = {0, -1, 0};
    const int16_t tZ[3] = {0, 0, -1};
    // convert deg/s to rad/s
    const float D2R = M_PI / 180.0f;
    // gyro full scale range
    float gyro_range_rads;
    // gyro data
    float gyro_rads[3];
    // self test
    bool selfTest();
    // enable data read interrupt
    bool setDrdyInt(bool enable);
    // command soft reset
    void softReset();
    // power settings
    bool setPower(PowerMode mode);
    // check id
    bool isCorrectId();
    // write / read registers
    void writeRegister(uint8_t subAddress, uint8_t data);
    void readRegisters(uint8_t subAddress, uint8_t count, uint8_t* dest);
};

class Bmi088 {
  public:
    enum AccelRange {
      ACCEL_RANGE_3G,
      ACCEL_RANGE_6G,
      ACCEL_RANGE_12G,
      ACCEL_RANGE_24G
    };
    enum GyroRange {
      GYRO_RANGE_2000DPS,
      GYRO_RANGE_1000DPS,
      GYRO_RANGE_500DPS,
      GYRO_RANGE_250DPS,
      GYRO_RANGE_125DPS
    };
    enum Odr {
      ODR_2000HZ,
      ODR_1000HZ,
      ODR_400HZ
    };
    enum DrdyPin {
      PIN_1,
      PIN_2
    };
    enum SyncPin {
      PIN_3,
      PIN_4
    };
    enum PinMode {
      PUSH_PULL,
      OPEN_DRAIN
    };
    enum PinLevel {
      ACTIVE_HIGH,
      ACTIVE_LOW
    };
    Bmi088(TwoWire &bus,uint8_t accel_addr,uint8_t gyro_addr);
    Bmi088(SPIClass &bus,uint8_t accel_cs,uint8_t gyro_cs);
    int begin();
    bool setOdr(Odr odr);
    bool setRange(AccelRange accel_range,GyroRange gyro_range);
    bool mapDrdy(DrdyPin pin);
    bool mapSync(SyncPin pin);    
    bool pinModeDrdy(PinMode mode, PinLevel level);
    void readSensor();
    float getAccelX_mss();
    float getAccelY_mss();
    float getAccelZ_mss();
    float getTemperature_C();
    uint64_t getTime_ps();
    float getGyroX_rads();
    float getGyroY_rads();
    float getGyroZ_rads();  
  private:
    Bmi088Accel *accel;
    Bmi088Gyro *gyro;
    uint8_t drdy_pin;
    // constants
    static const uint8_t ACC_DISABLE = 0;
    static const uint8_t ACC_ENABLE = 1;
    static const uint8_t ACC_DATA_SYNC_LEN = 1;
    static const uint16_t ACC_DATA_SYNC_MODE_MASK = 0x0003;
    static const uint16_t ACC_DATA_SYNC_MODE_OFF = 0x00;
    static const uint16_t ACC_DATA_SYNC_MODE_400HZ = 0x01;
    static const uint16_t ACC_DATA_SYNC_MODE_1000HZ = 0x02;
    static const uint16_t ACC_DATA_SYNC_MODE_2000HZ = 0x03;
    static const uint8_t ACC_INTA_DISABLE = 0x00;
    static const uint8_t ACC_INTA_ENABLE  = 0x01;
    static const uint8_t ACC_INTB_DISABLE = 0x00;
    static const uint8_t ACC_INTB_ENABLE  = 0x02;
    // registers
    static const uint8_t ACC_INIT_CTRL_ADDR = 0x59;
    static const uint8_t ACC_FEATURE_LSB_ADDR = 0x5B;
    static const uint8_t ACC_FEATURE_MSB_ADDR = 0x5C;
    static const uint8_t ACC_FEATURE_CFG_ADDR = 0x5E;
    static const uint8_t ACC_INTERNAL_STATUS_ADDR = 0x2A;
    static const uint8_t ACC_DATA_SYNC_ADDR = 0x02;
    static const uint8_t ACC_INT1_MAP_ADDR = 0x56;
    static const uint8_t ACC_INT2_MAP_ADDR = 0x57;
    bool writeFeatureConfig();  
    void updateFeatureConfig(uint8_t addr, uint8_t count, const uint16_t *data);    
};

#endif
