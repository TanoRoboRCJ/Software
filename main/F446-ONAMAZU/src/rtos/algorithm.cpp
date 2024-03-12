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
        app.delay(300);
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
            if (homing.started == true) {
                app.stop(homingApp);
            } else {
                app.stop(rightWallApp);
            }
            servo.suspend  = true;
            servo.velocity = 0;
            movement.back();
            if (gyro.direction == NORTH) {
                exploring.reachedCount[location.x + FIELD_ORIGIN]
                                      [location.y + FIELD_ORIGIN + 1] = 5;
                homing.homingReachedCount[location.x + FIELD_ORIGIN]
                                         [location.y + FIELD_ORIGIN + 1] = 5;
            }
            if (gyro.direction == EAST) {
                exploring.reachedCount[location.x + FIELD_ORIGIN + 1]
                                      [location.y + FIELD_ORIGIN] = 5;
                homing.homingReachedCount[location.x + FIELD_ORIGIN + 1]
                                         [location.y + FIELD_ORIGIN] = 5;
            }
            if (gyro.direction == SOUTH) {
                exploring.reachedCount[location.x + FIELD_ORIGIN]
                                      [location.y + FIELD_ORIGIN - 1] = 5;
                homing.homingReachedCount[location.x + FIELD_ORIGIN]
                                         [location.y + FIELD_ORIGIN - 1] = 5;
            }
            if (gyro.direction == WEST) {
                exploring.reachedCount[location.x + FIELD_ORIGIN - 1]
                                      [location.y + FIELD_ORIGIN] = 5;
                homing.homingReachedCount[location.x + FIELD_ORIGIN - 1]
                                         [location.y + FIELD_ORIGIN] = 5;
            }
            if (homing.started == true) {
                app.restart(homingApp);
            } else {
                app.restart(rightWallApp);
            }
        }
        if (millis() - 2000 < waitTime) {
            app.delay(Period);
        } else if (floorSensor.backColor == floorSensor.BLUE) {  // 5秒止まる
            if (homing.started == true) {
                app.stop(homingApp);
            } else {
                app.stop(rightWallApp);
            }
            servo.suspend  = true;
            servo.velocity = 0;
            app.delay(5000);
            waitTime = millis();
            if (homing.started == true) {
                app.start(homingApp);
            } else {
                app.start(rightWallApp);
            }
        }
        app.delay(Period);
    }
}

void homingApp(App) {  // CHECK 最適化されてない
    while (1) {
        if (millis() > homing.HomingTime) {
            if (homing.started == false && servo.suspend == true &&
                victim.isDetected == false) {
                app.stop(rightWallApp);
                homing.homingReachedCount[location.x + FIELD_ORIGIN]
                                         [location.y + FIELD_ORIGIN]++;
                buzzer.beat(440, 2);
                homing.started = true;
            }
            if (homing.started == true) {
                // if ((location.x == 0) &&
                //     (location.y == 0)) {  // NOTE 座標厳密モード
                //     app.stop(adjustmentApp);
                //     servo.suspend  = true;
                //     servo.velocity = 0;
                //     buzzer.matsukenSamba();
                // }

                if ((abs(location.x) <= 1) && (abs(location.y) <= 1) &&
                    (location.route[0].wall[0] == tof.wallExists[NORTH]) &&
                    (location.route[0].wall[1] == tof.wallExists[EAST]) &&
                    (location.route[0].wall[2] == tof.wallExists[SOUTH]) &&
                    (location.route[0].wall[3] == tof.wallExists[WEST])) {// NOTE 座標曖昧壁判定モード
                    app.stop(adjustmentApp);
                    servo.suspend  = true;
                    servo.velocity = 0;
                    buzzer.matsukenSamba();
                } else {
                    app.delay(Period);
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
                    app.delay(300);
                }
            }
            app.delay(Period);
        } else {
            app.delay(Period);
        }
    }
}
