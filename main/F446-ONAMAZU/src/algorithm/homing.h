#ifndef HOMING_H_
#define HOMING_H_

#include "../device/device.h"
#include "../kit/RTOS-Kit.h"
#include "../process/process.h"
#include "../algorithm/exploring.h"

class Homing{
    public:
        const int HomingTime = 30000;
        bool started = false;

        int homingWeighting(void);
        int homingRightWeight(void);
        int homingFrontWeight(void);
        int homingLeftWeight(void);

        int homingReachedCount[FIELD_ORIGIN * 2][FIELD_ORIGIN * 2] = {0};
    private:
};

extern Homing homing;

#endif