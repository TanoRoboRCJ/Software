#include "distanceSensor.h"

#include "gyro.h"

extern GYRO gyro;

DISTANCE_SENSOR::DISTANCE_SENSOR(HardwareSerial *p) {
    serialPtr = p;
    serialPtr->begin(1000000);
}

int DISTANCE_SENSOR::read(void) {
    if (serialPtr->available() >= 35) {
        int checkDegit = 0;

        if (serialPtr->read() == 'V') {
            if (serialPtr->read() == 'L') {
                for (int i = 0; i < 16; i++) {
                    char data[2];
                    data[0] = serialPtr->read();
                    data[1] = serialPtr->read();

                    checkDegit += data[0] + data[1];

                    _valTemp[i] = (data[0] << 8) + data[1];
                }
            }
        }

        if (checkDegit % 256 == serialPtr->read()) {
            for (int i = 0; i < 16; i++) {
                if (_valTemp[i] > 10) {
                    val[i] = constrain(_valTemp[i], 10, 1200);
                }
            }
        }

        while (serialPtr->available() > 0) {
            serialPtr->read();
        }

        return 0;

    } else {
        return 1;
    }
}

void DISTANCE_SENSOR::calc(int angle) {
    for (int n = 0; n < 16; n++) {
        vecX[n] = val[n] * sin(radians(n * 22.5 + angle));
        vecY[n] = val[n] * cos(radians(n * 22.5 + angle));
    }
    direction();
}


void DISTANCE_SENSOR::direction(void) {
    gyro.read();
    if (gyro.North) {
        if (val[0] > 170) {
            isNorthWall = false;
        } else {
            isNorthWall = true;
        }

        if (val[4] > 210) {
            isEastWall = false;
        } else {
            isEastWall = true;
        }

        if (val[8] > 210) {
            isSouthWall = false;
        } else {
            isSouthWall = true;
        }

        if (val[12] > 210) {
            isWestWall = false;
        } else {
            isWestWall = true;
        }
    } else if (gyro.East) {
        if (val[12] > 210) {
            isNorthWall = false;
        } else {
            isNorthWall = true;
        }

        if (val[0] > 210) {
            isEastWall = false;
        } else {
            isEastWall = true;
        }

        if (val[4] > 210) {
            isSouthWall = false;
        } else {
            isSouthWall = true;
        }

        if (val[8] > 210) {
            isWestWall = false;
        } else {
            isWestWall = true;
        }
    } else if (gyro.South) {
        if (val[8] > 210) {
            isNorthWall = false;
        } else {
            isNorthWall = true;
        }

        if (val[12] > 210) {
            isEastWall = false;
        } else {
            isEastWall = true;
        }

        if (val[0] > 210) {
            isSouthWall = false;
        } else {
            isSouthWall = true;
        }

        if (val[4] > 210) {
            isWestWall = false;
        } else {
            isWestWall = true;
        }
    } else if (gyro.West) {
        if (val[4] > 210) {
            isNorthWall = false;
        } else {
            isNorthWall = true;
        }

        if (val[8] > 210) {
            isEastWall = false;
        } else {
            isEastWall = true;
        }

        if (val[12] > 210) {
            isSouthWall = false;
        } else {
            isSouthWall = true;
        }

        if (val[0] > 210) {
            isWestWall = false;
        } else {
            isWestWall = true;
        }
    }
}

void DISTANCE_SENSOR::relativeDirection(void) {
    if (val[0] < 150) {
        isFrontWall = true;
    } else {
        isFrontWall = false;
    }
    if (val[4] < 180) {
        isRightWall = true;
    } else {
        isRightWall = false;
    }
    if (val[8] < 180) {
        isBehindWall = true;
    } else {
        isBehindWall = false;
    }
    if (val[12] < 180) {
        isLeftWall = true;
    } else {
        isLeftWall = false;
    }
}