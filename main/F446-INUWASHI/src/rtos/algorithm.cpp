/**
 * algorithm.cpp
 * 探索・帰還などのアルゴリズム的な動作
 * もうちょっといいファイル名があると思うので思いついたら変えてください
 */

#include "./RTOS.h"

static double oldCoordinateX = 0;
static double oldCoordinateY = 0;

#define WAIT 500

void rightWallApp(App) {
    updateMap();  // 初期状態を記録。以降は停止時に更新
    reachedCount[location.x + MAP_ORIGIN][location.y + MAP_ORIGIN]++;
    while (1) {
        app.delay(Period);
        servo.suspend = true;
        servo.velocity = 0;

        switch (weighting()) {
            case 0:  // right
                turnRight();
                break;
            case 1:  // front
                break;
            case 2:  // left
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
            servo.velocity = servo.DefaultSpeed;
            app.start(servoApp);
            isHit = true;
        }
        app.delay(Period);
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
        servo.suspend = false;
        servo.velocity = servo.DefaultSpeed;
        app.delay(Period);
    }  // 次のタイルまで前進
    servo.suspend = true;
    servo.velocity = 0;
}
