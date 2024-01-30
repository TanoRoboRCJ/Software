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
    exploring.updateMap();
    exploring
        .reachedCount[location.x + FIELD_ORIGIN][location.y + FIELD_ORIGIN]++;
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
        exploring.updateMap();
        exploring.reachedCount[location.x + FIELD_ORIGIN]
                              [location.y + FIELD_ORIGIN]++;
        app.delay(100);
    }
}

void adjustmentApp(
    App) {  // NOTE::ハードコードだからmovement.hへ分割を検討してもいいかも
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
            servo.velocity = servo.DefaultSpeed;
            app.start(servoApp);
            isHit = true;
        }
        app.delay(Period);
    }
}

void homingApp(App) {
    static bool isHoming = false;
    while (1) {
        if (homing.HomingTime < millis() && victim.isDetected == false) {
            isHoming = true;
            if (isHoming) {
                servo.suspend  = true;
                servo.velocity = 0;
                location.updateMap();
                app.stop(rightWallApp);
                app.stop(victimNotifyApp);
                isHoming = false;
            }
            switch (homing.compareLocation(location.x, location.y)) {
                case 0:
                    app.delay(500);
                    break;
                case 1:
                    movement.turnNorth();
                    movement.move_1tile();
                    break;
                case 2:
                    movement.turnEast();
                    movement.move_1tile();
                    break;
                case 3:
                    movement.turnSouth();
                    movement.move_1tile();
                    break;
                case 4:
                    movement.turnWest();
                    movement.move_1tile();
                    break;
                case 5:
                    exploring.maximumArray++;
                    app.delay(500);
                    break;
                default:
                    break;
            }
            exploring.maximumArray--;
            if ((location.x == 0) && (location.y == 0)) {
                servo.suspend  = true;
                servo.velocity = 0;
                buzzer.matsukenSamba();
            }
            app.delay(Period);
        } else {
            app.delay(Period);
        }
    }
}
