#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

#include "../device/device.h"
#include "../kit/RTOS-Kit.h"
#include "./algorithm/field.h"
#include "./rtosIO.h"
#include "./rtosLocation.h"
#include "./rtosVictim.h"
#include "./search.h"
#include "./algorithm/field.h"

extern RTOS_Kit app;
Field field;

#define SPEED 100
#define WAIT 500
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3
#define MAX_DISTANCE 800
#define RETURN_TIME 300000  // 帰還開始時間(ms)

void rightWallApp(App);
void monitorApp(App);
void adjustmentApp(App);

void info(void);

static bool JCT[MAP_ORIGIN * 2][MAP_ORIGIN * 2] = {false};
static int info_show                            = 0;


void monitorApp(App) {//NOTE センサの値見たい時に使う。
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

void info(void){
    uart1.print("(");
        uart1.print(Map[info_show].x);
        uart1.print(",");
        uart1.print(Map[info_show].y);
        uart1.print(")");
        uart1.print("\t");

        uart1.print("FloorColor:");
        switch(Map[info_show].color){
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
    int weight[3] = {RIGHT_WEIGHT, FRONT_WEIGHT, LEFT_WEIGHT};
    weight[right] += RightWeight();
    weight[front] += FrontWeight();
    weight[left] += LeftWeigt();

    if (tof.val[4] < 170) {
        weight[right] = DISABLE;
    }
    if (tof.val[0] < 170) {
        weight[front] = DISABLE;
    }
    if (tof.val[12] < 170) {
        weight[left] = DISABLE;
    }
    if (weight[right] <= weight[front] && weight[right] <= weight[left]) {
        return right;
    } else if (weight[front] <= weight[right] &&
               weight[front] <= weight[left]) {
        return front;
    } else if (weight[left] <= weight[right] && weight[left] <= weight[front]) {
        return left;
    }
}

int RightWeight(void) {
    if (gyro.West && !tof.isNorthWall &&
        reachedCount[location.x + MAP_ORIGIN][location.y + MAP_ORIGIN + 1] !=
            0) {
        return PASSED_WEIGHT * reachedCount[location.x + MAP_ORIGIN]
                                            [location.y + MAP_ORIGIN + 1];
    } else if (gyro.North && !tof.isEastWall &&
               reachedCount[location.x + MAP_ORIGIN + 1]
                            [location.y + MAP_ORIGIN] != 0) {
        return PASSED_WEIGHT * reachedCount[location.x + MAP_ORIGIN + 1]
                                            [location.y + MAP_ORIGIN];
    } else if (gyro.East && !tof.isSouthWall &&
               reachedCount[location.x + MAP_ORIGIN]
                            [location.y + MAP_ORIGIN - 1] != 0) {
        return PASSED_WEIGHT * reachedCount[location.x + MAP_ORIGIN]
                                            [location.y + MAP_ORIGIN - 1];
    } else if (gyro.South && !tof.isWestWall &&
               reachedCount[location.x + MAP_ORIGIN - 1]
                            [location.y + MAP_ORIGIN] != 0) {
        return PASSED_WEIGHT * reachedCount[location.x + MAP_ORIGIN - 1]
                                            [location.y + MAP_ORIGIN];
    } else {
        return 0;
    }
}

int FrontWeight(void) {
    if (gyro.West && !tof.isWestWall &&
        reachedCount[location.x + MAP_ORIGIN - 1][location.y + MAP_ORIGIN] !=
            0) {
        return PASSED_WEIGHT * reachedCount[location.x + MAP_ORIGIN - 1]
                                            [location.y + MAP_ORIGIN];
    } else if (gyro.North && !tof.isNorthWall &&
               reachedCount[location.x + MAP_ORIGIN]
                            [location.y + MAP_ORIGIN + 1] != 0) {
        return PASSED_WEIGHT * reachedCount[location.x + MAP_ORIGIN]
                                            [location.y + MAP_ORIGIN + 1];
    } else if (gyro.East && !tof.isEastWall &&
               reachedCount[location.x + MAP_ORIGIN + 1]
                            [location.y + MAP_ORIGIN] != 0) {
        return PASSED_WEIGHT * reachedCount[location.x + MAP_ORIGIN + 1]
                                            [location.y + MAP_ORIGIN];
    } else if (gyro.South && !tof.isSouthWall &&
               reachedCount[location.x + MAP_ORIGIN]
                            [location.y + MAP_ORIGIN - 1] != 0) {
        return PASSED_WEIGHT * reachedCount[location.x + MAP_ORIGIN]
                                            [location.y + MAP_ORIGIN - 1];
    } else {
        return 0;
    }
}

int LeftWeigt(void) {
    if (gyro.West && !tof.isSouthWall &&
        reachedCount[location.x + MAP_ORIGIN][location.y + MAP_ORIGIN - 1] !=
            0) {
        return PASSED_WEIGHT * reachedCount[location.x + MAP_ORIGIN]
                                            [location.y + MAP_ORIGIN - 1];
    } else if (gyro.North && !tof.isWestWall &&
               reachedCount[location.x + MAP_ORIGIN - 1]
                            [location.y + MAP_ORIGIN] != 0) {
        return PASSED_WEIGHT * reachedCount[location.x + MAP_ORIGIN - 1]
                                            [location.y + MAP_ORIGIN];
    } else if (gyro.East && !tof.isNorthWall &&
               reachedCount[location.x + MAP_ORIGIN]
                            [location.y + MAP_ORIGIN + 1] != 0) {
        return PASSED_WEIGHT * reachedCount[location.x + MAP_ORIGIN]
                                            [location.y + MAP_ORIGIN + 1];
    } else if (gyro.South && !tof.isEastWall &&
               reachedCount[location.x + MAP_ORIGIN + 1]
                            [location.y + MAP_ORIGIN] != 0) {
        return PASSED_WEIGHT * reachedCount[location.x + MAP_ORIGIN + 1]
                                            [location.y + MAP_ORIGIN];
    } else {
        return 0;
    }
}

#endif