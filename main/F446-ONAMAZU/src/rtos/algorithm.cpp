/**
 * algorithm.cpp
 * 探索・帰還などのアルゴリズム的な動作
 * もうちょっといいファイル名があると思うので思いついたら変えてください
 */

#include "../algorithm/exploring.h"
#include "../algorithm/homing.h"
#include "../algorithm/movement.h"
#include "./RTOS.h"

void rightWallApp(App) {
    static bool originUpdate = true;
    while (1) {
        if (originUpdate) {
            exploring.reachedCount[FIELD_ORIGIN][FIELD_ORIGIN]++;
            exploring.updateMap();
            originUpdate = false;
        }
        app.delay(Period);
        servo.suspend  = true;
        servo.velocity = 0;

        switch (exploring.weighting()) {
            case 0:  // right
                movement.turnRight();
                break;
            case 1:  // front
                break;
            case 2:  // left
                movement.turnLeft();
                break;
        }
        movement.move_1tile();
        exploring.reachedCount[location.x + FIELD_ORIGIN]
                              [location.y + FIELD_ORIGIN]++;
        exploring.updateMap();
        app.delay(100);
    }
}

void adjustmentApp(App) {  // NOTE movement.hに移行
    while (1) {
        movement.angleAdjustment();
        movement.avoidBarrier();
        app.delay(Period);
    }
}

void floorApp(App) {
    int waitTime = 0;
    while (1) {
        if (floorSensor.frontColor == floorSensor.BLACK) {
            app.stop(rightWallApp);
            servo.suspend  = true;
            servo.velocity = 0;
            movement.back();
            if (gyro.direction == NORTH) {
                exploring.reachedCount[location.x + FIELD_ORIGIN]
                                      [location.y + FIELD_ORIGIN + 1] = 50;
                homing.homingReachedCount[location.x + FIELD_ORIGIN]
                                         [location.y + FIELD_ORIGIN + 1] = 50;
            }
            if (gyro.direction == EAST) {
                exploring.reachedCount[location.x + FIELD_ORIGIN + 1]
                                      [location.y + FIELD_ORIGIN] = 50;
                homing.homingReachedCount[location.x + FIELD_ORIGIN + 1]
                                         [location.y + FIELD_ORIGIN] = 50;
            }
            if (gyro.direction == SOUTH) {
                exploring.reachedCount[location.x + FIELD_ORIGIN]
                                      [location.y + FIELD_ORIGIN - 1] = 50;
                homing.homingReachedCount[location.x + FIELD_ORIGIN]
                                         [location.y + FIELD_ORIGIN - 1] = 50;
            }
            if (gyro.direction == WEST) {
                exploring.reachedCount[location.x + FIELD_ORIGIN - 1]
                                      [location.y + FIELD_ORIGIN] = 50;
                homing.homingReachedCount[location.x + FIELD_ORIGIN - 1]
                                         [location.y + FIELD_ORIGIN] = 50;
            }
            app.restart(rightWallApp);
        }
        if (millis() - 2000 < waitTime) {
            app.delay(Period);
        } else if (floorSensor.backColor == floorSensor.BLUE) {  // 5秒止まる
            app.stop(rightWallApp);
            servo.suspend  = true;
            servo.velocity = 0;
            app.delay(5000);
            waitTime = millis();
            app.start(rightWallApp);
        }
        app.delay(Period);
    }
}

void homingApp(App) {
    static bool isHoming = true;
    while (1) {
        if (millis() > homing.HomingTime && victim.isDetected == false) {
            if (isHoming) {
                app.stop(rightWallApp);
                app.stop(victimNotifyApp);
                homing.homingReachedCount[location.x + FIELD_ORIGIN]
                                         [location.y + FIELD_ORIGIN]++;
                isHoming = false;
            }
            if ((location.x == 0) &&
                (location.y == 0)) {  // FIXME帰還条件をもっと絞る
                app.stop(adjustmentApp);
                servo.suspend  = true;
                servo.velocity = 0;
                buzzer.matsukenSamba();
            } else {
                servo.suspend  = true;
                servo.velocity = 0;
                switch (homing.homingWeighting()) {
                    case 0:  // right
                        movement.turnRight();
                        break;
                    case 1:  // front
                        break;
                    case 2:  // left
                        movement.turnLeft();
                        break;
                }
                movement.move_1tile();
                homing.homingReachedCount[location.x + FIELD_ORIGIN]
                                         [location.y + FIELD_ORIGIN]++;
            }
            app.delay(100);
        } else {
            app.delay(Period);
        }
    }
}
