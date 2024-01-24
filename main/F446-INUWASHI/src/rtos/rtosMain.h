#ifndef _RTOS_MAIN_H_
#define _RTOS_MAIN_H_

#include <Arduino.h>

#include "../device/device.h"
#include "../kit/RTOS-Kit.h"
#include "./algorithm.h"
#include "./rtosIO.h"
#include "./rtosLocation.h"
#include "./rtosVictim.h"
#include "./search.h"
#include "./uartUI.h"

int robotStaus = 0;

#define SPEED 100

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
                location.coordinateX = 0;
                location.coordinateY = 0;
                servo.suspend        = false;
                servo.velocity       = SPEED;
                status               = false;
            }
            runningWrite();
        } else {
            app.stop(rightWallApp);
            app.stop(adjustmentApp);
            app.stop(locationApp);

            servo.suspend  = true;
            servo.velocity = 0;
            status         = true;

            settingWrite();
        }

        app.delay(period);
    }
}

#endif