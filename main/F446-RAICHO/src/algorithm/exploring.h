#ifndef _EXPLORING_H_
#define _EXPLORING_H_

#include "../device/device.h"
#include "../kit/RTOS-Kit.h"
#include "../process/process.h"
#include "./algorithm/movement.h"

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

#define PASSED_WEIGHT 5
#define RIGHT 0
#define FRONT 1
#define LEFT 2
#define BACK 3
#define DISABLE 100

#define FIELD_3D

#ifndef FIELD_3D

class Exploring {
   public:
    void updateMap(void);

    int rightWeight(void);
    int frontWeight(void);
    int leftWeight(void);
    int weighting(void);

    int reachedCount[FIELD_ORIGIN * 2][FIELD_ORIGIN * 2] = {0};
};

#else

class Exploring {
   private:
    static const int FloorNum = 3;

   public:
    void updateMap(void);

    int rightWeight(void);
    int frontWeight(void);
    int leftWeight(void);
    int weighting(void);

    int reachedCount[FIELD_ORIGIN * 2][FIELD_ORIGIN * 2] = {0};

    struct reachdCount3D_tile {
        int count = 0;
        int z = 0;
    };

    struct reachdCount3D_tile reachedCount3D[FIELD_ORIGIN * 2][FIELD_ORIGIN * 2]
                                            [FloorNum];

    int getReachedCount3D(int x, int y);

    int* reachedCount3DPtr(int x, int y);
};

#endif

extern Exploring exploring;

#endif