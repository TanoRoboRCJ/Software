/**
 * mainApp.cpp
 * スイッチによる競技進行切り替え動作を行う
 */
#include "./RTOS.h"

void mainApp(App) {
    startDaemon();

    static bool status         = false;
    static bool hasGameStarted = false;
    static bool isFirst        = true;

    while (1) {
        if (ui.toggle  && !isFirst) {
            if (hasGameStarted == false) {
                hasGameStarted = true;
                gyro.setOffset();
            }
            if (status) {
                app.start(servoApp);
                if (homing.started == true) {
                    app.start(homingApp);
                } else {
                    app.start(rightWallApp);
                }
                app.start(adjustmentApp);
                app.start(locationApp);
                app.start(victimNotifyApp);
                app.start(homingApp);
                app.start(floorApp);

                servo.suspend  = false;
                servo.velocity = servo.DefaultSpeed;
                status         = false;
            }
            lcd.writeRunningStatus();

        } else {
            if(!ui.toggle){
                isFirst = false;
            }
            
            app.stop(homingApp);
            app.stop(rightWallApp);
            app.stop(adjustmentApp);
            app.stop(locationApp);
            app.stop(floorApp);

            servo.suspend  = true;
            servo.velocity = 0;
            servo.driveAngularVelocity(0, 0);
            // location.coordinateX = floorSensor.checkPointX * 300;
            // location.coordinateY = floorSensor.checkPointY * 300;
            status = true;

            lcd.begin();
        }

        app.delay(Period);
    }
}