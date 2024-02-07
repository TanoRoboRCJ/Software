#include <Arduino.h>

#include <IO-Kit.h>
Output ledBuiltin = Output(PB12);

HardwareSerial uart1(PA10, PA9);

bool chickPeep(void) {
    return (millis() / 20) % 2 == 0 && (millis() / 300) % 5 == 0;
}

void setup() {
    uart1.begin(115200);
}

void loop() {
    ledBuiltin = chickPeep();
}