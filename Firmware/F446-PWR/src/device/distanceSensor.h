#ifndef _DISTANCESENSOR_H_
#define _DISTANCESENSOR_H_

#include <Arduino.h>
#include <VL53L0X.h>

#include "../kit/IO-Kit.h"

class DistanceSensor {
   private:
    static const int Quantity = 2;
    const int FirstAddr = 0x30;

    VL53L0X vl53l0x[Quantity];
    TwoWire* wirePtr;

    Output xshutPin[Quantity] = {Output(PA5), Output(PA6)};

   public:
    DistanceSensor(TwoWire* _wirePtr);

    bool init(void);
    bool read(void);

    int val[Quantity] = {0};
};
#endif  // _DISTANCESENSOR_H_
