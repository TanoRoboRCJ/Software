/**
 * process.cpp
 * 得点走行中のみ起動するプロセスはここに記述する
 * モーターに関わるものはalgorithm.cppへどうぞ
 */

#include "./RTOS.h"

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

bool duplicate(void);

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

        while (1) {
            if (victim.isRightOrLeft != NONE && ui.toggle == true) {
                if (duplicate() ||
                    victim.place[location.x + FIELD_ORIGIN]
                                [location.y + FIELD_ORIGIN] == true) {
                    victim.isRightOrLeft = NONE;
                    camera[0].data       = 'N';
                    camera[1].data       = 'N';
                } else if ((victim.isRightOrLeft == RIGHT && tof.val[4] < 190 &&
                            tof.val[3] < 240) ||
                           (victim.isRightOrLeft == LEFT && tof.val[12] < 190 &&
                            tof.val[13] < 240)) {
                    break;
                } else {
                    victim.isRightOrLeft = NONE;
                    camera[0].data       = 'N';
                    camera[1].data       = 'N';
                }
            }
            app.delay(10);
        }

        if (homing.started == true) {
            app.stop(homingApp);
        } else {
            app.stop(rightWallApp);
        }
        app.stop(adjustmentApp);
        app.stop(floorApp);

        victim.kindOfVictim[location.x + FIELD_ORIGIN]
                           [location.y + FIELD_ORIGIN] = victim.id;
        victim.place[location.x + FIELD_ORIGIN][location.y + FIELD_ORIGIN] =
            true;
        victim.isDetected = true;

        servo.suspend  = true;
        servo.velocity = 0;

        buzzer.bpm = 120;
        buzzer.beat(FA_, 0.5);

        app.delay(5000);

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

        buzzer.rescueKit(rescueKitNum);
        servo.rescueKit(rescueKitNum, victim.isRightOrLeft);
        app.delay(100);

        servo.suspend  = false;
        servo.velocity = servo.DefaultSpeed;
        if (homing.started == true) {
            app.start(homingApp);
        } else {
            app.start(rightWallApp);
        }
        app.start(adjustmentApp);
        app.start(floorApp);
        int camTimer = millis();
        while (millis() - camTimer < 1000) {
            victim.isDetected    = false;
            victim.id            = 0;
            victim.isRightOrLeft = NONE;
            camera[0].data       = 'N';
            camera[1].data       = 'N';
        }

        victim.isDetected    = false;
        victim.id            = 0;
        victim.isRightOrLeft = NONE;
        camera[0].data       = 'N';
        camera[1].data       = 'N';
    }
}

bool duplicate(void) {  // 進行方向に今見ているデータと同じデータがあれば破棄
    if (gyro.direction == NORTH || gyro.direction == SOUTH) {
        if (victim.kindOfVictim[location.x + FIELD_ORIGIN]
                               [location.y + FIELD_ORIGIN + 1] == victim.id ||
            victim.kindOfVictim[location.x + FIELD_ORIGIN]
                               [location.y + FIELD_ORIGIN - 1] == victim.id) {
            return true;
        } else {
            return false;
        }
    } else if (gyro.direction == EAST || gyro.direction == WEST) {
        if (victim.kindOfVictim[location.x + FIELD_ORIGIN + 1]
                               [location.y + FIELD_ORIGIN] == victim.id ||
            victim.kindOfVictim[location.x + FIELD_ORIGIN - 1]
                               [location.y + FIELD_ORIGIN] == victim.id) {
            return true;
        } else {
            return false;
        }
    }else{
        return false;
    }
}