/** gyro.h
 * ジャイロ読むよ
 * 次の機体ではセンサー変えるかも？
 */

#ifndef _GYRO_H_
#define _GYRO_H_

#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <EEPROM.h>
#include <SPI.h>
#include <Wire.h>

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

#endif