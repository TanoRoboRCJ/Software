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
        // if (floorSensor.backColor == floorSensor.BLACK) {
        //     app.stop(rightWallApp);
        //     servo.suspend  = true;
        //     servo.velocity = 0;
        //     app.delay(5000);
        //     servo.suspend = false;
        //     app.start(rightWallApp);
        //     int tempX = location.x;
        //     int tempY = location.y;

        //     while (1) {
        //         if (location.x != tempX || location.y != tempY) {
        //             break;
        //         }
        //         app.delay(Period);
        //     }

        //     app.delay(1400);
        // }

        // if (floorSensor.backColor == floorSensor.BLUE) {
        //     app.stop(rightWallApp);
        //     servo.suspend  = true;
        //     servo.velocity = 0;
        //     app.delay(5000);
        //     servo.suspend = false;
        //     app.start(rightWallApp);

        //     int tempX = location.x;
        //     int tempY = location.y;

        //     while (1) {
        //         if (location.x != tempX || location.y != tempY) {
        //             break;
        //         }
        //         app.delay(Period);
        //     }

        //     app.delay(1400);
        // }
        app.delay(Period);
    }
}