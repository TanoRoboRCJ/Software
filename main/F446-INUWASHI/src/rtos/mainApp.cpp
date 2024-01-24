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

                servo.suspend = false;
                servo.velocity = servo.DefaultSpeed;
                status = false;
            }
            UI_runningWrite();

        } else {
            app.stop(rightWallApp);
            app.stop(adjustmentApp);
            app.stop(locationApp);

            servo.suspend = true;
            servo.velocity = 0;
            status = true;

            UI_settingWrite();
        }

        app.delay(Period);
    }
}