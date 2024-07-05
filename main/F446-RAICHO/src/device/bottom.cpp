#include "./bottom.h"

Bottom::Bottom(HardwareSerial *ptr) {
    serialPtr = ptr;
    serialPtr->begin(115200);
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

            tof[0] = (serialPtr->read() << 8) + serialPtr->read();
            tof[1] = (serialPtr->read() << 8) + serialPtr->read();
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

    serialPtr->write(LED_color[0]);
    serialPtr->write(LED_color[1]);
    serialPtr->write(LED_color[2]);
}