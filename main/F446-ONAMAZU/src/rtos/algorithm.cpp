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
    // CHECK: これstaticじゃなくてよくね？
    static bool originUpdate = true;
    while (1) {
        bottom.LED_color[0] = 0;
        bottom.LED_color[1] = 255;
        bottom.LED_color[2] = 150;

        if (originUpdate) {
            exploring.reachedCount[FIELD_ORIGIN][FIELD_ORIGIN]++;
            exploring.updateMap();
            originUpdate = false;
        }

        while (movement.isHit == true) {
            app.delay(Period);
        }
        app.delay(Period);

        // CHECK:とりあえずプリントデバッグしてみよう
        if (abs(gyro.slope) < 15) {
            // CHECK:右の判定を30cm地点じゃなくて、28 -
            // 30の地点で空いていたら見たいにしたいね
            switch (exploring.weighting()) {
                case 0:  // right
                         // uart1.println("CASE A: right");
                    servo.suspend  = true;
                    servo.velocity = 0;
                    movement.turnRight();
                    break;
                case 1:  // front
                    // uart1.println("CASE B: front");
                    break;
                case 2:  // left
                    servo.suspend  = true;
                    servo.velocity = 0;
                    movement.turnLeft();
                    // uart1.println("CASE C: left");
                    break;
            }
        }
        movement.move_1tile();
        // app.delay(100);
        exploring.reachedCount[location.x + FIELD_ORIGIN]
                              [location.y + FIELD_ORIGIN]++;

        exploring.updateMap();
    }
}

void adjustmentApp(App) {
    while (1) {
        while (movement.isStucked(gyro.direction) == true) {
            app.delay(Period);
        }
        movement.angleAdjustment();
        movement.avoidBarrier();
        app.delay(Period);
    }
}

void floorApp(App) {
    while (1) {
        int blueTileX = FIELD_ORIGIN;
        int blueTileY = FIELD_ORIGIN;
        if (floorSensor.frontColor == floorSensor.BLACK &&
            abs(gyro.slope) <= 5) {
            if (homing.started == true && homing.hasFinished == false) {
                app.stop(homingApp);
            } else {
                app.stop(rightWallApp);
            }
            servo.suspend  = true;
            servo.velocity = 0;
            movement.back();
            app.delay(100);
            if (gyro.direction == NORTH) {
                exploring.reachedCount[location.x + FIELD_ORIGIN]
                                      [location.y + FIELD_ORIGIN + 1] = 20;
                homing.homingReachedCount[location.x + FIELD_ORIGIN]
                                         [location.y + FIELD_ORIGIN + 1] = 50;

                location.setToAvoidBlackTile(location.x, location.y + 1);
            }
            if (gyro.direction == EAST) {
                exploring.reachedCount[location.x + FIELD_ORIGIN + 1]
                                      [location.y + FIELD_ORIGIN] = 20;
                homing.homingReachedCount[location.x + FIELD_ORIGIN + 1]
                                         [location.y + FIELD_ORIGIN] = 50;

                location.setToAvoidBlackTile(location.x + 1, location.y);
            }
            if (gyro.direction == SOUTH) {
                exploring.reachedCount[location.x + FIELD_ORIGIN]
                                      [location.y + FIELD_ORIGIN - 1] = 20;
                homing.homingReachedCount[location.x + FIELD_ORIGIN]
                                         [location.y + FIELD_ORIGIN - 1] = 50;

                location.setToAvoidBlackTile(location.x, location.y - 1);
            }
            if (gyro.direction == WEST) {
                exploring.reachedCount[location.x + FIELD_ORIGIN - 1]
                                      [location.y + FIELD_ORIGIN] = 20;
                homing.homingReachedCount[location.x + FIELD_ORIGIN - 1]
                                         [location.y + FIELD_ORIGIN] = 50;

                location.setToAvoidBlackTile(location.x - 1, location.y);
            }
            if (homing.started == true && homing.hasFinished == false) {
                app.restart(homingApp);
            } else {
                app.restart(rightWallApp);
            }
        }
        if (floorSensor.backColor == floorSensor.BLUE &&
            floorSensor.frontColor == floorSensor.BLUE &&
            abs(gyro.slope) <= 5) {  // 5秒止まる
            if (homing.started == true && homing.hasFinished == false) {
                app.stop(homingApp);
            } else {
                app.stop(rightWallApp);
            }
            servo.suspend  = true;
            servo.velocity = 0;
            blueTileX      = location.x;
            blueTileY      = location.y;
            app.delay(5500);
            if (homing.started == true && homing.hasFinished == false) {
                app.start(homingApp);
            } else {
                app.start(rightWallApp);
            }
            while (location.x == blueTileX && location.y == blueTileY) {
                app.delay(Period);
            }
        }
        if (floorSensor.frontColor == floorSensor.SILVER &&
            floorSensor.backColor == floorSensor.SILVER && gyro.slope == 0) {
            floorSensor.checkPointX = location.x;
            floorSensor.checkPointY = location.y;
            app.delay(Period);
        }
        app.delay(Period);
    }
}

void homingApp(App) {  // CHECK 最適化されてない
    while (1) {
        if (millis() > homing.HomingTime) {
            bottom.LED_color[0] = 255;
            bottom.LED_color[1] = 120;
            bottom.LED_color[2] = 0;
            if (homing.started == false && servo.suspend == true &&
                victim.isDetected == false) {
                app.stop(rightWallApp);

                if (millis() > homing.HomingTime + 5000) {
                    homing.homingReachedCount[location.x + FIELD_ORIGIN]
                                             [location.y + FIELD_ORIGIN]++;
                }
                buzzer.beat(440, 2);
                homing.started = true;
            }
            //  if ((abs(location.x) <= 1) && (abs(location.y) <= 1) &&
            //         (location.route[0].wall[0] == tof.wallExists[NORTH]) &&
            //         (location.route[0].wall[1] == tof.wallExists[EAST]) &&
            //         (location.route[0].wall[2] == tof.wallExists[SOUTH]) &&
            //         (location.route[0].wall[3] ==
            //          tof.wallExists[WEST])) {  // NOTE 座標曖昧壁判定モード
            //         app.stop(adjustmentApp);
            //         servo.suspend  = true;
            //         servo.velocity = 0;
            //         buzzer.matsukenSamba();
            //     }
            if (homing.started == true) {
                // if ((location.x == 0) &&
                //     (location.y == 0)) {  // NOTE 座標厳密モード
                //     app.stop(adjustmentApp);
                //     servo.suspend = true;
                //     servo.velocity = 0;

                //     homing.hasFinished = true;
                //     buzzer.matsukenSamba();
                // }

                if ((abs(location.x) <= 1) && (abs(location.y) <= 1) &&
                    (location.route[0].wall[0] == tof.wallExists[NORTH]) &&
                    (location.route[0].wall[1] == tof.wallExists[EAST]) &&
                    (location.route[0].wall[2] == tof.wallExists[SOUTH]) &&
                    (location.route[0].wall[3] ==
                     tof.wallExists[WEST])) {  // NOTE 座標曖昧壁判定モード
                    app.stop(adjustmentApp);
                    servo.suspend  = true;
                    servo.velocity = 0;
                    buzzer.matsukenSamba();
                } else {
                    app.delay(Period);
                    if (abs(gyro.slope) < 15) {
                        switch (homing.dijkstraWeighting()) {
                            case 0:  // right
                                servo.suspend  = true;
                                servo.velocity = 0;
                                movement.turnRight();
                                break;
                            case 1:  // front
                                break;
                            case 2:  // left
                                servo.suspend  = true;
                                servo.velocity = 0;
                                movement.turnLeft();
                                break;
                            case 3:  // back
                                servo.suspend  = true;
                                servo.velocity = 0;
                                movement.turnReverse();
                                break;
                        }
                    }
                    movement.move_1tile();

                    if (millis() > homing.HomingTime + 5000) {
                        homing.homingReachedCount[location.x + FIELD_ORIGIN]
                                                 [location.y + FIELD_ORIGIN]++;
                    }
                    // app.delay(100);
                }
            }
            app.delay(Period);
        } else {
            app.delay(Period);
        }
    }
}