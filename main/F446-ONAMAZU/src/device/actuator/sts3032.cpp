#include "sts3032.h"

SMS_STS serialServo;

extern HardwareSerial uart1;

STS3032::STS3032(HardwareSerial *ptr) {
    serialPtr = ptr;
    serialPtr->begin(baudRate);
    serialServo.pSerial = serialPtr;

    for (int i = 1; i <= 4; i++) {
        serialServo.unLockEprom(i);
        serialServo.WheelMode(i);
        serialServo.EnableTorque(i, 1);
        serialServo.writeByte(i, SMS_STS_MODE, 1);
        serialServo.LockEprom(i);
    }
}

void STS3032::directDrive(int id, int percent, int acceleration) {
    int sendData;
    sendData = percent * maximumSpeed / 100;
    sendData = constrain(sendData, -maximumSpeed, maximumSpeed);

    serialServo.WriteSpe(id + 1, sendData, acceleration);
}

void STS3032::driveAngularVelocity(int velocity, int angularVelocity) {
    int data[2];
    data[0] = angularVelocity - velocity;
    data[1] = angularVelocity + velocity;

    if (gyro.slope < -15) {
        data[0] *= SlopeSpeed;
        data[1] *= SlopeSpeed;
    }

    rightWheelSpeed = -data[0];
    leftWheelSpeed = data[1];

    for (int i = 0; i < 2; i++) {
        data[i] = constrain(data[i], -100, 100);
    }

    for (int i = 0; i < 2; i++) {
        directDrive(i, data[0]);
    }

    for (int i = 2; i < 4; i++) {
        directDrive(i, data[1]);
    }
}

void STS3032::drive(int velocity, int angle) {
    const double Kp = -3.4;

    // 0-360変換
    while (angle < 0) {
        angle += 360;
    }
    angle %= 360;

    int angularVelocity = gyro.deg - angle;

    //-180から180変換
    while (angularVelocity < 0) {
        angularVelocity += 360;
    }
    if (angularVelocity > 180) {
        angularVelocity -= 360;
    }

    if (abs(angularVelocity) > 40) {
        angularVelocity *= Kp;
        driveAngularVelocity(0, angularVelocity);
    } else {
        angularVelocity *= Kp;
        driveAngularVelocity(velocity, angularVelocity);
    }
}

void STS3032::stop(void) {
    driveAngularVelocity(0, 0);
}

void STS3032::rescueKit(int num, int position) {
    static int remainingRescueKitR = 2;
    static int remainingRescueKitL = 2;

    int deg = gyro.deg;
    int turnDeg = (deg + 180) % 360;
    bool turn = false;

    app.stop(servoApp);

    for (int i = 0; i < num; i++) {
        if (position == 0) {
            if (remainingRescueKitR == 0 && turn == false) {
                position = 2;
                turn = true;

                while (abs(gyro.deg - turnDeg) > 5 &&
                       abs(gyro.deg - turnDeg) < 355) {
                    drive(0, turnDeg);
                    app.delay(10);
                }
            }
        } else if (position == 2) {
            if (remainingRescueKitL == 0 && turn == false) {
                position = 0;
                turn = true;

                while (abs(gyro.deg - turnDeg) > 5 &&
                       abs(gyro.deg - turnDeg) < 355) {
                    drive(0, turnDeg);
                    app.delay(10);
                }
            }
        }
        driveAngularVelocity(0, 0);

        if (position == 0) {
            bottom.rescueKit[0] = false;
            app.delay(400);
            bottom.rescueKit[0] = true;
            app.delay(200);

            remainingRescueKitR--;
        } else if (position == 2) {
            bottom.rescueKit[1] = false;
            app.delay(400);
            bottom.rescueKit[1] = true;
            app.delay(200);

            remainingRescueKitL--;
        }
    }

    while (abs(gyro.deg - deg) > 5 && abs(gyro.deg - deg) < 355) {
        drive(0, deg);
        app.delay(10);
    }
    driveAngularVelocity(0, 0);

    app.start(servoApp);
}