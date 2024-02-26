#ifndef _FLOOR_SENSOR_H_
#define _FLOOR_SENSOR_H_

#include <Arduino.h>
#include <Wire.h>

class FloorSensor {
   private:
    const char Addr = 0x29;

   public:
    FloorSensor(TwoWire* wirePtr, uint8_t addr);
    void init();
    uint16_t read();
    TwoWire* wirePtr;
};

#endif  // _FLOOR_SENSOR_H_