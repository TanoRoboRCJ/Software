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
    exploring.reachedCount[FIELD_ORIGIN][FIELD_ORIGIN]++;
    exploring.updateMap();
    while (1) {
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
