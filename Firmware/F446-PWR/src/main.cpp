#include <Arduino.h>

#include "./kit/IO-Kit.h"

Output ledL = Output(PA0);

#include <Wire.h>
TwoWire wireBus1(PB9, PB8);
TwoWire wireBus2(PB3, PB10);

#include "./device/distanceSensor.h"
DistanceSensor tof(&wireBus1);

#include "./device/floorSensor.h"
FloorSensor floorSensor[2] = {FloorSensor(&wireBus2), FloorSensor(&wireBus1)};

#include "./device/encoder.h"
Encoder enc;

HardwareSerial uartForDebug(PA10, PA9);
const bool uartForDebugEnable = true;

void setup(void) {
    uartForDebug.begin(115200);

    wireBus1.begin();
    wireBus2.begin();

    tof.init();
    floorSensor[0].init();
    floorSensor[1].init();

    enc.bootIllumination();    
}

void loop(void) {
    // encLED[2] = 0;
    // static bool state = 0;
    // if (encSW != state && encSW == false) {
    //     state = !state;

    //     ledL = !ledL * 0.3;
    // }
    // state = encSW;

    tof.read();
    floorSensor[0].read();
    floorSensor[1].read();

    if (!uartForDebugEnable) {
        return;
    }

    // uartForDebug.print("ToF|\t");
    // for (int i = 0; i < 2; i++) {
    //     uartForDebug.print(tof.val[i]);
    //     uartForDebug.print("\t");
    // }

    // for (int tcsSelecter = 0; tcsSelecter < 2; tcsSelecter++) {
    //     uartForDebug.print("TCS| clear\t");
    //     uartForDebug.print(floorSensor[tcsSelecter].clear);
    //     uartForDebug.print("\tRGB:\t");
    //     for (int i = 0; i < 3; i++) {
    //         uartForDebug.print(floorSensor[tcsSelecter].rgb[i]);
    //         uartForDebug.print("\t");
    //     }
    // }

    // uartForDebug.println();
}