#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

#include "../device/device.h"
#include "../kit/RTOS-Kit.h"
#include "./algorithm/field.h"
#include "./rtosIO.h"
#include "./rtosLocation.h"
#include "./rtosVictim.h"
#include "./search.h"

extern RTOS_Kit app;
Field field;

#define PASSED_WEIGHT 5
#define RIGHT 0
#define FRONT 1
#define LEFT 2
#define DISABLE 50

void rightWallApp(App);
void monitorApp(App);
void adjustmentApp(App);

void info(void);

static bool JCT[MAP_ORIGIN * 2][MAP_ORIGIN * 2] = {false};
static int info_show                            = 0;

void monitorApp(App) {  // NOTE センサの値見たい時に使う。
    while (1) {
        uart1.print(floorSensor.redVal);
        uart1.print("\t");
        uart1.print(floorSensor.blankVal);
        uart1.print("\t");
        uart1.print(floorSensor.blueVal);
        uart1.print("\t");
        uart1.print(floorSensor.Color);
        uart1.println("\t");
        app.delay(period);
    }
}

void updateMap(void) {
    static int i = 0;

    Map[i].x = location.x;
    Map[i].y = location.y;

    Map[i].color  = floorSensor.Color;
    Map[i].victim = victim.isDetected;

    Map[i].wall[0] = tof.isNorthWall;  // 北
    Map[i].wall[1] = tof.isEastWall;   // 東
    Map[i].wall[2] = tof.isSouthWall;  // 南
    Map[i].wall[3] = tof.isWestWall;   // 西

    // info();

    i++;
    info_show++;
}

void info(void) {
    uart1.print("(");
    uart1.print(Map[info_show].x);
    uart1.print(",");
    uart1.print(Map[info_show].y);
    uart1.print(")");
    uart1.print("\t");

    uart1.print("FloorColor:");
    switch (Map[info_show].color) {
        case 0:
            uart1.print("White");
            break;
        case 1:
            uart1.print("Black");
            break;
        case 2:
            uart1.print("Blue");
            break;
        case 3:
            uart1.print("Silver");
            break;
    }
    uart1.print("\t");

    uart1.print("Victim:");
    uart1.print(Map[info_show].victim);
    uart1.print("\t");

    uart1.print("North:");
    uart1.print(Map[info_show].wall[0]);
    uart1.print("\t");
    uart1.print("East:");
    uart1.print(Map[info_show].wall[1]);
    uart1.print("\t");
    uart1.print("South:");
    uart1.print(Map[info_show].wall[2]);
    uart1.print("\t");
    uart1.print("West:");
    uart1.print(Map[info_show].wall[3]);
    uart1.print("\n");
}

int weighting(void) {
    int weight[3] = {1, 2, 3};  // {right, front, left}

    weight[RIGHT] += rightWeight();
    weight[FRONT] += frontWeight();
    weight[LEFT] += leftWeigt();

    if (tof.val[4] < 170) {
        weight[RIGHT] = DISABLE;
    }
    if (tof.val[0] < 170) {
        weight[FRONT] = DISABLE;
    }
    if (tof.val[12] < 170) {
        weight[LEFT] = DISABLE;
    }
    if (weight[RIGHT] <= weight[FRONT] && weight[RIGHT] <= weight[LEFT]) {
        return 0;  // right
    } else if (weight[FRONT] <= weight[RIGHT] &&
               weight[FRONT] <= weight[LEFT]) {
        return 1;  // front
    } else if (weight[LEFT] <= weight[RIGHT] && weight[LEFT] <= weight[FRONT]) {
        return 2;  // left
    }
}

int rightWeight(void) {
    int x = location.x + MAP_ORIGIN;
    int y = location.y + MAP_ORIGIN;

    int weight = 0;

    if (gyro.West && !tof.isNorthWall) {
        weight = reachedCount[x][y + 1];
    } else if (gyro.North && !tof.isEastWall) {
        weight = reachedCount[x + 1][y];
    } else if (gyro.East && !tof.isSouthWall) {
        weight = reachedCount[x][y - 1];
    } else if (gyro.South && !tof.isWestWall) {
        weight = reachedCount[x - 1][y];
    }

    return weight * PASSED_WEIGHT;
}

int frontWeight(void) {
    int x = location.x + MAP_ORIGIN;
    int y = location.y + MAP_ORIGIN;

    int weight = 0;

    if (gyro.West && !tof.isWestWall) {
        weight = reachedCount[x - 1][y];

    } else if (gyro.North && !tof.isNorthWall) {
        weight = reachedCount[x][y + 1];

    } else if (gyro.East && !tof.isEastWall) {
        weight = reachedCount[x + 1][y];

    } else if (gyro.South && !tof.isSouthWall) {
        weight = reachedCount[x][y - 1];
    }

    return weight * PASSED_WEIGHT;
}

int leftWeigt(void) {
    int weight = 0;

    int x = location.x + MAP_ORIGIN;
    int y = location.y + MAP_ORIGIN;

    if (gyro.West && !tof.isSouthWall) {
        weight = reachedCount[x][y - 1];

    } else if (gyro.North && !tof.isWestWall) {
        weight = reachedCount[x - 1][y];

    } else if (gyro.East && !tof.isNorthWall) {
        weight = reachedCount[x][y + 1];

    } else if (gyro.South && !tof.isEastWall) {
        weight = reachedCount[x + 1][y];
    }

    return weight * PASSED_WEIGHT;
}

#endif