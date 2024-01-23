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
// void makeMap(void);

static bool JCT[MAP_ORIGIN * 2][MAP_ORIGIN * 2] = {false};
static int infoShow                             = 0;

void monitorApp(App) {  // NOTE センサの値見たい時に使う。
    while (1) {
        // uart1.print(floorSensor.redVal);
        // uart1.print("\t");
        // uart1.print(floorSensor.blankVal);
        // uart1.print("\t");
        // uart1.print(floorSensor.blueVal);
        // uart1.print("\t");
        // uart1.print(floorSensor.Color);
        // uart1.println("\t");
        app.delay(period);
    }
}

void updateMap(void) {
    static int i = 0;

    Map[i].x = location.x;
    Map[i].y = location.y;

    Map[i].color  = floorSensor.Color;
    Map[i].victim = victim.isDetected;

    Map[i].wall[0] = tof.wallExist[NORTH];  // 北
    Map[i].wall[1] = tof.wallExist[EAST];   // 東
    Map[i].wall[2] = tof.wallExist[SOUTH];  // 南
    Map[i].wall[3] = tof.wallExist[WEST];   // 西

    // info();

    i++;
    infoShow++;
    // makeMap();
}

void info(void) {
    uart1.print("(");
    uart1.print(Map[infoShow].x);
    uart1.print(",");
    uart1.print(Map[infoShow].y);
    uart1.print(")");
    uart1.print("\t");

    uart1.print("FloorColor:");
    switch (Map[infoShow].color) {
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
    uart1.print(Map[infoShow].victim);
    uart1.print("\t");

    uart1.print("North:");
    uart1.print(Map[infoShow].wall[0]);
    uart1.print("\t");
    uart1.print("East:");
    uart1.print(Map[infoShow].wall[1]);
    uart1.print("\t");
    uart1.print("South:");
    uart1.print(Map[infoShow].wall[2]);
    uart1.print("\t");
    uart1.print("West:");
    uart1.print(Map[infoShow].wall[3]);
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

    if (gyro.direction == WEST && tof.wallExist[NORTH] == false) {
        weight = reachedCount[x][y + 1];
    } else if (gyro.direction == NORTH && tof.wallExist[EAST] == false) {
        weight = reachedCount[x + 1][y];
    } else if (gyro.direction == EAST && tof.wallExist[SOUTH] == false) {
        weight = reachedCount[x][y - 1];
    } else if (gyro.direction == SOUTH && tof.wallExist[WEST] == false) {
        weight = reachedCount[x - 1][y];
    }

    return weight * PASSED_WEIGHT;
}

int frontWeight(void) {
    int x = location.x + MAP_ORIGIN;
    int y = location.y + MAP_ORIGIN;

    int weight = 0;

    if (gyro.direction == WEST && tof.wallExist[WEST] == false) {
        weight = reachedCount[x - 1][y];

    } else if (gyro.direction == NORTH && tof.wallExist[NORTH] == false) {
        weight = reachedCount[x][y + 1];

    } else if (gyro.direction == EAST && !tof.wallExist[EAST] == false) {
        weight = reachedCount[x + 1][y];

    } else if (gyro.direction == SOUTH && !tof.wallExist[SOUTH] == false) {
        weight = reachedCount[x][y - 1];
    }

    return weight * PASSED_WEIGHT;
}

int leftWeigt(void) {
    int weight = 0;

    int x = location.x + MAP_ORIGIN;
    int y = location.y + MAP_ORIGIN;

    if (gyro.direction == WEST && tof.wallExist[SOUTH] == false) {
        weight = reachedCount[x][y - 1];

    } else if (gyro.direction == NORTH && tof.wallExist[WEST] == false) {
        weight = reachedCount[x - 1][y];

    } else if (gyro.direction == EAST && tof.wallExist[NORTH] == false) {
        weight = reachedCount[x][y + 1];

    } else if (gyro.direction == SOUTH && tof.wallExist[EAST] == false) {
        weight = reachedCount[x + 1][y];
    }

    return weight * PASSED_WEIGHT;
}

// void makeMap(void) {
// }

#endif