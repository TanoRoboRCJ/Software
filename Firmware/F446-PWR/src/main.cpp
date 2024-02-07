#include <Arduino.h>

#include <IO-Kit.h>
Output ledBuiltin = Output(PB12);
Output ledL = Output(PB14);
// Output ledBuiltin = Output(PA0);

Output encLED[3] = {Output(PB4), Output(PB5), Output(PB6)};
Input enc[2] = {Input(PA14), Input(PA15)};

HardwareSerial uart1(PA10, PA9);

bool chickPeep(void) {
    return (millis() / 20) % 2 == 0 && (millis() / 300) % 5 == 0;
}

void setup() {
    uart1.begin(115200);
    for (int i = 0; i < 3; i++) {
        encLED[i] = 1;
    }
}

void loop() {
    // static bool encAState = true;
    // static bool encBState = false;
    // static int encData = 0;
    // if (enc[1] == 1 && encAState == 0) {
    //     if (enc[1] ^ enc[0]) {
    //         encData--;
    //     } else {
    //         encData++;
    //     }
    // }
    // encAState = enc[1];

    // delay(3);

    // uart1.println(encData);

    ledBuiltin = chickPeep();
    ledL = (millis() % 1000 < 500) * 0.1 ;

    // static bool ledState = false;
    // ledState = !ledState;
    // ledBuiltin = ledState * 0.3;

    // delay(500);

    // ledState = !ledState;
    // int ledBright = encData * 10;
    // ledBright = constrain(ledBright, 0, 255);

    // ledBuiltin <<= ledBright;

    // // delay(500);

    // for (int i = 0; i < 3; i++) {
    //     encLED[i] = 1;
    // }
    // ledBright = constrain(encData, 0, 255);
    // ledBright = ledBright % 3;
    // encLED[ledBright] = 0;

    // for (int i = 0; i < 3; i++) {
    //     encLED[i] = 0;
    // }

    // delay(500);

    // for (int i = 0; i < 3; i++) {
    //     encLED[i] = 1;
    // }

    // delay(500);
}