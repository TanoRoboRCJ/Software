#include <Arduino.h>
#include <Adafruit_BNO055.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <EEPROM.h>
#include <SPI.h>
#include <Wire.h>

#define Serial uart1
HardwareSerial uart1(PA10, PA9);

void GyroStart();
void GyroGet();

void mpuCalib();
void meansensors();
void calibration();

#define Accel_X -5518
#define Accel_Y 442
#define Accel_Z 757
#define Gyro_X 30
#define Gyro_Y 52
#define Gyro_Z 14

MPU6050 mpu;
static uint8_t mpuIntStatus;
static bool dmpReady = false;  // set true if DMP init was successful
static uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)

int16_t Gyro_Now = 0, Gyro = 0, Gyro_Offset = 0;
uint16_t fifoCount;
uint8_t fifoBuffer[64];  // FIFO storage buffer                 //
                         // orientation/motion vars
Quaternion q;            // [w, x, y, z]         quaternion container
VectorFloat gravity;     // [x, y, z]            gravity vector
float ypr[3];

void setup() {
    Wire.setSDA(PB9);
    Wire.setSCL(PB8);
    Wire.begin();
    Serial.begin(115200);

    mpuCalib();

    GyroStart();
}

void loop() {
    GyroGet();

    Serial.println(Gyro);
}

void GyroGet() {
    mpuIntStatus = false;
    mpuIntStatus = mpu.getIntStatus();
    fifoCount = mpu.getFIFOCount();
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        mpu.resetFIFO();
    } else if (mpuIntStatus & 0x02) {
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        fifoCount -= packetSize;
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
        Gyro_Now = degrees(ypr[0]) + 180;
        Gyro = Gyro_Now + Gyro_Offset;
        if (Gyro < 0) Gyro += 360;
        if (Gyro > 359) Gyro -= 360;
    }
}

void GyroStart() {
    mpu.initialize();
    if (mpu.testConnection() != true) {
        Serial.println("MPU disconection");
        while (true) {
        }
    }
    if (mpu.dmpInitialize() != 0) {
        Serial.println("MPU break");
        while (true) {
        }
    }
    mpu.setXGyroOffset(Gyro_X);
    mpu.setYGyroOffset(Gyro_Y);
    mpu.setZGyroOffset(Gyro_Z);
    mpu.setZAccelOffset(Accel_Z);
    mpu.setDMPEnabled(true);
    mpuIntStatus = mpu.getIntStatus();
    dmpReady = true;
    packetSize = mpu.dmpGetFIFOPacketSize();
}

// Arduino sketch that returns calibration offsets for MPU6050 //   Version 1.1
// (31th January 2014) Done by Luis Ródenas <luisrodenaslorda@gmail.com> Based
// on the I2Cdev library and previous work by Jeff Rowberg <jeff@rowberg.net>
// Updates (of the library) should (hopefully) always be available at
// https://github.com/jrowberg/i2cdevlib

// These offsets were meant to calibrate MPU6050's internal DMP, but can be also
// useful for reading sensors. The effect of temperature has not been taken into
// account so I can't promise that it will work if you calibrate indoors and
// then use it outdoors. Best is to calibrate and use at the same room
// temperature.

/* ==========  LICENSE  ==================================
 I2Cdev device library code is placed under the MIT license
 Copyright (c) 2011 Jeff Rowberg

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 =========================================================
 */

// I2Cdev and MPU6050 must be installed as libraries
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"

///////////////////////////////////   CONFIGURATION
////////////////////////////////
// Change this 3 variables if you want to fine tune the skecth to your needs.
int buffersize =
    1000;  // Amount of readings used to average, make it higher to get more
           // precision but sketch will be slower  (default:1000)
int acel_deadzone = 8;  // Acelerometer error allowed, make it lower to get more
                        // precision, but sketch may not converge  (default:8)
int giro_deadzone = 1;  // Giro error allowed, make it lower to get more
                        // precision, but sketch may not converge  (default:1)

int16_t ax, ay, az, gx, gy, gz;

int mean_ax, mean_ay, mean_az, mean_gx, mean_gy, mean_gz, state = 0;
int ax_offset, ay_offset, az_offset, gx_offset, gy_offset, gz_offset;

void mpuCalib() {
    mpu.initialize();
    mpu.setXAccelOffset(0);
    mpu.setYAccelOffset(0);
    mpu.setZAccelOffset(0);
    mpu.setXGyroOffset(0);
    mpu.setYGyroOffset(0);
    mpu.setZGyroOffset(0);
    delay(1000);
    if (state == 0) {
        Serial.println("\nReading sensors for first time...");
        meansensors();
        state++;
        delay(1000);
    }

    if (state == 1) {
        Serial.println("\nCalculating offsets...");
        calibration();
        state++;
        delay(1000);
    }

    if (state == 2) {
        meansensors();
        Serial.println("\nFINISHED!");
        Serial.print("\nSensor readings with offsets:\t");
        Serial.print(mean_ax);
        Serial.print("\t");
        Serial.print(mean_ay);
        Serial.print("\t");
        Serial.print(mean_az);
        Serial.print("\t");
        Serial.print(mean_gx);
        Serial.print("\t");
        Serial.print(mean_gy);
        Serial.print("\t");
        Serial.println(mean_gz);
        Serial.println("Your offsets:\t");
        Serial.print("#define Accel_X ");
        Serial.println(ax_offset);
        Serial.print("#define Accel_Y ");
        Serial.println(ay_offset);
        Serial.print("#define Accel_Z ");
        Serial.println(az_offset);
        Serial.print("#define Gyro_X ");
        Serial.println(gx_offset);
        Serial.print("#define Gyro_Y ");
        Serial.println(gy_offset);
        Serial.print("#define Gyro_Z ");
        Serial.println(gz_offset);

        // Serial.print(ax_offset);
        // Serial.print("\t");
        // Serial.print(ay_offset);
        // Serial.print("\t");
        // Serial.print(az_offset);
        // Serial.print("\t");
        // Serial.print(gx_offset);
        // Serial.print("\t");
        // Serial.print(gy_offset);
        // Serial.print("\t");
        // Serial.println(gz_offset);
        Serial.println(
            "\nData is printed as: acelX acelY acelZ giroX giroY giroZ");
        Serial.println(
            "Check that your sensor readings are close to 0 0 16384 0 0 0");
        Serial.println(
            "If calibration was succesful write down your offsets so you can "
            "set them in your projects using something similar to "
            "mpu.setXAccelOffset(youroffset)");
        while (1)
            ;
    }
}

void meansensors() {
    long i = 0, buff_ax = 0, buff_ay = 0, buff_az = 0, buff_gx = 0, buff_gy = 0,
         buff_gz = 0;

    while (i < (buffersize + 101)) {
        // read raw accel/gyro measurements from device
        mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

        if (i > 100 &&
            i <= (buffersize + 100)) {  // First 100 measures are discarded
            buff_ax = buff_ax + ax;
            buff_ay = buff_ay + ay;
            buff_az = buff_az + az;
            buff_gx = buff_gx + gx;
            buff_gy = buff_gy + gy;
            buff_gz = buff_gz + gz;
        }
        if (i == (buffersize + 100)) {
            mean_ax = buff_ax / buffersize;
            mean_ay = buff_ay / buffersize;
            mean_az = buff_az / buffersize;
            mean_gx = buff_gx / buffersize;
            mean_gy = buff_gy / buffersize;
            mean_gz = buff_gz / buffersize;
        }
        i++;
        delay(2);  // Needed so we don't get repeated measures
    }
}

void calibration() {
    ax_offset = -mean_ax / 8;
    ay_offset = -mean_ay / 8;
    az_offset = (16384 - mean_az) / 8;

    gx_offset = -mean_gx / 4;
    gy_offset = -mean_gy / 4;
    gz_offset = -mean_gz / 4;
    while (1) {
        int ready = 0;
        mpu.setXAccelOffset(ax_offset);
        mpu.setYAccelOffset(ay_offset);
        mpu.setZAccelOffset(az_offset);

        mpu.setXGyroOffset(gx_offset);
        mpu.setYGyroOffset(gy_offset);
        mpu.setZGyroOffset(gz_offset);

        meansensors();
        Serial.println(ready);

        if (abs(mean_ax) <= acel_deadzone)
            ready++;
        else
            ax_offset = ax_offset - mean_ax / acel_deadzone;

        if (abs(mean_ay) <= acel_deadzone)
            ready++;
        else
            ay_offset = ay_offset - mean_ay / acel_deadzone;

        if (abs(16384 - mean_az) <= acel_deadzone)
            ready++;
        else
            az_offset = az_offset + (16384 - mean_az) / acel_deadzone;

        if (abs(mean_gx) <= giro_deadzone)
            ready++;
        else
            gx_offset = gx_offset - mean_gx / (giro_deadzone + 1);

        if (abs(mean_gy) <= giro_deadzone)
            ready++;
        else
            gy_offset = gy_offset - mean_gy / (giro_deadzone + 1);

        if (abs(mean_gz) <= giro_deadzone)
            ready++;
        else
            gz_offset = gz_offset - mean_gz / (giro_deadzone + 1);

        if (ready == 6) break;
    }
}