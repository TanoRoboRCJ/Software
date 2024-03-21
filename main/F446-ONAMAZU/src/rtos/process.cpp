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
        for (int i = 0; i < 25; i++) {
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
                if (duplicate() == true) {
                    victim.isRightOrLeft = NONE;
                    camera[0].data       = 'N';
                    camera[1].data       = 'N';
                } else if ((victim.isRightOrLeft == RIGHT && tof.val[4] < 190 &&
                            tof.lidarRightWallExists) ||
                           (victim.isRightOrLeft == LEFT && tof.val[12] < 190 &&
                            tof.lidarLeftWallExists)) {
                    break;
                } else {
                    victim.isRightOrLeft = NONE;
                    camera[0].data       = 'N';
                    camera[1].data       = 'N';
                }
            }
            app.delay(10);
        }

        if (homing.started == true && homing.hasFinished == false) {
            app.stop(homingApp);
        } else {
            app.stop(rightWallApp);
        }
        app.stop(adjustmentApp);
        app.stop(floorApp);

        victim.isDetected = true;

        servo.suspend  = true;
        servo.velocity = 0;
        servo.driveAngularVelocity(0, 0);

        buzzer.bpm = 120;
        buzzer.beat(FA_, 0.1);

        unsigned long stopTimer = millis();
        while (millis() - stopTimer < 5000) {
            servo.driveAngularVelocity(0, 0);
            app.delay(100);
        }
        
        if (gyro.direction == NORTH || gyro.direction == SOUTH) {
            switch (victim.id) {
                case VICTIM_H:
                    victim.setKindOfvictimY(location.x + FIELD_ORIGIN,
                                            location.y + FIELD_ORIGIN, H);
                    break;
                case VICTIM_S:
                    victim.setKindOfvictimY(location.x + FIELD_ORIGIN,
                                            location.y + FIELD_ORIGIN, S);
                    break;
                case VICTIM_U:
                    victim.setKindOfvictimY(location.x + FIELD_ORIGIN,
                                            location.y + FIELD_ORIGIN, U);
                    break;
                case VICTIM_RED:
                    victim.setKindOfvictimY(location.x + FIELD_ORIGIN,
                                            location.y + FIELD_ORIGIN, R);
                    break;
                case VICTIM_YELLOW:
                    victim.setKindOfvictimY(location.x + FIELD_ORIGIN,
                                            location.y + FIELD_ORIGIN, Y);
                    break;
                case VICTIM_GREEN:
                    victim.setKindOfvictimY(location.x + FIELD_ORIGIN,
                                            location.y + FIELD_ORIGIN, G);
                    break;
            }
        } else if (gyro.direction == EAST || gyro.direction == WEST) {
            switch (victim.id) {
                case VICTIM_H:
                    victim.setKindOfvictimX(location.x + FIELD_ORIGIN,
                                            location.y + FIELD_ORIGIN, H);
                    break;
                case VICTIM_S:

                    victim.setKindOfvictimX(location.x + FIELD_ORIGIN,
                                            location.y + FIELD_ORIGIN, S);
                    break;
                case VICTIM_U:

                    victim.setKindOfvictimX(location.x + FIELD_ORIGIN,
                                            location.y + FIELD_ORIGIN, U);
                    break;
                case VICTIM_RED:

                    victim.setKindOfvictimX(location.x + FIELD_ORIGIN,
                                            location.y + FIELD_ORIGIN, R);
                    break;
                case VICTIM_YELLOW:

                    victim.setKindOfvictimX(location.x + FIELD_ORIGIN,
                                            location.y + FIELD_ORIGIN, Y);
                    break;
                case VICTIM_GREEN:
                    victim.setKindOfvictimX(location.x + FIELD_ORIGIN,
                                            location.y + FIELD_ORIGIN, G);
                    break;
            }
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

        buzzer.rescueKit(rescueKitNum);
        servo.rescueKit(rescueKitNum, victim.isRightOrLeft);
        app.delay(100);

        if (homing.started == true && homing.hasFinished == false) {
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
        switch (victim.id) {
            case VICTIM_H:
                if (victim.returnKindOfvictimY(location.x + FIELD_ORIGIN,
                                        location.y + FIELD_ORIGIN, H) ||
                    victim.returnKindOfvictimY(location.x + FIELD_ORIGIN,
                                        location.y + FIELD_ORIGIN - 1, H) ||
                    victim.returnKindOfvictimY(location.x + FIELD_ORIGIN,
                                        location.y + FIELD_ORIGIN + 1, H)) {
                    return true;
                }
                break;
            case VICTIM_S:
                if (victim.returnKindOfvictimY(location.x + FIELD_ORIGIN,
                                        location.y + FIELD_ORIGIN, S) ||
                    victim.returnKindOfvictimY(location.x + FIELD_ORIGIN,
                                        location.y + FIELD_ORIGIN - 1, S) ||
                    victim.returnKindOfvictimY(location.x + FIELD_ORIGIN,
                                        location.y + FIELD_ORIGIN + 1, S)) {
                    return true;
                }
                break;
            case VICTIM_U:
                if (victim.returnKindOfvictimY(location.x + FIELD_ORIGIN,
                                        location.y + FIELD_ORIGIN, U) ||
                    victim.returnKindOfvictimY(location.x + FIELD_ORIGIN,
                                        location.y + FIELD_ORIGIN - 1, U) ||
                    victim.returnKindOfvictimY(location.x + FIELD_ORIGIN,
                                        location.y + FIELD_ORIGIN + 1, U)) {
                    return true;
                }
                break;
            case VICTIM_RED:
                if (victim.returnKindOfvictimY(location.x + FIELD_ORIGIN,
                                        location.y + FIELD_ORIGIN, R) ||
                    victim.returnKindOfvictimY(location.x + FIELD_ORIGIN,
                                        location.y + FIELD_ORIGIN - 1, R) ||
                    victim.returnKindOfvictimY(location.x + FIELD_ORIGIN,
                                        location.y + FIELD_ORIGIN + 1, R)) {
                    return true;
                }
                break;
            case VICTIM_YELLOW:
                if (victim.returnKindOfvictimY(location.x + FIELD_ORIGIN,
                                        location.y + FIELD_ORIGIN, Y) ||
                    victim.returnKindOfvictimY(location.x + FIELD_ORIGIN,
                                        location.y + FIELD_ORIGIN - 1, Y) ||
                    victim.returnKindOfvictimY(location.x + FIELD_ORIGIN,
                                        location.y + FIELD_ORIGIN + 1, Y)) {
                    return true;
                }
                break;
            case VICTIM_GREEN:
                if (victim.returnKindOfvictimY(location.x + FIELD_ORIGIN,
                                        location.y + FIELD_ORIGIN, G) ||
                    victim.returnKindOfvictimY(location.x + FIELD_ORIGIN,
                                        location.y + FIELD_ORIGIN - 1, G) ||
                    victim.returnKindOfvictimY(location.x + FIELD_ORIGIN,
                                        location.y + FIELD_ORIGIN + 1, G)) {
                    return true;
                }
                break;
        }
    } else if (gyro.direction == EAST || gyro.direction == WEST) {
        switch (victim.id) {
            case VICTIM_H:
                if (victim.returnKindOfvictimX(location.x + FIELD_ORIGIN,
                                        location.y + FIELD_ORIGIN, H) ||
                    victim.returnKindOfvictimX(location.x + FIELD_ORIGIN - 1,
                                        location.y + FIELD_ORIGIN, H) ||
                    victim.returnKindOfvictimX(location.x + FIELD_ORIGIN + 1,
                                        location.y + FIELD_ORIGIN, H)) {
                    return true;
                }
                break;
            case VICTIM_S:
                if (victim.returnKindOfvictimX(location.x + FIELD_ORIGIN,
                                        location.y + FIELD_ORIGIN, S) ||
                    victim.returnKindOfvictimX(location.x + FIELD_ORIGIN - 1,
                                        location.y + FIELD_ORIGIN, S) ||
                    victim.returnKindOfvictimX(location.x + FIELD_ORIGIN + 1,
                                        location.y + FIELD_ORIGIN, S)) {
                    return true;
                }
                break;

            case VICTIM_U:
                if (victim.returnKindOfvictimX(location.x + FIELD_ORIGIN,
                                        location.y + FIELD_ORIGIN, U) ||
                    victim.returnKindOfvictimX(location.x + FIELD_ORIGIN - 1,
                                        location.y + FIELD_ORIGIN, U) ||
                    victim.returnKindOfvictimX(location.x + FIELD_ORIGIN + 1,
                                        location.y + FIELD_ORIGIN, U)) {
                    return true;
                }
                break;
            case VICTIM_RED:
                if (victim.returnKindOfvictimX(location.x + FIELD_ORIGIN,
                                        location.y + FIELD_ORIGIN, R) ||
                    victim.returnKindOfvictimX(location.x + FIELD_ORIGIN - 1,
                                        location.y + FIELD_ORIGIN, R) ||
                    victim.returnKindOfvictimX(location.x + FIELD_ORIGIN + 1,
                                        location.y + FIELD_ORIGIN, R)) {
                    return true;
                }
                break;
            case VICTIM_YELLOW:
                if (victim.returnKindOfvictimX(location.x + FIELD_ORIGIN,
                                        location.y + FIELD_ORIGIN, Y) ||
                    victim.returnKindOfvictimX(location.x + FIELD_ORIGIN - 1,
                                        location.y + FIELD_ORIGIN, Y) ||
                    victim.returnKindOfvictimX(location.x + FIELD_ORIGIN + 1,
                                        location.y + FIELD_ORIGIN, Y)) {
                    return true;
                }
                break;
            case VICTIM_GREEN:
                if (victim.returnKindOfvictimX(location.x + FIELD_ORIGIN,
                                        location.y + FIELD_ORIGIN, G) ||
                    victim.returnKindOfvictimX(location.x + FIELD_ORIGIN - 1,
                                        location.y + FIELD_ORIGIN, G) ||
                    victim.returnKindOfvictimX(location.x + FIELD_ORIGIN + 1,
                                        location.y + FIELD_ORIGIN, G)) {
                    return true;
                }
                break;
        }
    }

    return false;
}