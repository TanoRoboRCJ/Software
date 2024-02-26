// #include <Arduino.h>

// #include <IO-Kit.h>
// Output ledBuiltin = Output(PB12);

// HardwareSerial uart1(PA10, PA9);

// bool chickPeep(void) {
//     return (millis() / 20) % 2 == 0 && (millis() / 300) % 5 == 0;
// }

// void setup() {
//     uart1.begin(115200);
// }

// void loop() {
//     ledBuiltin = chickPeep();
// }

#include <Arduino.h>

volatile bool resetFlag = false;

bool isSwtichPressed() {
    // 任意のコードを記述
    return 0;
}

void interrupt() {
    if (isSwtichPressed()) {
        resetFlag = true;
    }
}

int millisecondToSecond(int millisecond) {
    return millisecond / 1000;
}

void loop() {
    static unsigned long startTime = 0;  // 開始時刻

    if (resetFlag) {
        startTime = millis();
        resetFlag = false;
    }

    const int elapsedTime = millis() - startTime;
    const int remainingTime = 60000 - elapsedTime;

    showDisplay(millisecondToSecond(remainingTime)); //この辺の仕様は人による
}