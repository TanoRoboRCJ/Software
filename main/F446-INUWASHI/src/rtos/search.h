#ifndef _SEARCH_H_
#define _SEARCH_H_

#include "../device/device.h"
#include "../kit/RTOS-Kit.h"
#include "./algorithm.h"
#include "./rtosIO.h"
#include "./rtosLocation.h"
#include "./rtosVictim.h"

extern RTOS_Kit app;
extern void updateMap();

#define SPEED 100
#define WAIT 500
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3


static double oldCoordinateX                            = 0;
static double oldCoordinateY                            = 0;
static int reachedCount[MAP_ORIGIN * 2][MAP_ORIGIN * 2] = {0};

void AstarApp(App);
void adjustmentApp(App);
void turnRight(void);
void turnLeft(void);
void turnReverse(void);
int weighting(void);
int rightWeight(void);
int frontWeight(void);
int leftWeigt(void);
void move_1tile(void);

// extern void updateMap(void);

void rightWallApp(App) {
    updateMap();  // 初期状態を記録。以降は停止時に更新
    reachedCount[location.x + MAP_ORIGIN][location.y + MAP_ORIGIN]++;
    while (1) {
        app.delay(period);
        servo.suspend  = true;
        servo.velocity = 0;

        switch (weighting()) {
            case 0: //right
                turnRight();
                break;
            case 1: //front
                break;
            case 2: //left
                turnLeft();
                break;
        }
        move_1tile();
        updateMap();
        reachedCount[location.x + MAP_ORIGIN][location.y + MAP_ORIGIN]++;
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

#endif