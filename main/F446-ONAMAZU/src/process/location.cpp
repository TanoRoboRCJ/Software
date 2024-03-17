#include "location.h"

extern HardwareSerial uart1;

Location::Location(/* args */) {
}

void Location::updateOdometory(void) {
    static unsigned long lastTime = millis();

    double vec = (servo.rightWheelSpeed + servo.leftWheelSpeed) / 2.0;
    double vecX = vec * sin(radians(gyro.deg));
    double vecY = vec * cos(radians(gyro.deg));

    coordinateX += vecX * constrain((millis() - lastTime), 0, 20) *
                   _VelocityConstant * cos(radians(gyro.slope));
    coordinateY += vecY * constrain((millis() - lastTime), 0, 20) *
                   _VelocityConstant * cos(radians(gyro.slope));

    lastTime = millis();
}

bool Location::canGo(int x1, int y1, int x2, int y2) {
    if (x1 == x2) {
        if (y1 < y2) {  // y2が北側
            return !wall[x1][y2].horizontal;
        } else {  // y2が南側
            return !wall[x1][y1].horizontal;
        }
    } else if (y1 == y2) {
        if (x1 < x2) {  // x2が東側
            return !wall[x2][y1].vertical;
        } else {  // x2が西側
            return !wall[x1][y1].vertical;
        }
    }

    return false;
}

void Location::updateObservationData(void) {
    bool trustX = false;
    bool trustY = false;

    int sensorX = 0;
    int sensorY = 0;

    static int widthY = 0;
    static int widthX = 0;

    const int SensorRadius = 25;

    // 北か南　誤差10°
    const int allowanceDegError = 10;
    const int allowanceWidthError = 10;
    const int range = 3;  // 信用するマス数

    if ((gyro.deg < 0 + allowanceDegError ||
         gyro.deg > 360 - allowanceDegError) ||
        (180 - allowanceDegError < gyro.deg &&
         gyro.deg < 180 + allowanceDegError)) {
        bool isNorth = (gyro.deg < 0 + allowanceDegError ||
                        gyro.deg > 360 - allowanceDegError);
        // width
        widthX = abs(tof.vecX[4]) + abs(tof.vecX[12]) + SensorRadius * 2;
        widthY = abs(tof.vecY[0]) + abs(tof.vecY[8]) + 76 + 30 + SensorRadius;

        for (int i = 1; i <= range; i++) {
            if ((i * 300 - allowanceWidthError) < widthX &&
                widthX < (i * 300 + allowanceWidthError)) {
                trustX = true;
                int oldCoordinateX = coordinateX;
                coordinateX = round(coordinateX / 300.0) * 300.0;

                if (isNorth) {
                    coordinateX +=
                        (abs(tof.vecX[12]) % 300) - (150 - SensorRadius);
                } else {
                    coordinateX +=
                        (abs(tof.vecX[4]) % 300) - (150 - SensorRadius);
                }

                while (abs(coordinateX - oldCoordinateX) > 150) {
                    if (coordinateX - oldCoordinateX > 150) {
                        coordinateX -= 300;
                    } else if (coordinateX - oldCoordinateX < -150) {
                        coordinateX += 300;
                    }
                }
                break;
            }
        }

        for (int i = 1; i <= range; i++) {
            if ((i * 300 - allowanceWidthError) < widthY &&
                widthY < (i * 300 + allowanceWidthError)) {
                trustY = true;

                int oldCoordinateY = coordinateY;
                coordinateY = round(coordinateY / 300.0) * 300.0;

                if (isNorth) {
                    coordinateY += (abs(tof.vecY[8]) % 300) - (150 - 76);
                } else {
                    coordinateY +=
                        (abs(tof.vecY[0]) % 300) - (150 - SensorRadius - 30);
                }

                while (abs(coordinateY - oldCoordinateY) > 150) {
                    if (coordinateY - oldCoordinateY > 150) {
                        coordinateY -= 300;
                    } else if (coordinateY - oldCoordinateY < -150) {
                        coordinateY += 300;
                    }
                }

                break;
            }
        }

    } else if ((90 - allowanceDegError < gyro.deg &&
                gyro.deg < 90 + allowanceDegError) ||
               (270 - allowanceDegError < gyro.deg &&
                gyro.deg < 270 + allowanceDegError)) {
        bool isEast = (90 - allowanceDegError < gyro.deg &&
                       gyro.deg < 90 + allowanceDegError);
        widthX = abs(tof.vecX[0]) + abs(tof.vecX[8]) + 76 + 30 + SensorRadius;
        widthY = abs(tof.vecY[4]) + abs(tof.vecY[12]) + SensorRadius * 2;
        for (int i = 1; i <= range; i++) {
            if ((i * 300 - allowanceWidthError) < widthX &&
                widthX < (i * 300 + allowanceWidthError)) {
                trustX = true;
                int oldCoordinateX = coordinateX;
                coordinateX = round(coordinateX / 300.0) * 300.0;

                if (isEast) {
                    coordinateX += (abs(tof.vecX[8]) % 300) - (150 - 76);
                } else {
                    coordinateX +=
                        (abs(tof.vecX[0]) % 300) - (150 - SensorRadius - 30);
                }

                while (abs(coordinateX - oldCoordinateX) > 150) {
                    if (coordinateX - oldCoordinateX > 150) {
                        coordinateX -= 300;
                    } else if (coordinateX - oldCoordinateX < -150) {
                        coordinateX += 300;
                    }
                }

                break;
            }
        }

        for (int i = 1; i <= range; i++) {
            if ((i * 300 - allowanceWidthError) < widthY &&
                widthY < (i * 300 + allowanceWidthError)) {
                trustY = true;

                int oldCoordinateY = coordinateY;
                coordinateY = round(coordinateY / 300.0) * 300.0;

                if (isEast) {
                    coordinateY +=
                        (abs(tof.vecY[4]) % 300) - (150 - SensorRadius);
                } else {
                    coordinateY +=
                        (abs(tof.vecY[12]) % 300) - (150 - SensorRadius);
                }

                while (abs(coordinateY - oldCoordinateY) > 150) {
                    if (coordinateY - oldCoordinateY > 150) {
                        coordinateY -= 300;
                    } else if (coordinateY - oldCoordinateY < -150) {
                        coordinateY += 300;
                    }
                }

                break;
            }
        }
    }

    // uart1.println(widthX);

    if (trustX || trustY) {
        tof.canCorrect = true;
    } else {
        tof.canCorrect = false;
    }

    int oldX = x;
    int oldY = y;

    x = round(coordinateX / 300.0);
    y = round(coordinateY / 300.0);

    // 通った経路をはさみで切るイメージ
    if (oldX < x) {  // 東に移動
        wall[x + FIELD_ORIGIN][y + FIELD_ORIGIN].vertical = false;
    }

    if (oldX > x) {  // 西に移動
        wall[oldX + FIELD_ORIGIN][y + FIELD_ORIGIN].vertical = false;
    }

    if (oldY < y) {  // 北に移動
        wall[x + FIELD_ORIGIN][y + FIELD_ORIGIN].horizontal = false;
    }

    if (oldY > y) {  // 南に移動
        wall[x + FIELD_ORIGIN][oldY + FIELD_ORIGIN].horizontal = false;
    }
}

void Location::updateMap(void) {
}