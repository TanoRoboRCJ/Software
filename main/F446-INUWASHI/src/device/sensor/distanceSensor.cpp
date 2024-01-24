#include "distanceSensor.h"

#include "gyro.h"

extern GYRO gyro;

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

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
    if (gyro.direction == NORTH) {
        if (val[0] > 170) {
            wallExists[NORTH] = false;
        } else {
            wallExists[NORTH] = true;
        }

        if (val[4] > 210) {
            wallExists[EAST] = false;
        } else {
            wallExists[EAST] = true;
        }

        if (val[8] > 210) {
            wallExists[SOUTH] = false;
        } else {
            wallExists[SOUTH] = true;
        }

        if (val[12] > 210) {
            wallExists[WEST] = false;
        } else {
            wallExists[WEST] = true;
        }
    } else if (gyro.direction == EAST) {
        if (val[12] > 210) {
            wallExists[NORTH] = false;
        } else {
            wallExists[NORTH] = true;
        }

        if (val[0] > 210) {
            wallExists[EAST] = false;
        } else {
            wallExists[EAST] = true;
        }

        if (val[4] > 210) {
            wallExists[SOUTH] = false;
        } else {
            wallExists[SOUTH] = true;
        }

        if (val[8] > 210) {
            wallExists[WEST] = false;
        } else {
            wallExists[WEST] = true;
        }
    } else if (gyro.direction == SOUTH) {
        if (val[8] > 210) {
            wallExists[NORTH] = false;
        } else {
            wallExists[NORTH] = true;
        }

        if (val[12] > 210) {
            wallExists[EAST] = false;
        } else {
            wallExists[EAST] = true;
        }

        if (val[0] > 210) {
            wallExists[SOUTH] = false;
        } else {
            wallExists[SOUTH] = true;
        }

        if (val[4] > 210) {
            wallExists[WEST] = false;
        } else {
            wallExists[WEST] = true;
        }
    } else if (gyro.direction == WEST) {
        if (val[4] > 210) {
            wallExists[NORTH] = false;
        } else {
            wallExists[NORTH] = true;
        }

        if (val[8] > 210) {
            wallExists[EAST] = false;
        } else {
            wallExists[EAST] = true;
        }

        if (val[12] > 210) {
            wallExists[SOUTH] = false;
        } else {
            wallExists[SOUTH] = true;
        }

        if (val[0] > 210) {
            wallExists[WEST] = false;
        } else {
            wallExists[WEST] = true;
        }
    }
}
