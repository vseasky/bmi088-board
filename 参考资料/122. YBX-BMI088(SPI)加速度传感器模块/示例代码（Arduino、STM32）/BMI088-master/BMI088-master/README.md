# BMI088
Arduino library for communicating with the [BMI088](https://www.bosch-sensortec.com/bst/products/all_products/bmi088_1) six-axis Inertial Measurement Unit (IMU).

# Description
The Bosch Sensortec [BMI088](https://www.bosch-sensortec.com/bst/products/all_products/bmi088_1) is a high performance six-axis inertial measurement unit (IMU) featuring a high vibration robustness and specifically designed for use in drones and robotics. BMI088 is specifically designed to effectively suppress vibrations that could occur due to resonances on the pcb or the structure of the total system. Apart from high vibration robustness, the excellent temperature stability of BMI088 helps improve estimation filter performance and the IMU features an exceptionally wide 24G accelerometer range.

The BMI088 is a System in Package (SiP) combining an accelerometer and a gyro into a single package. The BMI088 supports I2C, up to 400 kHz, and SPI communication, up to 10 MHz. The accelerometer and gyro can be accessed individually, or their output data can be synchronized. The BMI088 also features programmable full scale ranges, output data rates, and interrupts.

# Usage
This library supports both I2C and SPI commmunication with the BMI088.

## Installation
Simply clone or download this library into your Arduino/libraries folder.

## Function Description
This library supports both I2C and SPI communication with the BMI088. Classes are available for working with the accelerometer, the gyro, and the synchronized output from both. 

## Bmi088Accel Class
This class is used for working with the BMI088 accelerometer. The *Bmi088Accel* object declaration is overloaded with different declarations for I2C and SPI communication. All other functions remain the same. 

### I2C Object Declaration

**Bmi088Accel(TwoWire &bus,uint8_t address)**
A Bmi088Accel object should be declared, specifying the I2C bus and the Bmi088Accel I2C address. The I2C address will be 0x18 if the SDO1 pin is grounded or 0x19 if the SDO1 pin is pulled high. For example, the following code declares a Bmi088Accel object called *accel* with a BMI088 sensor located on I2C bus 0 with a sensor address of 0x18 (SDO1 grounded).

```C++
Bmi088Accel accel(Wire,0x18);
```

### SPI Object Declaratioon

**Bmi088Accel(SPIClass &bus,uint8_t csPin)**
A Bmi088Accel object should be declared, specifying the SPI bus and the chip select pin used. Multiple BMI088 or other SPI objects could be used on the same SPI bus, each with their own chip select pin. The chip select pin can be any available digital pin. For example, the following code declares a Bmi088Accel object called *accel* with a BMI088 sensor located on SPI bus 0 with chip select pin 10.

```C++
Bmi088Accel accel(SPI,10);
```

### Common Setup Functions
The following functions are used to setup the BMI088 sensor. These should be called once before data collection, typically this is done in the Arduino *void setup()* function. The *begin* function should always be used. Optionally, *setOdr* can be used to setup the output data rate and digital low pass filtering, and *setRange* can be used to setup the full scale range of the sensor. *pinModeInt1* and *pinModeInt2* can be used to setup the pin 1 and pin 2 interrupt pins (push pull vs open drain, active high vs active low). *mapDrdyInt1* and *mapDrdyInt2* map the data ready interrupt to pin 1 and pin 2, respectively. If these optional functions are not used, default values for output data rate and digital low pass filtering are used instead and the data ready interrupt pins are not configured.

**int begin()**
This should be called in your setup function. It initializes communication with the BMI088 accelerometer and sets up the sensor for reading data. This function returns a positive value on a successful initialization and returns a negative value on an unsuccesful initialization. If unsuccessful, please check your wiring or try resetting power to the sensor. The following is an example of setting up the Bmi088Accel.

```C++
int status;
status = accel.begin();
```

**(optional) bool setOdr(Odr odr)**
The BMI088 features a programmable output data rate and digital low pass filtering. The following enumerated settings are supported:

| Output Data Rate | DLPF Bandwidth | Odr Name            |
| ---------------- | -------------- | ------------------- |
| 1600 Hz          | 280 Hz         | ODR_1600HZ_BW_280HZ | 
| 1600 Hz          | 234 Hz         | ODR_1600HZ_BW_234HZ |
| 1600 Hz          | 145 Hz         | ODR_1600HZ_BW_145HZ |
| 800 Hz           | 230 Hz         | ODR_800HZ_BW_230HZ  | 
| 800 Hz           | 140 Hz         | ODR_800HZ_BW_140HZ  |
| 800 Hz           | 80 Hz          | ODR_800HZ_BW_80HZ   |
| 400 Hz           | 145 Hz         | ODR_400HZ_BW_145HZ  | 
| 400 Hz           | 75 Hz          | ODR_400HZ_BW_75HZ   |
| 400 Hz           | 40 Hz          | ODR_400HZ_BW_40HZ   |
| 200 Hz           | 80 Hz          | ODR_200HZ_BW_80HZ   | 
| 200 Hz           | 38 Hz          | ODR_200HZ_BW_38HZ   |
| 200 Hz           | 20 Hz          | ODR_200HZ_BW_20HZ   |
| 100 Hz           | 40 Hz          | ODR_100HZ_BW_40HZ   | 
| 100 Hz           | 19 Hz          | ODR_100HZ_BW_19HZ   |
| 100 Hz           | 10 Hz          | ODR_100HZ_BW_10HZ   |
| 50 Hz            | 20 Hz          | ODR_50HZ_BW_20HZ    | 
| 50 Hz            | 9 Hz           | ODR_50HZ_BW_9HZ     |
| 50 Hz            | 5 Hz           | ODR_50HZ_BW_5HZ     |
| 25 Hz            | 10 Hz          | ODR_25HZ_BW_10HZ    | 
| 25 Hz            | 5 Hz           | ODR_25HZ_BW_5HZ     |
| 25 Hz            | 3 Hz           | ODR_25HZ_BW_3HZ     |
| 12.5 Hz          | 5 Hz           | ODR_12_5HZ_BW_5HZ   | 
| 12.5 Hz          | 2 Hz           | ODR_12_5HZ_BW_2HZ   |
| 12.5 Hz          | 1 Hz           | ODR_12_5HZ_BW_1HZ   |

Below is an example of setting the output data rate to 100 Hz and the bandwidth to 19 Hz.

```C++
bool status;
status = accel.setOdr(Bmi088Accel::ODR_100HZ_BW_19HZ);
```

**(optional) bool setRange(Range range)**
The BMI088 features a programmable full scale range. The following enumerated settings are supported:

| Full Scale Range | Range Name |
| ---------------- | ---------- |
| +/- 24 G         | RANGE_24G  |
| +/- 12 G         | RANGE_12G  |
| +/- 6 G          | RANGE_6G   |
| +/- 3 G          | RANGE_3G   |

Below is an example of setting the range to +/- 6 G.

```C++
bool status;
status = accel.setRange(Bmi088Accel::RANGE_6G);
```

**(optional) pinModeInt1(PinMode mode, PinLevel level)**
**(optional) pinModeInt2(PinMode mode, PinLevel level)**
The BMI088 accelerometer features two interrupt pins that the data ready interrupt can be mapped two. Each of these can be setup with respect to whether the output should be push-pull or open-drain and whether the pin should be active high or active low. 

| Pin Mode   | Name       |
| ---------- | ---------- |
| Push-Pull  | PUSH_PULL  |
| Open-Drain | OPEN_DRAIN |

| Active Level | Name        |
| ------------ | ----------- |
| High         | ACTIVE_HIGH |
| Low          | ACTIVE_LOW  |

Below is an example of setting the interrupt pin 1 to push-pull and active-high.

```C++
bool status;
status = accel.pinModeInt1(Bmi088Accel::PUSH_PULL,Bmi088Accel::ACTIVE_HIGH);
```

**(optional) mapDrdyInt1(bool enable)**
**(optional) mapDrdyInt2(bool enable)**
These functions setup whether the BMI088 accelerometer maps its data ready interrupt to pin 1 and/or pin 2. Set to true to map the data ready interrupt to the pin or false to disable it.

```C++
bool status;
status = accel.mapDrdyInt1(true);
```

### Common Data Collection Functions
The functions below are used to collect data from the BMI088 accelerometer. Data is returned scaled to engineering units and transformed to a right hand coordinate system with the Z axis positive down. Please see the [Coordinate System](#sensor-orientation) section for a depiction. In addition to acceleration, the BMI088 accelerometer also provides the BMI088 die temperature and a sensor time value. Accelerometer data is returned in units of m/s/s, temperature data in units of degrees Celsius, and time data in pico-seconds. *readSensor* is used to read the sensor and store the newest data in a buffer, it should be called every time you would like to retrieve data from the sensor. *getAccelX_mss*, *getAccelY_mss*, *getAccelZ_mss*, *getTemperature_C*, and *getTime_ps* return the accelerometer, temperature, and time values from that buffer. 

**bool readSensor()** reads the sensor and stores the newest data in a buffer, it should be called every time you would like to retrieve data from the sensor.

```C++
bool status;
status = accel.readSensor();
```

**float getAccelX_mss()** gets the accelerometer value from the data buffer in the X direction and returns it in units of m/s/s.

```C++
float ax;
ax = accel.getAccelX_mss();
```

**float getAccelY_mss()** gets the accelerometer value from the data buffer in the Y direction and returns it in units of m/s/s.

```C++
float ay;
ay = accel.getAccelY_mss();
```

**float getAccelZ_mss()** gets the accelerometer value from the data buffer in the Z direction and returns it in units of m/s/s.

```C++
float az;
az = accel.getAccelZ_mss();
```

**float getTemperature_C()** gets the die temperature value from the data buffer and returns it in units of C.

```C++
float t;
t = accel.getTemperature_C();
```

**uint64_t getTime_ps()** gets the sensor time value from the data buffer and returns it in units of ps.

```C++
uint_64t time_ps;
time_ps = accel.getTime_ps();
```

## Bmi088Gyro Class
This class is used for working with the BMI088 gyro. The *Bmi088Gyro* object declaration is overloaded with different declarations for I2C and SPI communication. All other functions remain the same. 

### I2C Object Declaration

**Bmi088Gyro(TwoWire &bus,uint8_t address)**
A Bmi088Gyro object should be declared, specifying the I2C bus and the Bmi088Gyro I2C address. The I2C address will be 0x68 if the SDO2 pin is grounded or 0x69 if the SDO2 pin is pulled high. For example, the following code declares a Bmi088Gyro object called *gyro* with a BMI088 sensor located on I2C bus 0 with a sensor address of 0x68 (SDO2 grounded).

```C++
Bmi088Gyro gyro(Wire,0x68);
```

### SPI Object Declaratioon

**Bmi088Gyro(SPIClass &bus,uint8_t csPin)**
A Bmi088Gyro object should be declared, specifying the SPI bus and the chip select pin used. Multiple BMI088 or other SPI objects could be used on the same SPI bus, each with their own chip select pin. The chip select pin can be any available digital pin. For example, the following code declares a Bmi088Gyro object called *gyro* with a BMI088 sensor located on SPI bus 0 with chip select pin 9.

```C++
Bmi088Gyro gyro(SPI,9);
```

### Common Setup Functions
The following functions are used to setup the BMI088 sensor. These should be called once before data collection, typically this is done in the Arduino *void setup()* function. The *begin* function should always be used. Optionally, *setOdr* can be used to setup the output data rate and digital low pass filtering, and *setRange* can be used to setup the full scale range of the sensor. *pinModeInt3* and *pinModeInt4* can be used to setup the pin 3 and pin 4 interrupt pins (push pull vs open drain, active high vs active low). *mapDrdyInt3* and *mapDrdyInt4* map the data ready interrupt to pin 3 and pin 4, respectively. If these optional functions are not used, default values for output data rate and digital low pass filtering are used instead and the data ready interrupt pins are not configured.

**int begin()**
This should be called in your setup function. It initializes communication with the BMI088 gyro and sets up the sensor for reading data. This function returns a positive value on a successful initialization and returns a negative value on an unsuccesful initialization. If unsuccessful, please check your wiring or try resetting power to the sensor. The following is an example of setting up the Bmi088Gyro.

```C++
int status;
status = gyro.begin();
```

**(optional) bool setOdr(Odr odr)**
The BMI088 features a programmable output data rate and digital low pass filtering. The following enumerated settings are supported:

| Output Data Rate | DLPF Bandwidth | Odr Name            |
| ---------------- | -------------- | ------------------- |
| 2000 Hz          | 532 Hz         | ODR_2000HZ_BW_532HZ | 
| 2000 Hz          | 230 Hz         | ODR_2000HZ_BW_230HZ |
| 1000 Hz          | 116 Hz         | ODR_1000HZ_BW_116HZ |
| 400 Hz           | 47 Hz          | ODR_400HZ_BW_47HZ   | 
| 200 Hz           | 64 Hz          | ODR_200HZ_BW_64HZ   |
| 200 Hz           | 23 Hz          | ODR_200HZ_BW_23HZ   |
| 100 Hz           | 32 Hz          | ODR_100HZ_BW_32H    | 
| 100 Hz           | 12 Hz          | ODR_100HZ_BW_12HZ   |

Below is an example of setting the output data rate to 100 Hz and the bandwidth to 12 Hz.

```C++
bool status;
status = gyro.setOdr(Bmi088Gyro::ODR_100HZ_BW_12HZ);
```

**(optional) bool setRange(Range range)**
The BMI088 features a programmable full scale range. The following enumerated settings are supported:

| Full Scale Range | Range Name     |
| ---------------- | -------------- |
| +/- 2000 deg/s   | RANGE_2000DPS  |
| +/- 1000 deg/s   | RANGE_1000DPS  |
| +/- 500 deg/s    | RANGE_500DPS   |
| +/- 250 deg/s    | RANGE_250DPS   |
| +/- 125 deg/s    | RANGE_125DPS   |

Below is an example of setting the range to +/- 500 deg/s.

```C++
bool status;
status = gyro.setRange(Bmi088Gyro::RANGE_500DPS);
```

**(optional) pinModeInt3(PinMode mode, PinLevel level)**
**(optional) pinModeInt4(PinMode mode, PinLevel level)**
The BMI088 gyro features two interrupt pins that the data ready interrupt can be mapped two. Each of these can be setup with respect to whether the output should be push-pull or open-drain and whether the pin should be active high or active low. 

| Pin Mode   | Name       |
| ---------- | ---------- |
| Push-Pull  | PUSH_PULL  |
| Open-Drain | OPEN_DRAIN |

| Active Level | Name        |
| ------------ | ----------- |
| High         | ACTIVE_HIGH |
| Low          | ACTIVE_LOW  |

Below is an example of setting the interrupt pin 3 to push-pull and active-high.

```C++
bool status;
status = gyro.pinModeInt3(Bmi088Gyro::PUSH_PULL,Bmi088Gyro::ACTIVE_HIGH);
```

**(optional) mapDrdyInt3(bool enable)**
**(optional) mapDrdyInt4(bool enable)**
These functions setup whether the BMI088 gyro maps its data ready interrupt to pin 3 and/or pin 4. Set to true to map the data ready interrupt to the pin or false to disable it.

```C++
bool status;
status = gyro.mapDrdyInt3(true);
```

### Common Data Collection Functions
The functions below are used to collect data from the BMI088 gyro. Data is returned scaled to engineering units and transformed to a right hand coordinate system with the Z axis positive down. Please see the [Coordinate System](#sensor-orientation) section for a depiction. Gyro data is returned in units of rad/s. *readSensor* is used to read the sensor and store the newest data in a buffer, it should be called every time you would like to retrieve data from the sensor. *getGyroX_rads*, *getGyroY_rads*, *getGyroZ_rads* return the gyro values from that buffer. 

**bool readSensor()** reads the sensor and stores the newest data in a buffer, it should be called every time you would like to retrieve data from the sensor.

```C++
bool status;
status = gyro.readSensor();
```

**float getGyroX_rads()** gets the gyroscope value from the data buffer in the X direction and returns it in units of rad/s.

```C++
float gx;
gx = gyro.getGyroX_rads();
```

**float getGyroY_rads()** gets the gyroscope value from the data buffer in the Y direction and returns it in units of rad/s.

```C++
float gy;
gy = gyro.getGyroY_rads();
```

**float getGyroZ_rads()** gets the gyroscope value from the data buffer in the Z direction and returns it in units of rad/s.

```C++
float gz;
gz = gyro.getGyroZ_rads();
```

## Bmi088 Class
This class is used for working with the synchronized output from the BMI088 sensor. In this mode, one of the gyro interrupts is tied into the accelerometer interrupt and logic on the accelerometer is used to interpolate and synchronize the data output. Only a subset of the individual output data rates and digital low pass filters from the accelerometer and gyro are supported in sync mode. The *Bmi088* object declaration is overloaded with different declarations for I2C and SPI communication. All other functions remain the same. 

### I2C Object Declaration

**Bmi088(TwoWire &bus,uint8_t accel_addr,uint8_t gyro_addr)**
A Bmi088 object should be declared, specifying the I2C bus and the I2C addresses of the accelerometer and gyro. The I2C address of the accelerometer will be 0x18 if the SDO1 pin is grounded or 0x19 if the SDO1 pin is pulled high. The I2C address will be 0x68 if the SDO2 pin is grounded or 0x69 if the SDO2 pin is pulled high. For example, the following code declares a Bmi088 object called *bmi* with a BMI088 sensor located on I2C bus 0 with an accelerometer address of 0x18 (SDO1 grounded) and a gyro address of 0x68 (SDO2 grounded);

```C++
Bmi088 bmi(Wire,0x18,0x68);
```

### SPI Object Declaratioon

**Bmi088(SPIClass &bus,uint8_t accel_cs,uint8_t gyro_cs)**
A Bmi088 object should be declared, specifying the SPI bus and the chip select pins of the accelerometer and gyro. Multiple BMI088 or other SPI objects could be used on the same SPI bus, each with their own chip select pin. The chip select pin can be any available digital pin. For example, the following code declares a Bmi088 object called *bmi* with a BMI088 sensor located on SPI bus 0 with the accelerometer on chip select pin 10 and gyro on chip select pin 9.

```C++
Bmi088 bmi(SPI,10,9);
```

### Common Setup Functions
The following functions are used to setup the BMI088 sensor. These should be called once before data collection, typically this is done in the Arduino *void setup()* function. The *begin* function should always be used. Optionally, *setOdr* can be used to setup the output data rate and digital low pass filtering, and *setRange* can be used to setup the full scale range of the sensor. *mapDrdy* sets the pin used for the synchronized data ready interrupt, the other accelerometer pin is then used for synchronization with the gyro. *mapSync* sets the gyro pin used for synchronization with the accelerometer. *pinModeDrdy* sets up the data ready interrupt pin (push-pull vs open-drain and active high vs active low). If these optional functions are not used, default values for output data rate and digital low pass filtering are used instead and accelerometer pin 1 and gyro pin 3 are used for synchronization with accelerometer pin 2 available as a synchronized data ready interrupt.

**int begin()**
This should be called in your setup function. It initializes communication with the BMI088 and sets up the sensor for reading data. This function returns a positive value on a successful initialization and returns a negative value on an unsuccesful initialization. If unsuccessful, please check your wiring or try resetting power to the sensor. The following is an example of setting up the Bmi088.

```C++
int status;
status = bmi.begin();
```

**(optional) bool setOdr(Odr odr)**
The BMI088 features a programmable output data rate and digital low pass filtering. The following enumerated settings are supported:

| Output Data Rate | Accel DLPF Bandwidth | Gyro DLPF Bandwidth | Odr Name            |
| ---------------- | -------------------- | ------------------- | ------------------- |
| 2000 Hz          | 280 Hz               | 230 Hz              | ODR_2000HZ          | 
| 1000 Hz          | 230 Hz               | 116 Hz              | ODR_1000HZ          | 
| 400 Hz           | 145 Hz               | 47 Hz               | ODR_400HZ           | 

Below is an example of setting the output data rate to 400 Hz.

```C++
bool status;
status = bmi.setOdr(Bmi088::ODR_400HZ);
```

**(optional) bool setRange(AccelRange accel_range,GyroRange gyro_range)**
The BMI088 features programmable accelerometer and gyro full scale ranges. The following enumerated settings are supported:

| Accel Full Scale Range | Range Name       |
| ----------------       | ----------       |
| +/- 24 G               | ACCEL_RANGE_24G  |
| +/- 12 G               | ACCEL_RANGE_12G  |
| +/- 6 G                | ACCEL_RANGE_6G   |
| +/- 3 G                | ACCEL_RANGE_3G   |

| Gyro Full Scale Range | Range Name          |
| ----------------      | --------------      |
| +/- 2000 deg/s        | GYRO_RANGE_2000DPS  |
| +/- 1000 deg/s        | GYRO_RANGE_1000DPS  |
| +/- 500 deg/s         | GYRO_RANGE_500DPS   |
| +/- 250 deg/s         | GYRO_RANGE_250DPS   |
| +/- 125 deg/s         | GYRO_RANGE_125DPS   |

Below is an example of setting the accelerometer range to +/- 6 G and the gyro range to +/- 500 deg/s.

```C++
bool status;
status = bmi.setRange(Bmi088::ACCEL_RANGE_6G,Bmi088::GYRO_RANGE_500DPS);
```

**(optional) bool mapDrdy(DrdyPin pin)**
Two pins are available for the synchronized data ready interrupt - pin 1 or pin 2. This function is used to select which pin to use, the unselected pin is automatically configured for use as a synchronization pin between the accelerometer and gyro.

| Pin Number | Name  |
| ---------- | ----- |
| 1          | PIN_1 |
| 2          | PIN_2 |

Below is an example of setting pin 2 for use as a synchronized data ready interrupt.

```C++
bool status;
status = bmi.mapDrdy(Bmi088::PIN_2);
```

**(optional) bool mapSync(SyncPin pin)**
This function is used to select which gyro pin is used as a synchronization pin between the accelerometer and gyo. Two pins are available - pin 3 or pin 4.

| Pin Number | Name  |
| ---------- | ----- |
| 3          | PIN_3 |
| 4          | PIN_4 |

Below is an example of setting pin 3 for use as a synchronization pin.

```C++
bool status;
status = bmi.mapSync(Bmi088::PIN_3);
```

**(optional) pinModeDrdy(PinMode mode, PinLevel level)**
The BMI088 synchronized data interrupt pin can be setup with respect to whether the output should be push-pull or open-drain and whether the pin should be active high or active low. 

| Pin Mode   | Name       |
| ---------- | ---------- |
| Push-Pull  | PUSH_PULL  |
| Open-Drain | OPEN_DRAIN |

| Active Level | Name        |
| ------------ | ----------- |
| High         | ACTIVE_HIGH |
| Low          | ACTIVE_LOW  |

Below is an example of setting the interrupt to push-pull and active-high.

```C++
bool status;
status = bmi.pinModeDrdy(Bmi088::PUSH_PULL,Bmi088::ACTIVE_HIGH);
```

### Common Data Collection Functions
The functions below are used to collect data from the BMI088. Data is returned scaled to engineering units and transformed to a right hand coordinate system with the Z axis positive down. Please see the [Coordinate System](#sensor-orientation) section for a depiction. Accelerometer data is returned in units of m/s/s, gyro data in units of rad/s, temperature data in units of degrees Celsius, and time data in pico-seconds. *readSensor* is used to read the sensor and store the newest data in a buffer, it should be called every time you would like to retrieve data from the sensor. *getAccelX_mss*, *getAccelY_mss*, *getAccelZ_mss*, *getGyroX_rads*, *getGyroY_rads*, *getGyroZ_rads*, *getTemperature_C*, and *getTime_ps* return the accelerometer, gyro, temperature, and time values from that buffer. 

**bool readSensor()** reads the sensor and stores the newest data in a buffer, it should be called every time you would like to retrieve data from the sensor.

```C++
bool status;
status = bmi.readSensor();
```

**float getAccelX_mss()** gets the accelerometer value from the data buffer in the X direction and returns it in units of m/s/s.

```C++
float ax;
ax = bmi.getAccelX_mss();
```

**float getAccelY_mss()** gets the accelerometer value from the data buffer in the Y direction and returns it in units of m/s/s.

```C++
float ay;
ay = bmi.getAccelY_mss();
```

**float getAccelZ_mss()** gets the accelerometer value from the data buffer in the Z direction and returns it in units of m/s/s.

```C++
float az;
az = bmi.getAccelZ_mss();
```

**float getGyroX_rads()** gets the gyroscope value from the data buffer in the X direction and returns it in units of rad/s.

```C++
float gx;
gx = bmi.getGyroX_rads();
```

**float getGyroY_rads()** gets the gyroscope value from the data buffer in the Y direction and returns it in units of rad/s.

```C++
float gy;
gy = bmi.getGyroY_rads();
```

**float getGyroZ_rads()** gets the gyroscope value from the data buffer in the Z direction and returns it in units of rad/s.

```C++
float gz;
gz = bmi.getGyroZ_rads();
```

**float getTemperature_C()** gets the die temperature value from the data buffer and returns it in units of C.

```C++
float t;
t = bmi.getTemperature_C();
```

**uint64_t getTime_ps()** gets the sensor time value from the data buffer and returns it in units of ps.

```C++
uint_64t time_ps;
time_ps = bmi.getTime_ps();
```

## <a name="sensor-orientation"></a>Sensor Orientation
This library transforms all data to a common axis system before it is returned. This axis system is shown below. It is a right handed coordinate system with the z-axis positive down, common in aircraft dynamics.

<img src="https://github.com/bolderflight/MPU9250/blob/master/extras/MPU-9250-AXIS.png" alt="Common Axis System" width="250">

**Caution!** This axis system is shown relative to the BMI088 sensor. The sensor may be rotated relative to the breakout board. 

## Example List
* **Basic_I2C**: demonstrates using I2C to communicate with the sensor.
* **Basic_SPI**: demonstrates using SPI to communicate with the sensor.
* **Int_I2C**: demonstrates setting up the output data rates and interrupt pins. Data ready interrupts are used to read the sensor and communication is I2C.
* **Sync_SPI**: demonstrates interfacing with the sensor using synchronized output via the Bmi088 object. SPI is used for communication.

# Wiring and Pullups 
Please refer to the [BMI088 datasheet](https://github.com/bolderflight/BMI088/blob/master/extras/BST-BMI088-DS001.pdf) and your microcontroller's pinout diagram.

## I2C

The BMI088 pins should be connected as:
   * VDD: this should be a 1.7V to 3.6V power source.
   * GND: ground.
   * VDDIO: digital I/O supply voltage. This should be between 1.2V and 3.6V.
   * SDA / SDI: connect to SDA.
   * SCL / SCK: connect to SCL.
   * SDO1: ground to select accelerometer I2C address 0x18. Pull high to VDD to select I2C address 0x19.
   * SDO2: ground to select gyro I2C address 0x68. Pull high to VDD to select I2C address 0x69.
   * PS: pull high to VDD to select I2C.
   * CSB1: float
   * CSB2: float
   * INT1 / INT2: use as data ready interrupts for the accelerometer as desired. Tie one to INT3 / INT4 for use in synchronized modes.
   * INT3 / INT4: use as data ready interrupts for the gyro as desired. Tie one to INT1 / INT2 for use in synchronized modes.

2.2 kOhm - 4.7 kOhm resistors should be used as pullups on SDA and SCL, these resistors should pullup with a 3.3V source.

## SPI

The BMI088 pins should be connected as:
   * VDD: this should be a 1.7V to 3.6V power source.
   * GND: ground.
   * VDDIO: digital I/O supply voltage. This should be between 1.2V and 3.6V.
   * SDA / SDI: connect to MOSI.
   * SCL / SCK: connect to SCL.
   * SDO1: connect to MISO.
   * SDO2: connect to MISO.
   * PS: ground to select SPI.
   * CSB1: connect to accelerometer chip select pin.
   * CSB2: connect to gyro chip select pin.
   * INT1 / INT2: use as data ready interrupts for the accelerometer as desired. Tie one to INT3 / INT4 for use in synchronized modes.
   * INT3 / INT4: use as data ready interrupts for the gyro as desired. Tie one to INT1 / INT2 for use in synchronized modes.
