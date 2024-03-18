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
#include <Adafruit_BNO08x.h>

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

struct euler_t {
    float yaw;
    float pitch;
    float roll;
};

class GYRO {
   public:
    euler_t ypr;
    GYRO(Adafruit_BNO08x *p);

    void quaternionToEuler(float qr, float qi, float qj, float qk, euler_t *ypr,
                           bool degrees = false);
    void quaternionToEulerRV(sh2_RotationVectorWAcc_t *rotational_vector,
                             euler_t *ypr, bool degrees = false);

    void init(void);
    void setOffset(void);
    int read(void);
    void directionDecision(void);
    void displaySensorOffsets(const adafruit_bno055_offsets_t &calibData);

    Adafruit_BNO08x *sensorPtr;

    int deg = 0;
    int offset = 0;

    int error = 0;
    int oldDeg = 0;

    int slope = 0;  // 上向が負
    int slopeOffset = 0;

    bool North = false;
    bool East = false;
    bool South = false;
    bool West = false;

    int direction = 0;

   private:
};

#endif

#endif