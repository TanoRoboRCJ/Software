#ifndef _SEARCH_H_
#define _SEARCH_H_

#include "../device/device.h"
#include "../kit/RTOS-Kit.h"
#include "./algorithm.h"
#include "./rtosIO.h"
#include "./rtosLocation.h"
#include "./rtosVictim.h"

extern RTOS_Kit app;

#define SPEED 100
#define WAIT 500
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3
#define RETURN_TIME 300000  // 帰還開始時間(ms)

#define RIGHT_WEIGHT 1
#define FRONT_WEIGHT 2
#define LEFT_WEIGHT 3
#define PASSED_WEIGHT 5
#define right 0
#define front 1
#define left 2
#define DISABLE 50

bool oldstatus                                           = false;
static int oldmillis                                     = 0;
static double oldCoordinateX                             = 0;
static double oldCoordinateY                             = 0;
static int reachingCount[MAP_ORIGIN * 2][MAP_ORIGIN * 2] = {0};

void AstarApp(App);
void adjustmentApp(App);
void turnRight(void);
void turnLeft(void);
void turnReverse(void);
int weighting(void);
int RightWeight(void);
int FrontWeight(void);
int LeftWeigt(void);
void move_1tile(void);

extern void updateMap(void);

void rightWallApp(App) {
    updateMap();  // 初期状態を記録。以降は停止時に更新
    reachingCount[location.x + MAP_ORIGIN][location.y + MAP_ORIGIN]++;
    while (1) {
        app.delay(period);
        servo.suspend  = true;
        servo.velocity = 0;

        switch (weighting()) {
            case right:
                turnRight();
                break;
            case front:
                break;
            case left:
                turnLeft();
                break;
        }

        move_1tile();
        updateMap();
        reachingCount[location.x + MAP_ORIGIN][location.y + MAP_ORIGIN]++;
        app.delay(100);
    }
}

void adjustmentApp(App) {
    while (1) {
        static bool isHit = false;
        if (tof.val[4] + tof.val[12] < 300) {
            if (tof.val[4] > tof.val[12]) {
                servo.isCorrectingAngle = 5;
            }
            if (tof.val[12] > tof.val[4]) {
                servo.isCorrectingAngle = -5;
            }
            app.delay(50);
        } else {
            if (tof.val[12] < 120) {
                servo.isCorrectingAngle = 5;
            }
            if (tof.val[4] < 120) {
                servo.isCorrectingAngle = -5;
            }
        }

        if (tof.val[4] > 300 && tof.val[12] < 300) {
            if (tof.val[12] > 120) {
                servo.isCorrectingAngle = -5;
            }
        }
        if (tof.val[12] > 300 && tof.val[4] < 300) {
            if (tof.val[4] > 120) {
                servo.isCorrectingAngle = 5;
            }
        }

        if (loadcell.status == RIGHT) {
            app.stop(servoApp);
            servo.driveAngularVelocity(-30, -45);
            app.delay(500);
            servo.driveAngularVelocity(-30, 45);
            app.delay(500);
            isHit = false;
        }
        if (loadcell.status == LEFT) {
            app.stop(servoApp);
            servo.driveAngularVelocity(-30, 45);
            app.delay(500);
            servo.driveAngularVelocity(-30, -45);
            app.delay(500);
            isHit = false;
        }
        if (!isHit) {
            servo.velocity = SPEED;
            app.start(servoApp);
            isHit = true;
        }
        app.delay(period);
    }
}

void turnRight(void) {
    servo.suspend = true;
    app.delay(WAIT);
    servo.suspend = false;
    servo.angle += 90;
    servo.isCorrectingAngle = 0;
    app.delay(WAIT * 2);
}

void turnLeft(void) {
    servo.suspend = true;
    app.delay(WAIT);
    servo.suspend = false;
    servo.angle -= 90;
    servo.isCorrectingAngle = 0;
    app.delay(WAIT * 2);
}

void turnReverse(void) {
    servo.suspend = true;
    app.delay(WAIT);
    servo.suspend = false;
    servo.angle += 180;
    servo.isCorrectingAngle = 0;
    app.delay(WAIT * 3);
}

void move_1tile(void) {
    oldCoordinateX = location.coordinateX;
    oldCoordinateY = location.coordinateY;

    while (abs(location.coordinateX - oldCoordinateX) < 300 &&
           abs(location.coordinateY - oldCoordinateY) < 300) {
        if (tof.val[0] < 150) {
            break;
        }
        servo.suspend  = false;
        servo.velocity = SPEED;
        app.delay(period);
    }  // 次のタイルまで前進
    servo.suspend  = true;
    servo.velocity = 0;
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
        reachingCount[location.x + MAP_ORIGIN][location.y + MAP_ORIGIN + 1] !=
            0) {
        return PASSED_WEIGHT * reachingCount[location.x + MAP_ORIGIN]
                                            [location.y + MAP_ORIGIN + 1];
    } else if (gyro.North && !tof.isEastWall &&
               reachingCount[location.x + MAP_ORIGIN + 1]
                            [location.y + MAP_ORIGIN] != 0) {
        return PASSED_WEIGHT * reachingCount[location.x + MAP_ORIGIN + 1]
                                            [location.y + MAP_ORIGIN];
    } else if (gyro.East && !tof.isSouthWall &&
               reachingCount[location.x + MAP_ORIGIN]
                            [location.y + MAP_ORIGIN - 1] != 0) {
        return PASSED_WEIGHT * reachingCount[location.x + MAP_ORIGIN]
                                            [location.y + MAP_ORIGIN - 1];
    } else if (gyro.South && !tof.isWestWall &&
               reachingCount[location.x + MAP_ORIGIN - 1]
                            [location.y + MAP_ORIGIN] != 0) {
        return PASSED_WEIGHT * reachingCount[location.x + MAP_ORIGIN - 1]
                                            [location.y + MAP_ORIGIN];
    } else {
        return 0;
    }
}

int FrontWeight(void) {
    if (gyro.West && !tof.isWestWall &&
        reachingCount[location.x + MAP_ORIGIN - 1][location.y + MAP_ORIGIN] !=
            0) {
        return PASSED_WEIGHT * reachingCount[location.x + MAP_ORIGIN - 1]
                                            [location.y + MAP_ORIGIN];
    } else if (gyro.North && !tof.isNorthWall &&
               reachingCount[location.x + MAP_ORIGIN]
                            [location.y + MAP_ORIGIN + 1] != 0) {
        return PASSED_WEIGHT * reachingCount[location.x + MAP_ORIGIN]
                                            [location.y + MAP_ORIGIN + 1];
    } else if (gyro.East && !tof.isEastWall &&
               reachingCount[location.x + MAP_ORIGIN + 1]
                            [location.y + MAP_ORIGIN] != 0) {
        return PASSED_WEIGHT * reachingCount[location.x + MAP_ORIGIN + 1]
                                            [location.y + MAP_ORIGIN];
    } else if (gyro.South && !tof.isSouthWall &&
               reachingCount[location.x + MAP_ORIGIN]
                            [location.y + MAP_ORIGIN - 1] != 0) {
        return PASSED_WEIGHT * reachingCount[location.x + MAP_ORIGIN]
                                            [location.y + MAP_ORIGIN - 1];
    } else {
        return 0;
    }
}

int LeftWeigt(void) {
    if (gyro.West && !tof.isSouthWall &&
        reachingCount[location.x + MAP_ORIGIN][location.y + MAP_ORIGIN - 1] !=
            0) {
        return PASSED_WEIGHT * reachingCount[location.x + MAP_ORIGIN]
                                            [location.y + MAP_ORIGIN - 1];
    } else if (gyro.North && !tof.isWestWall &&
               reachingCount[location.x + MAP_ORIGIN - 1]
                            [location.y + MAP_ORIGIN] != 0) {
        return PASSED_WEIGHT * reachingCount[location.x + MAP_ORIGIN - 1]
                                            [location.y + MAP_ORIGIN];
    } else if (gyro.East && !tof.isNorthWall &&
               reachingCount[location.x + MAP_ORIGIN]
                            [location.y + MAP_ORIGIN + 1] != 0) {
        return PASSED_WEIGHT * reachingCount[location.x + MAP_ORIGIN]
                                            [location.y + MAP_ORIGIN + 1];
    } else if (gyro.South && !tof.isEastWall &&
               reachingCount[location.x + MAP_ORIGIN + 1]
                            [location.y + MAP_ORIGIN] != 0) {
        return PASSED_WEIGHT * reachingCount[location.x + MAP_ORIGIN + 1]
                                            [location.y + MAP_ORIGIN];
    } else {
        return 0;
    }
}

#endif