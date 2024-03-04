/**
 * daemon.cpp
 * 常に起動しているappを格納。ここにあるappはいかなる場合も停止されることはない。*
 */

#include "../algorithm/exploring.h"
#include "../algorithm/homing.h"
#include "../process/process.h"
#include "./RTOS.h"

void startDaemon(void) {
    app.start(monitorApp);
    app.start(sensorApp);
    app.start(servoApp);
    app.start(ledApp);
}

void monitorApp(App) {
    while (1) {
        uart3.print(tof.val[2]);
        uart3.print("\t");
        uart3.print(tof.val[4]);
        uart3.print("\t");
        uart3.print(tof.val[6]);
        uart3.println("\t");
        app.delay(10);
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
    int ledStatus           = 0;
    int victimId            = 0;
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
            ledStatus         = 2;
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