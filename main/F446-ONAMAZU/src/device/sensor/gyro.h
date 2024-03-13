/** gyro.h
 * ジャイロ読むよ
 * 次の機体ではセンサー変えるかも？
 */

#ifndef _GYRO_H_
#define _GYRO_H_

#define BNO055_MODE

#include <Arduino.h>
#include <EEPROM.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <MPU6050_6Axis_MotionApps20.h>

#ifdef BNO055_MODE

class GYRO {
   public:
    const bool isGyroDisabled = false;

    GYRO(Adafruit_BNO055 *p);

    void init(void);
    void setOffset(void);
    int read(void);
    void directionDecision(void);
    void displaySensorOffsets(const adafruit_bno055_offsets_t &calibData);

    Adafruit_BNO055 *sensorPtr;

    int deg = 0;
    int magnetic = 0;
    int offset = 0;

    int error = 0;
    int oldDeg = 0;

    int slope = 0;
    int slopeOffset = 0;

    bool North = false;
    bool East = false;
    bool South = false;
    bool West = false;

    int direction = 0;

   private:
};

#else

#define Accel_X -5583
#define Accel_Y 419
#define Accel_Z 752
#define Gyro_X 41
#define Gyro_Y 54
#define Gyro_Z 17

class GYRO {
   public:
    const bool isGyroDisabled = false;

    GYRO(Adafruit_BNO055 *p);

    void init(void);
    void setOffset(void);
    int read(void);
    void directionDecision(void);

    Adafruit_BNO055 *sensorPtr;

    int deg = 0;
    int magnetic = 0;
    int offset = 0;

    int error = 0;
    int oldDeg = 0;

    int slope = 0;
    int slopeOffset = 0;

    bool North = false;
    bool East = false;
    bool South = false;
    bool West = false;

    int direction = 0;

   private:
    MPU6050 mpu;
    uint8_t mpuIntStatus;
    bool dmpReady = false;  // set true if DMP init was successful
    uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)

    uint16_t fifoCount;
    uint8_t fifoBuffer[64];  // FIFO storage buffer                 //
                             // orientation/motion vars
    Quaternion q;            // [w, x, y, z]         quaternion container
    VectorFloat gravity;     // [x, y, z]            gravity vector
    float ypr[3];
};

#endif

#endif