#include "./rtos.h"
#include "./uartUI.h"

static double oldCoordinateX = 0;
static double oldCoordinateY = 0;

#define WAIT 500

void mainApp(App) {
    app.start(ledApp);
    app.start(sensorApp);
    app.start(monitorApp);
    app.start(servoApp);

    static bool status = false;

    while (1) {
        if (ui.toggle) {
            if (status) {
                app.start(rightWallApp);
                app.start(adjustmentApp);
                app.start(locationApp);
                app.start(victimNotifyApp);

                servo.suspend = false;
                servo.velocity = servo.DefaultSpeed;
                status = false;
            }
            runningWrite();

        } else {
            app.stop(rightWallApp);
            app.stop(adjustmentApp);
            app.stop(locationApp);

            servo.suspend = true;
            servo.velocity = 0;
            status = true;

            settingWrite();
        }

        app.delay(Period);
    }
}

void sensorApp(App) {
    while (1) {
        ui.read();
        gyro.read();
        tof.read();
        tof.calc(gyro.deg);

        victim.read();

        loadcell.read();

        floorSensor.setFloorColor(floorSensor.white);
        app.delay(2);
        floorSensor.redVal = analogRead(PC0);
        app.delay(2);
        floorSensor.blankVal = analogRead(PC0);
        app.delay(2);
        floorSensor.blueVal = analogRead(PC0);

        floorSensor.colorJudgment();
    }
}

void servoApp(App) {
    while (1) {
        if (!servo.suspend) {
            if (servo.isAngleCorrectionEnabled) {
                servo.drive(servo.velocity,
                            servo.angle + servo.isCorrectingAngle);
            } else {
                servo.driveAngularVelocity(servo.velocity,
                                           servo.angularVelocity);
            }
        } else {
            servo.stop();
        }
        servo.angle %= 360;
        app.delay(1);
    }
}

void ledApp(App) {
    int ledStatus = 0;
    int victimId = 0;
    unsigned long startTime = millis();
    for (int i = 0; i < 4; i++) {
        led.setColor(i, led.cyan);
        led.setBrightness(i, 255);
    }
    led.showAll();

    while (ui.toggle == 0) {
        app.delay(10);
    }

    while (1) {
        if (!victim.isDetected) {
            if (ledStatus != 1) {
                ledStatus = 1;
                led.setGlowColor();
                led.showAll();
            }
            victimId = 0;
        } else {
            ledStatus = 2;
            static bool blink = true;

            int brightness;
            if (blink) {
                brightness = 255;
            } else {
                brightness = 0;
            }

            for (int i = 0; i < 4; i++) {
                led.setBrightness(i, brightness);
                led.setColor(i, victim.color(victimId));
            }
            led.showAll();

            app.delay(500);
            blink = !blink;
        }

        app.delay(10);
    }
}

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

void monitorApp(App) {  // NOTE センサの値見たい時に使う。
    while (1) {
        // uart1.print(floorSensor.redVal);
        // uart1.print("\t");
        // uart1.print(floorSensor.blankVal);
        // uart1.print("\t");
        // uart1.print(floorSensor.blueVal);
        // uart1.print("\t");
        // uart1.print(floorSensor.Color);
        // uart1.println("\t");
        app.delay(Period);
    }
}

void victimNotifyApp(App) {
    victim.isRightOrLeft = 0;
    while (1) {
        int rescueKitNum = 0;

        static int camTimer = 0;
        while (1) {
            if (victim.isRightOrLeft != 0 && ui.toggle == true) {
                if (victim.place[location.x + 20][location.y + 20] == true) {
                    victim.isRightOrLeft = 0;
                    camera[0].data = 'N';
                    camera[1].data = 'N';
                } else if ((victim.isRightOrLeft == RIGHT && tof.val[4] < 190 &&
                            tof.val[3] < 240 && tof.val[5] < 240) ||
                           (victim.isRightOrLeft == LEFT && tof.val[12] < 190 &&
                            tof.val[13] < 240 && tof.val[11] < 240)) {
                    break;
                } else {
                    victim.isRightOrLeft = 0;
                    camera[0].data = 'N';
                    camera[1].data = 'N';
                }
            }
            app.delay(10);
        }

        app.stop(rightWallApp);

        victim.place[location.x + 20][location.y + 20] = true;
        victim.isDetected = true;

        servo.velocity = 0;
        servo.suspend = true;

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

        servo.suspend = false;
        servo.velocity = servo.DefaultSpeed;
        app.start(rightWallApp);

        camTimer = millis();
        while (1) {
            if (millis() - camTimer > 3000) {
                break;
            }
            app.delay(10);
            camera[0].flush();
            camera[1].flush();
        }

        victim.isDetected = false;
        victim.id = 0;
        victim.isRightOrLeft = 0;
        camera[0].data = 'N';
        camera[1].data = 'N';
    }
}