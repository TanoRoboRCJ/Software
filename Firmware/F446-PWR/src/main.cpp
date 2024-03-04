#include <Arduino.h>

#include "./kit/IO-Kit.h"
Output ledToggle = Output(PA0);
Output servoR = Output(PB2);

#include <Servo.h>
// Servo servo[2];

// #include "./servo/Servo.h"
// Servo servoR;

// #include <PWMServo.h>
// PWMServo servoR;
// Servo servoL;

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

    enc.bootIllumination(2500);

    pinMode(PB2, OUTPUT);

    // servoR.attach(PB2);
    // servoL.attach(PB1);
}

void loop(void) {
    enc.read();

    tof.read();
    floorSensor[0].read();
    floorSensor[1].read();

    if (!uartForDebugEnable) {
        return;
    }

    ledToggle = enc.isPressed * 0.5;

    uartForDebug.print("encA:");
    uartForDebug.print(enc.encAB[0]);
    uartForDebug.print("\tencB:");
    uartForDebug.print(enc.encAB[1]);

    int deg = (millis() / 1000) % 2;
    deg *= 90;

    // servoR.write(deg);
    // // servoL.write(180 - deg);
    // servoR.writeMicroseconds(1000);

    servoR = HIGH;
    delayMicroseconds(1000 + deg * 1000 / 180);
    servoR = LOW;
    delay(20);

    uartForDebug.print("\tServoR:");
    uartForDebug.print(deg);

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

    uartForDebug.println();
}