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
        //tof

        // floor
        uart1.print("floor: ");
        uart1.print(floorSensor.frontCRGB[0]);
        uart1.print("\t");
        uart1.print(floorSensor.frontCRGB[1]);
        uart1.print("\t");
        uart1.print(floorSensor.frontCRGB[2]);
        uart1.print("\t");
        uart1.print(floorSensor.frontCRGB[3]);

        uart1.print("\t");

        uart1.print(floorSensor.backCRGB[0]);
        uart1.print("\t");
        uart1.print(floorSensor.backCRGB[1]);
        uart1.print("\t");
        uart1.print(floorSensor.backCRGB[2]);
        uart1.print("\t");
        uart1.print(floorSensor.backCRGB[3]);

        uart1.print("\t");

        uart1.print("Color of floor: ");
        switch(floorSensor.frontColor){
            case 0:
                uart1.print("white");
                break;
            case 1:
                uart1.print("black");
                break;
            case 2:
                uart1.print("Blue");
                break;
        }

        uart1.print("\t");

        switch(floorSensor.backColor){
            case 0:
                uart1.print("white");
                break;
            case 1:
                uart1.print("black");
                break;
            case 2:
                uart1.print("Blue");
                break;
        }

        // gyro
        // uart1.print("gyro: ");
        // uart1.print(gyro.deg);

        // location
        // uart1.print(location.x);
        // uart1.print(",");
        // uart1.print(location.y);

        uart1.println();
        app.delay(Period);
    }
}

void sensorApp(App) {
    while (1) {
        bottom.read();
        bottom.write();

        ui.read();

        gyro.read();

        tof.read();
        tof.calc(gyro.deg);

        victim.read();
        loadcell.read();

        floorSensor.read();

        app.delay(10);
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

    for (int i = 0; i < 3; i++) {
        led.setColor(i, led.white);
        led.setBrightness(i, 255);
    }
    led.showAll();

    while (ui.toggle == 0) {
        app.delay(100);
    }

    while (1) {
        if (victim.isDetected) {
            int blink = ((millis() / 200) % 5 == 0) * 255;

            for (int i = 0; i < 3; i++) {
                led.setBrightness(i, blink);
                led.setColor(i, victim.color(victimId));
            }
            led.showAll();

            app.delay(10);
        } else {
            for (int i = 0; i < 3; i++) {
                led.setBrightness(i, 0);
                led.setColor(i, led.white);
            }
            led.showAll();

            app.delay(10);
        }

        app.delay(10);
    }
}