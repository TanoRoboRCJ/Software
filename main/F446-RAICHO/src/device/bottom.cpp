#include "./bottom.h"
#include "../process/location.h"
#include "./sensor/gyro.h"
#include "./sensor/distanceSensor.h"

extern Location location;
extern GYRO gyro;
extern DISTANCE_SENSOR tof;

Bottom::Bottom(HardwareSerial *ptr) {
    serialPtr = ptr;
    serialPtr->begin(1000000);
}

bool Bottom::read(void) {
    if (serialPtr->available() < 35) {
        return 1;
    }

    if (serialPtr->read() == 'B') {
        if (serialPtr->read() == 'T') {
            // ハードコードなのはあえて（Notion見てね）
            tcsClear[0] = (serialPtr->read() << 8) + serialPtr->read();
            tcsRed[0] = (serialPtr->read() << 8) + serialPtr->read();
            tcsGreen[0] = (serialPtr->read() << 8) + serialPtr->read();
            tcsBlue[0] = (serialPtr->read() << 8) + serialPtr->read();

            tcsClear[1] = (serialPtr->read() << 8) + serialPtr->read();
            tcsRed[1] = (serialPtr->read() << 8) + serialPtr->read();
            tcsGreen[1] = (serialPtr->read() << 8) + serialPtr->read();
            tcsBlue[1] = (serialPtr->read() << 8) + serialPtr->read();

            int gomi = (serialPtr->read() << 8) + serialPtr->read();
            gomi = (serialPtr->read() << 8) + serialPtr->read();
        }
    }

    while (serialPtr->available() > 0) {
        serialPtr->read();
    }

    return 0;
}

void Bottom::write(void) {
    // ハードコードなのはあえて（Notion見てね）
    serialPtr->write('P');
    serialPtr->write('W');

    serialPtr->write(rescueKit[0]);
    serialPtr->write(rescueKit[1]);

    serialPtr->write(ToggleBrightness);

    serialPtr->write(highByte((int)location.coordinateX));
    serialPtr->write(lowByte((int)location.coordinateX));

    serialPtr->write(highByte((int)location.coordinateY));
    serialPtr->write(lowByte((int)location.coordinateY));

    serialPtr->write(highByte((int)location.coordinateZ));
    serialPtr->write(lowByte((int)location.coordinateZ));

    serialPtr->write(highByte(gyro.deg));
    serialPtr->write(lowByte(gyro.deg));

    char wall = tof.rightWallExists + (tof.frontWallExists << 1) + (tof.leftWallExists << 2) + (tof.behindWallExists << 3);
    serialPtr->write(wall);
}