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

    int _boundaryPointX = 0;
    int _boundaryPointY = 0;

    bool isHit = false;

    const int Radius = 20;
    const int _Wait = 500;
   public:
    void move_1tile(void);
    void turnRight(void);
    void turnLeft(void);
    void turnReverse(void);
    void turnNorth(void);
    void turnEast(void);
    void turnSouth(void);
    void turnWest(void);

    void angleAdjustment(void);
    void avoidBarrier(void);
};

extern Movement movement;

#endif