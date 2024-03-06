#ifndef _FLOOR_SENSOR_H_
#define _FLOOR_SENSOR_H_

#include <Arduino.h>
#include <Wire.h>

/**
 * 積分時間
 * 0xFF cycle   1   2.4ms
 * 0xF6 cycle  10   24ms
 * 0xD5 cycle  42   101ms
 * 0xC0 cycle  64   156ms
 * 0x00 cycle 256   700ms
 * 
 * ゲイン
 * 0x00   1
 * 0x01   4
 * 0x10  16
 * 0x11  60
 */

class FloorSensor {
   private:
    static const char Addr = 0x29;
    TwoWire* wirePtr;

    const uint8_t Exposure = 0xF6;
    const uint8_t Gain = 0x11;

   public:
    FloorSensor(TwoWire* _wirePtr);
    void init(void);
    void read();

    uint8_t id;
    uint16_t clear;
    uint16_t rgb[3];
};

#endif  // _FLOOR_SENSOR_H_