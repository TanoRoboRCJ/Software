#include "distanceSensor.h"

DistanceSensor::DistanceSensor(TwoWire* _wirePtr) {
    this->wirePtr = _wirePtr;

    for (int i = 0; i < Quantity; i++) {
        xshutPin[i] = LOW;
    }
}

bool DistanceSensor::init(void) {
    for (int i = 0; i < Quantity; i++) {
        xshutPin[i] = HIGH;
        delay(10);

        vl53l0x[i].setBus(wirePtr);
        vl53l0x[i].setTimeout(100);
        vl53l0x[i].setAddress(FirstAddr + i);

        if (!vl53l0x[i].init()) {
            return false;
        }

        vl53l0x[i].setSignalRateLimit(0.1);
        vl53l0x[i].setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
        vl53l0x[i].setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
        vl53l0x[i].startContinuous();
    }

    return true;
}

bool DistanceSensor::read(void) {
    for (int i = 0; i < Quantity; i++) {
        val[i] = vl53l0x[i].readRangeContinuousMillimeters();
    }

    return true;
}
