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
    if (serialPtr->available() >= 35 + 8) {
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

        uint8_t receivedBytesX[sizeof(float)];
        for (size_t i = 0; i < sizeof(float); i++) {
            receivedBytesX[i] = serialPtr->read();
        }
        // float covX_float = *reinterpret_cast<float *>(receivedBytesX);
        float covX_float;
        memcpy(&covX_float, receivedBytesX, sizeof(covX_float));

        uint8_t receivedBytesY[sizeof(float)];
        for (size_t i = 0; i < sizeof(float); i++) {
            receivedBytesY[i] = serialPtr->read();
        }
        // float covY_float = *reinterpret_cast<float *>(receivedBytesY);
        float covY_float;
        memcpy(&covY_float, receivedBytesY, sizeof(covY_float));

        covX = covX_float;
        covY = covY_float;

        while (serialPtr->available() > 0) {
            serialPtr->read();
        }

        val[8] = constrain(bottom.tof[1] - 5, 0, 1200);

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
    wallJudgment();
    direction();

    // if (covX > 60 && covY > 60) {
    //     canCorrect = true;
    // } else {
    //     canCorrect = false;
    // }
}

void DISTANCE_SENSOR::direction(void) {
    if (gyro.direction == NORTH) {
        wallExists[NORTH] = frontWallExists;
        wallExists[EAST] = rightWallExists;
        wallExists[SOUTH] = behindWallExists;
        wallExists[WEST] = leftWallExists;
    } else if (gyro.direction == EAST) {
        wallExists[NORTH] = leftWallExists;
        wallExists[EAST] = frontWallExists;
        wallExists[SOUTH] = rightWallExists;
        wallExists[WEST] = behindWallExists;
    } else if (gyro.direction == SOUTH) {
        wallExists[NORTH] = behindWallExists;
        wallExists[EAST] = leftWallExists;
        wallExists[SOUTH] = frontWallExists;
        wallExists[WEST] = rightWallExists;
    } else if (gyro.direction == WEST) {
        wallExists[NORTH] = rightWallExists;
        wallExists[EAST] = behindWallExists;
        wallExists[SOUTH] = leftWallExists;
        wallExists[WEST] = frontWallExists;
    }
}

void DISTANCE_SENSOR::wallJudgment(void) {
    gyro.read();    //  NOTE これを消すと動かない
    if (val[4] > 215) {
        rightWallExists = false;
    } else {
        rightWallExists = true;
    }
    if (val[0] > 140) {
        frontWallExists = false;
    } else {
        frontWallExists = true;
    }
    if (val[8] > 180) {
        behindWallExists = false;
    } else {
        behindWallExists = true;
    }
    if (val[12] > 215) {
        leftWallExists = false;
    } else {
        leftWallExists = true;
    }
}