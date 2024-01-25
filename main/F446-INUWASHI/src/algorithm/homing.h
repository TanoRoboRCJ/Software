#ifndef HOMING_H_
#define HOMING_H_

#include "../device/device.h"
#include "../kit/RTOS-Kit.h"
#include "../process/process.h"

class Homing{
    public:
        int compareLocation(int x, int y);
    private:
};

extern Homing homing;

#endif