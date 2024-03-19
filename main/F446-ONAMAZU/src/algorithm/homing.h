#ifndef HOMING_H_
#define HOMING_H_

#include "../algorithm/exploring.h"
#include "../device/device.h"
#include "../kit/RTOS-Kit.h"
#include "../process/process.h"

class Homing {
   public:
    const int HomingTime = 300000;
    bool started         = false;

    bool hasFinished = false;

    int homingWeighting(void);
    int homingRightWeight(void);
    int homingFrontWeight(void);
    int homingLeftWeight(void);
    int homingBackWeight(void);

    int dijkstra(int destX, int destY, int originX = 0, int originY = 0);
    int dijkstraWeighting(void);
    int dijkstraSteps[FIELD_ORIGIN * 2][FIELD_ORIGIN * 2] = {0};

    int homingReachedCount[FIELD_ORIGIN * 2][FIELD_ORIGIN * 2] = {0};

   private:
};

extern Homing homing;

#endif