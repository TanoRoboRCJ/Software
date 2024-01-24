// FIXME: 汚すぎる
#include "location.h"

extern HardwareSerial uart1;

Location::Location(/* args */) {
}

void Location::updateOdometory(void) {
    double vec = (servo.rightWheelSpeed + servo.leftWheelSpeed) / 2.0;
    double vecX = vec * sin(radians(gyro.deg));
    double vecY = vec * cos(radians(gyro.deg));

    coordinateX += vecX * Period * _VelocityConstant * cos(radians(gyro.slope));
    coordinateY += vecY * Period * _VelocityConstant * cos(radians(gyro.slope));
}

void Location::updateObservationData(void) {
    static bool trustX = false;
    static bool trustY = false;

    int sensorX = 0;
    int sensorY = 0;

    int minY = 360;  // 0°に一番近いセンサの誤差

    for (int i = 0; i < 16; i++) {
        if (min(abs((360 - round(i * 22.5)) - gyro.deg),
                360 - abs((360 - round(i * 22.5)) - gyro.deg)) < minY) {
            minY = min(abs((360 - round(i * 22.5)) - gyro.deg),
                       360 - abs((360 - round(i * 22.5)) - gyro.deg));
            IndexOfSensorFacingNorth = i;
        }
    }

    static int widthY = 0;
    static int widthX = 0;

    widthX = abs(tof.vecX[(IndexOfSensorFacingNorth + 4) % 16]) +
             abs(tof.vecX[(IndexOfSensorFacingNorth + 12) % 16]) + 55;
    widthY = abs(tof.vecY[IndexOfSensorFacingNorth]) +
             abs(tof.vecY[(IndexOfSensorFacingNorth + 8) % 16]) + 55;

    trustX = false;
    trustY = false;

    if (290 < widthX && widthX < 310) {
        trustX = true;
        coordinateX = round(coordinateX / 300.0) * 300.0 +
                      abs(tof.vecX[(IndexOfSensorFacingNorth + 12) % 16]) - 130;

        lastCorrection = millis();
        lastTrustX = millis();
    }
    if (590 < widthX && widthX < 610) {
        trustX = true;

        int temp;

        if (abs(tof.vecX[(IndexOfSensorFacingNorth + 4) % 16]) <
            abs(tof.vecX[(IndexOfSensorFacingNorth + 12) % 16])) {
            temp = round(coordinateX / 300.0) * 300.0 +
                   (300 - abs(tof.vecX[(IndexOfSensorFacingNorth + 3) % 16]) -
                    40) -
                   130;
        } else {
            temp = round(coordinateX / 300.0) * 300.0 +
                   abs(tof.vecX[(IndexOfSensorFacingNorth + 12) % 16]) - 130;
        }

        if (abs(coordinateX - temp) < 150) {
            coordinateX = temp;
        }

        lastCorrection = millis();
        lastTrustX = millis();
    }

    if (290 < widthY && widthY < 310) {
        trustY = true;
        coordinateY = round(coordinateY / 300.0) * 300.0 +
                      abs(tof.vecY[(IndexOfSensorFacingNorth + 8) % 16]) - 130;
        lastCorrection = millis();
        lastTrustY = millis();
    }

    if (590 < widthY && widthY < 610) {
        trustY = true;

        int temp;

        if (abs(tof.vecY[IndexOfSensorFacingNorth]) <
            abs(tof.vecY[(IndexOfSensorFacingNorth + 8) % 16])) {
            temp = round(coordinateY / 300.0) * 300.0 +
                   (300 - abs(tof.vecY[IndexOfSensorFacingNorth]) - 40) - 130;
        } else {
            temp = round(coordinateY / 300.0) * 300.0 +
                   abs(tof.vecY[(IndexOfSensorFacingNorth + 8) % 16]) - 130;
        }

        if (abs(coordinateY - temp) < 150) {
            coordinateY = temp;
        }

        lastCorrection = millis();
        lastTrustY = millis();
    }

    x = round(coordinateX / 300.0);
    y = round(coordinateY / 300.0);
}

void Location::updateMap(void) {
    int tempX = constrain(x + MAP_ORIGIN, 3, MAP_ORIGIN * 2 - 3);

    int tempY = constrain(y + MAP_ORIGIN, 3, MAP_ORIGIN * 2 - 3);

    if (mapData[tempX][tempY].isPassed == false) {
        mapData[tempX][tempY].isPassed = true;
        mapData[tempX][tempY].isDetected = true;
        mapData[tempX][tempY].firstPassedTime = millis();
    }

    if ((millis() - mapData[tempX][tempY].firstPassedTime) < 3000 &&
        (millis() - mapData[tempX][tempY].firstPassedTime) > 1000 &&
        abs(gyro.slope) == 0) {
        int judgeGain[2] = {400, 240};

        if (millis() - lastTrustY <= 20000) {
            if (tof.val[IndexOfSensorFacingNorth] > judgeGain[0] &&
                tof.val[(IndexOfSensorFacingNorth + 1) % 16] > judgeGain[1] &&
                tof.val[(IndexOfSensorFacingNorth + 15) % 16] > judgeGain[1]) {
                mapData[tempX][tempY + 1].isDetected = true;
            }

            if (tof.val[(IndexOfSensorFacingNorth + 6) % 16] > judgeGain[0] &&
                tof.val[(IndexOfSensorFacingNorth + 5) % 16] > judgeGain[1] &&
                tof.val[(IndexOfSensorFacingNorth + 7) % 16] > judgeGain[1]) {
                mapData[tempX][tempY - 1].isDetected = true;
            }
        }

        if (millis() - lastTrustX <= 20000) {
            if (tof.val[(IndexOfSensorFacingNorth + 3) % 16] > judgeGain[0] &&
                tof.val[(IndexOfSensorFacingNorth + 4) % 16] > judgeGain[1] &&
                tof.val[(IndexOfSensorFacingNorth + 2) % 16] > judgeGain[1]) {
                mapData[tempX + 1][tempY].isDetected = true;
            }
            if (tof.val[(IndexOfSensorFacingNorth + 9) % 16] > judgeGain[0] &&
                tof.val[(IndexOfSensorFacingNorth + 10) % 16] > judgeGain[1] &&
                tof.val[(IndexOfSensorFacingNorth + 8) % 16] > judgeGain[1]) {
                mapData[tempX - 1][tempY].isDetected = true;
            }
        }
    }
}