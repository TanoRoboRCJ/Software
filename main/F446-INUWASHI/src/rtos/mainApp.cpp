/**
 * mainApp.cpp
 * スイッチによる競技進行切り替え動作を行う
 */
#include "./RTOS.h"

void mainApp(App) {
    startDaemon();

    static bool status = false;

    while (1) {
        if (ui.toggle) {
            if (status) {
                app.start(rightWallApp);
                app.start(adjustmentApp);
                app.start(locationApp);
                app.start(victimNotifyApp);
                app.start(homingApp);

                servo.suspend = false;
                servo.velocity = servo.DefaultSpeed;
                status = false;
            }
            lcd.writeRunningStatus();

        } else {
            app.stop(homingApp);
            app.stop(rightWallApp);
            app.stop(adjustmentApp);
            app.stop(locationApp);

            servo.suspend = true;
            servo.velocity = 0;
            status = true;

            lcd.begin();
        }

        app.delay(Period);
    }
}