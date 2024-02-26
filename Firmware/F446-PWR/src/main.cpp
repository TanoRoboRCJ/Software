#include <Arduino.h>
#include <VL53L0X.h>

#include "./kit/IO-Kit.h"


Output encLED[3] = {Output(PB4), Output(PB5), Output(PB6)};
Output ledL = Output(PA0);

Input encSW(PB7);

#include <Wire.h>
#include <SPI.h>

#include "./device/distanceSensor.h"
TwoWire wireForVL53L0X(PB9, PB8);
DistanceSensor tof(&wireForVL53L0X);

HardwareSerial uartForDebug(PA10, PA9);

void setup(void) {
    uartForDebug.begin(115200);

    wireForVL53L0X.setSCL(PB8);
    wireForVL53L0X.setSDA(PB9);
    wireForVL53L0X.begin();

    bool status = tof.init();

    if (!status) {
        uartForDebug.println("Failed to detect and initialize VL53L0X");
    }

    for (int i = 0; i < 3; i++) {
        encLED[i] = HIGH;
    }

    delay(100);
}

void loop(void) {
    encLED[2] = 0;
    static bool state = 0;
    if (encSW != state && encSW == false) {
        state = !state;

        ledL = !ledL * 0.3;
    }
    state = encSW;

    tof.read();

    for (int i = 0; i < 2; i++) {
        uartForDebug.print(tof.val[i]);
        uartForDebug.print("\t");
    }

    uartForDebug.println();
}