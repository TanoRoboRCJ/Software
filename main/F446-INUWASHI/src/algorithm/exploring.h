#ifndef _EXPLORING_H_
#define _EXPLORING_H_

#include "../device/device.h"
#include "../kit/RTOS-Kit.h"
#include "../process/process.h"

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

#define PASSED_WEIGHT 5
#define RIGHT 0
#define FRONT 1
#define LEFT 2
#define DISABLE 50

class Exploring {
   public:
    int  maximumArray= 0;


    void updateMap(void);

    int rightWeight(void);
    int frontWeight(void);
    int leftWeigt(void);
    int weighting(void);

    int reachedCount[FIELD_ORIGIN * 2][FIELD_ORIGIN * 2];
};

extern Exploring exploring;

#endif