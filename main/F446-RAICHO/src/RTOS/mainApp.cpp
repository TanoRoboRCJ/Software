/**
 * mainApp.cpp
 * スイッチによる競技進行切り替え動作を行う
 */
#include "./RTOS.h"

void mainApp(App) {
    startDaemon();

    // CHECK: これstaticじゃなくてよくね？
    static bool status         = false;
    static bool hasGameStarted = false;
    static bool isFirst        = true;

    while (1) {
        if (ui.toggle && !isFirst) {
            if (hasGameStarted == false) {
                hasGameStarted = true;
                gyro.setOffset();
            }
            if (status) {
                app.start(servoApp);
                if (homing.started == true && homing.hasFinished == false) {
                    app.start(homingApp);
                } else {
                    app.start(rightWallApp);
                }
                app.start(adjustmentApp);
                app.start(locationApp);
                app.start(victimNotifyApp);
                app.start(homingApp);
                app.start(floorApp);
                app.start(superTeamApp);

                gyro.setLoPStart();

                servo.suspend  = false;
                servo.velocity = servo.DefaultSpeed;
                status         = false;
            }

        } else {
            if (!ui.toggle) {
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
            // victim.remainingRescueKitL = 6;
            // victim.remainingRescueKitR = 6;
            location.coordinateX       = 0.0;
            location.coordinateY       = 0.0;

            //             char _kindOfVictimX[FIELD_ORIGIN * 2][FIELD_ORIGIN *
            //             2] = {0};
            // char _kindOfVictimY[FIELD_ORIGIN * 2][FIELD_ORIGIN * 2] = {0};
            // all set to zero

            // for (int i = 0; i < FIELD_ORIGIN * 2; i++) {
            //     for (int j = 0; j < FIELD_ORIGIN * 2; j++) {
            //         victim._kindOfVictimX[i][j] = 0;
            //         victim._kindOfVictimY[i][j] = 0;
            //     }
            // }

            status = true;
        }

        app.delay(Period);
    }
}