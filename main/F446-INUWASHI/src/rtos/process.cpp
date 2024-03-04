/**
 * process.cpp
 * 得点走行中のみ起動するプロセスはここに記述する
 * モーターに関わるものはalgorithm.cppへどうぞ
 */

#include "./RTOS.h"

void locationApp(App) {
    while (1) {
        for (int i = 0; i < 100; i++) {
            location.updateOdometory();
            app.delay(location.Period);
            location.updateMap();
        }

        location.updateObservationData();
    }
}

void victimNotifyApp(App) {  // NOTE: ちょっとハードコードすぎるかな？
    victim.isRightOrLeft = NONE;
    while (1) {
        int rescueKitNum = 0;

        static int camTimer = 0;
        while (1) {
            if (victim.isRightOrLeft != NONE && ui.toggle == true) {
                while (victim.place[location.x + FIELD_ORIGIN]
                                   [location.y + FIELD_ORIGIN] == true) {//FIXME 座標の境目で2回検出される
                    victim.isRightOrLeft = NONE;
                    camera[0].data       = 'N';
                    camera[1].data       = 'N';
                }
                if ((victim.isRightOrLeft == RIGHT && tof.val[4] < 190 &&
                     tof.val[3] < 240 && tof.val[5] < 240) ||
                    (victim.isRightOrLeft == LEFT && tof.val[12] < 190 &&
                     tof.val[13] < 240 && tof.val[11] < 240)) {
                    break;
                } else {
                    victim.isRightOrLeft = NONE;
                    camera[0].data       = 'N';
                    camera[1].data       = 'N';
                }
            }
            app.delay(10);
        }

        app.stop(rightWallApp);
        app.stop(adjustmentApp);

        victim.place[location.x + FIELD_ORIGIN][location.y + FIELD_ORIGIN] =
            true;
        victim.isDetected = true;

        servo.velocity = 0;
        servo.suspend  = true;

        switch (victim.id) {
            case VICTIM_H:
                buzzer.sambaII();
                break;

            case VICTIM_S:
                buzzer.tokyoOndo();
                break;

            case VICTIM_U:
                buzzer.sakura();
                break;

            case VICTIM_RED:
                buzzer.shoten();
                break;

            case VICTIM_YELLOW:
                buzzer.shogun();
                break;

            case VICTIM_GREEN:
                buzzer.yuyake();
                break;

            default:
                break;
        }

        switch (victim.id) {
            case VICTIM_H:
                rescueKitNum = 3;
                break;

            case VICTIM_S:
                rescueKitNum = 2;
                break;

            case VICTIM_RED:
                rescueKitNum = 1;
                break;

            case VICTIM_YELLOW:
                rescueKitNum = 1;
                break;

            default:
                break;
        }

        app.delay(100);
        buzzer.rescueKit(rescueKitNum);
        servo.rescueKit(rescueKitNum, victim.isRightOrLeft);
        app.delay(100);

        servo.suspend  = false;
        servo.velocity = servo.DefaultSpeed;
        app.start(rightWallApp);
        app.start(adjustmentApp);

        victim.isDetected    = false;
        victim.id            = 0;
        victim.isRightOrLeft = NONE;
        camera[0].data       = 'N';
        camera[1].data       = 'N';

        camTimer = millis();
        while (1) {
            if (millis() - camTimer > 2500) {
                break;
            }
            app.delay(10);
            camera[0].flush();
            camera[1].flush();
        }

        victim.isDetected    = false;
        victim.id            = 0;
        victim.isRightOrLeft = NONE;
        camera[0].data       = 'N';
        camera[1].data       = 'N';
    }
}