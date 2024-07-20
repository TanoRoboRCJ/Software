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
        // if (abs(gyro.slope) < 15 || movement.loop == true) {
        // NOTE:28-30cmで壁判定Done
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
        // }
        movement.move_1tile();
        // app.delay(100);
        exploring.reachedCount[location.x + FIELD_ORIGIN]
                              [location.y + FIELD_ORIGIN]++;

        exploring.updateMap();
        while (tof.frontWallExists == true && tof.rightWallExists == true &&
               tof.leftWallExists == true) {
            servo.suspend  = true;
            servo.velocity = 0;
        }
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
        if (floorSensor.backColor == floorSensor.BLACK) {
            app.stop(rightWallApp);
            servo.suspend  = true;
            servo.velocity = 0;
            app.delay(5000);
            servo.suspend = false;
            // movement.turnRightAndPause();
            app.start(rightWallApp);
            int tempX = location.x;
            int tempY = location.y;

            while (1) {
                if (location.x != tempX || location.y != tempY) {
                    break;
                }
                app.delay(Period);
            }

            app.delay(1400);
        }

        if (floorSensor.backColor == floorSensor.BLUE) {
            app.stop(rightWallApp);
            servo.suspend  = true;
            servo.velocity = 0;
            app.delay(5000);
            servo.suspend = false;
            // movement.turnRightAndPause();
            app.start(rightWallApp);

            int tempX = location.x;
            int tempY = location.y;

            while (1) {
                if (location.x != tempX || location.y != tempY) {
                    break;
                }
                app.delay(Period);
            }

            app.delay(1400);
        }
        app.delay(Period);
    }
}

void homingApp(App) {  // CHECK 最適化されてない
    while (1) {
        homing.dijkstra(location.x, location.y);
        // uart1.println(homing.dijkstraSteps[location.x + FIELD_ORIGIN]
        //                                   [location.y + FIELD_ORIGIN]);

        if (millis() > homing.HomingTime) {
            bottom.LED_color[0] = 255;
            bottom.LED_color[1] = 120;
            bottom.LED_color[2] = 0;

            bool judgement_3d =
                (location.coordinateZ < 150) && (location.coordinateZ > -150) &&
                (homing.dijkstraSteps[location.x + FIELD_ORIGIN]
                                     [location.y + FIELD_ORIGIN] != -1);

#ifndef FIELD_3D
            judgement_3d = true;
#endif

            if (homing.started == false && servo.suspend == true &&
                victim.isDetected == false && judgement_3d &&
                (floorSensor.frontColor != floorSensor.BLUE) &&
                (floorSensor.backColor != floorSensor.BLUE)) {
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