#ifndef _MOTION_H_
#define _MOTION_H_

#include "../device/device.h"
#include "../kit/RTOS-Kit.h"
#include "../process/process.h"
#include "../rtos/rtos.h"

class Movement {
   private:
    double _oldCoordinateX = 0;
    double _oldCoordinateY = 0;

    const int Radius = 20;
    const int _Wait  = 500;
    bool exception = false;

    void goOverBarrier(void);

   public:
    bool isHit = false;
    bool CanGoRight = false;
    bool CanGoLeft = false;
    void move_1tile(void);
    void back(void);
    void turnRight(void);
    void turnLeft(void);
    void turnReverse(void);
    void turnNorth(void);
    void turnEast(void);
    void turnSouth(void);
    void turnWest(void);

    void angleAdjustment(void);
    void avoidBarrier(void);
    bool isStucked(int direction);
};

extern Movement movement;

#endif