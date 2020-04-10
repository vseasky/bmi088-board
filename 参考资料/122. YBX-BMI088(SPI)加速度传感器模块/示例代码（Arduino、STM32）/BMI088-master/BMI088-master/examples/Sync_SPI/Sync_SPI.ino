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

#include "BMI088.h"

/* BMI088 object */
Bmi088 bmi(SPI,10,9);

void drdy()
{
  /* read the imu */
  bmi.readSensor();
  /* print the data */
  Serial.print(bmi.getAccelX_mss());
  Serial.print("\t");
  Serial.print(bmi.getAccelY_mss());
  Serial.print("\t");
  Serial.print(bmi.getAccelZ_mss());
  Serial.print("\t");
  Serial.print(bmi.getGyroX_rads());
  Serial.print("\t");
  Serial.print(bmi.getGyroY_rads());
  Serial.print("\t");
  Serial.print(bmi.getGyroZ_rads());
  Serial.print("\t");
  Serial.print(bmi.getTemperature_C());
  Serial.print("\n");
}

void setup() 
{
  int status;
  /* USB Serial to print data */
  Serial.begin(115200);
  while(!Serial) {}
  /* start the sensors */
  status = bmi.begin();
  if (status < 0) {
    Serial.println("IMU Initialization Error");
    Serial.println(status);
    while (1) {}
  }
  /* set the ranges */
  status = bmi.setRange(Bmi088::ACCEL_RANGE_6G,Bmi088::GYRO_RANGE_500DPS);
  if (status < 0) {
    Serial.println("Failed to set ranges");
    Serial.println(status);
    while (1) {}
  }
  /* set the output data rate */
  status = bmi.setOdr(Bmi088::ODR_400HZ);
  if (status < 0) {
    Serial.println("Failed to set ODR");
    Serial.println(status);
    while (1) {}
  }
  /* specify whether to use pin 3 or pin 4 to loop back the gyro interrupt */
  status = bmi.mapSync(Bmi088::PIN_3);
  if (status < 0) {
    Serial.println("Failed to map sync pin");
    Serial.println(status);
    while (1) {}
  }
  /* 
  * specify whether to use pin 1 or pin 2 to indicate data ready, the other pin will be used
  * for gyro interrupt input 
  */
  status = bmi.mapDrdy(Bmi088::PIN_2);
  if (status < 0) {
    Serial.println("Failed to map data ready pin");
    Serial.println(status);
    while (1) {}
  }
  /* set the data ready pin to push-pull and active high */
  status = bmi.pinModeDrdy(Bmi088::PUSH_PULL,Bmi088::ACTIVE_HIGH);
  if (status < 0) {
    Serial.println("Failed to setup data ready pin");
    Serial.println(status);
    while (1) {}
  }
  /* attach the corresponding uC pin to an interrupt */
  pinMode(1,INPUT);
  attachInterrupt(1,drdy,RISING);
}

void loop() 
{
}
