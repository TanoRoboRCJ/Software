#ifndef _MOTION_H_
#define _MOTION_H_

#include "../device/device.h"
#include "../kit/RTOS-Kit.h"
#include "../process/process.h"

class Movement {
   private:
    double _oldCoordinateX = 0;
    double _oldCoordinateY = 0;

    const int _Wait = 500;
   public:
    void move_1tile(void);

    void turnRight(void);
    void turnLeft(void);
    void turnReverse(void);
};

extern Movement movement;

#endif