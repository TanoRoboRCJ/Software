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
        // exploring.reachedCount[location.x + FIELD_ORIGIN]
        //                       [location.y + FIELD_ORIGIN]++;

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
        app.delay(Period);
    }
}

void homingApp(App) {  // CHECK 最適化されてない
    bool isCompleted = false;
    while (1) {
        homing.dijkstra(location.x, location.y, victim.keyX, victim.keyY);
        // uart1.println(homing.dijkstraSteps[location.x + FIELD_ORIGIN]
        //                                   [location.y + FIELD_ORIGIN]);

        if (homing.started == true && movement.wallWasBroken == false) {
            uart2.println("keyへの移動開始");
            app.delay(Period);

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
            movement.move_1tile();

            if (homing.dijkstra(location.x, location.y, victim.keyX,
                                victim.keyY) == 0) {
                uart2.println("keyへの移動完了");
                movement.breakWall();
            }

            if (millis() > homing.HomingTime + 5000) {
                homing.homingReachedCount[location.x + FIELD_ORIGIN]
                                         [location.y + FIELD_ORIGIN]++;
            }
            app.delay(Period);
        } else {
            app.delay(Period);
        }
    }
}

void superTeamApp(App) {
    static bool homingStatus = false;
    static bool isFirst      = true;
    while (1) {
        if (servo.suspend == true && (location.x == 0) && (location.y == 0) &&
            (victim.isDetectedLetter ||
             victim.isDetectedColor) && isFirst == true && millis() > 90000) {  // NOTE:条件もっといい方法があるはず

            app.stop(victimNotifyApp);
            app.stop(rightWallApp);
            servo.suspend  = true;
            servo.velocity = 0;
            if (victim.isDetectedLetter == true) {
                uart2.write('L');
            } else {
                uart2.write('C');
            }
            while (1) {
                if (uart2.available() > 0) {
                    victim.kindOfVictim = uart2.read();
                    switch (victim.kindOfVictim) {
                        case 'H':
                            victim.keyX = victim.detectVictimX[H];
                            victim.keyY = victim.detectVictimY[H];
                            break;
                        case 'S':
                            victim.keyX = victim.detectVictimX[S];
                            victim.keyY = victim.detectVictimY[S];
                            break;
                        case 'U':
                            victim.keyX = victim.detectVictimX[U];
                            victim.keyY = victim.detectVictimY[U];
                            break;
                        case 'R':
                            victim.keyX = victim.detectVictimX[R];
                            victim.keyY = victim.detectVictimY[R];
                            break;
                        case 'G':
                            victim.keyX = victim.detectVictimX[G];
                            victim.keyY = victim.detectVictimY[G];
                            break;
                        case 'Y':
                            victim.keyX = victim.detectVictimX[Y];
                            victim.keyY = victim.detectVictimY[Y];
                            break;
                        default:
                            continue;
                    }
                    homing.started = true;
                    uart2.print(victim.keyX);
                    uart2.print(",");
                    uart2.println(victim.keyY);
                    isFirst = false;
                    break;
                }
                app.delay(Period);
            }
        }

        if ((movement.wallWasBroken == true) && (homingStatus == false)) {
            app.stop(homingApp);
            app.start(rightWallApp);
            app.start(locationApp);
            uart2.println("壁破壊Done");
            // servo.suspend = false;
            // servo.velocity = servo.DefaultSpeed;
            homingStatus = true;
        }

        if (floorSensor.frontColor == floorSensor.BLUE &&
            floorSensor.backColor ==
                floorSensor.BLUE) {  // FIXME:帰還条件を青に変えてね
            app.stop(rightWallApp);
            servo.suspend     = true;
            servo.velocity    = 0;
            victim.isDetected = true;
            app.delay(5000);
            victim.isDetected = false;
            if (movement.wallWasBroken == true) {
                servo.rescueKit(1, LEFT);
                app.delay(5000);
            } else {
                app.start(rightWallApp);
                app.delay(10000);
            }
            app.delay(Period);
        }
        app.delay(Period);
    }
}