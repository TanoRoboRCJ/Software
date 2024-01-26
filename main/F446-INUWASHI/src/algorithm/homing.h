#ifndef HOMING_H_
#define HOMING_H_

#include "../device/device.h"
#include "../kit/RTOS-Kit.h"
#include "../process/process.h"
#include "../algorithm/exploring.h"

class Homing{
    public:
        const int HomingTime = 15000;

        int compareLocation(int x, int y);
    private:
};

extern Homing homing;

#endif