#include <Arduino.h>

#define RIGHT 0
#define LEFT 1

#include "./kit/IO-Kit.h"
Output ledToggle = Output(PA0);
Output ledBuiltin = Output(PB12);

const int PushDeg[2] = {-38, 38};
const int PullDeg[2] = {40, -40};
Output servo[2] = {Output(PB2), Output(PB1)};
bool isPushing[2] = {true, true};

const int PwmFreq = 200;

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
HardwareSerial uart2(PA3, PA2);

const bool uartForDebugEnable = true;

int degToPulse(int deg) {
    const int Neutral = 1500;
    const int Range = 1000;

    return map(map(deg, -90, 90, Neutral - Range, Neutral + Range), 0, 5000, 0,
               255);
}

void uartRead(void) {
    // serial
    if (uart2.available() >= 8) {
        int checkDegit = 0;
        char data[6] = {0};

        if (uart2.read() == 'P') {
            if (uart2.read() == 'W') {
                for (int i = 0; i < 6; i++) {
                    data[i] = uart2.read();
                    checkDegit += data[i];
                }

                isPushing[RIGHT] = (data[0] == 1);
                isPushing[LEFT] = (data[1] == 1);

                ledToggle <<= data[2];

                uint8_t color[3] = {0};
                for (int i = 0; i < 3; i++) {
                    color[i] = data[i + 3];
                }

                enc.show(color);
            }
        }

        while (uart2.available() > 0) {
            uart2.read();
        }
    }
}

void uartWrite(void) {
    uart2.write('B');
    uart2.write('T');

    uart2.write(highByte(floorSensor[0].clear));
    uart2.write(lowByte(floorSensor[0].clear));

    for (int i = 0; i < 3; i++) {
        uart2.write(highByte(floorSensor[0].rgb[i]));
        uart2.write(lowByte(floorSensor[0].rgb[i]));
    }

    uart2.write(highByte(floorSensor[1].clear));
    uart2.write(lowByte(floorSensor[1].clear));

    for (int i = 0; i < 3; i++) {
        uart2.write(highByte(floorSensor[1].rgb[i]));
        uart2.write(lowByte(floorSensor[1].rgb[i]));
    }

    uart2.write(highByte(tof.val[RIGHT]));
    uart2.write(lowByte(tof.val[RIGHT]));

    uart2.write(highByte(tof.val[LEFT]));
    uart2.write(lowByte(tof.val[LEFT]));
}

void setup(void) {
    uartForDebug.begin(115200);
    uart2.begin(115200);

    wireBus1.begin();
    wireBus2.begin();

    tof.init();
    floorSensor[0].init();
    floorSensor[1].init();

    enc.bootIllumination(0);

    analogWriteFrequency(PwmFreq);
}

void loop(void) {
    // status LED
    bool chirp = ((millis() / 50) % 30 == 0);
    ledBuiltin = chirp;

    // read
    enc.read();

    tof.read();
    floorSensor[0].read();
    floorSensor[1].read();

    // serial
    uartRead();
    uartWrite();

    // write
    for (int id = 0; id < 2; id++) {
        if (isPushing[id]) {
            servo[id] <<= degToPulse(PushDeg[id]);
        } else {
            servo[id] <<= degToPulse(PullDeg[id]);
        }
    }

    // SERIAL DEBUG
    if (!uartForDebugEnable) {
        return;
    }

    uartForDebug.print("ToF|\t");
    for (int i = 0; i < 2; i++) {
        uartForDebug.print(tof.val[i]);
        uartForDebug.print("\t");
    }

    for (int tcsSelecter = 0; tcsSelecter < 2; tcsSelecter++) {
        uartForDebug.print("TCS| clear\t");
        uartForDebug.print(floorSensor[tcsSelecter].clear);
        uartForDebug.print("\tRGB:\t");
        for (int i = 0; i < 3; i++) {
            uartForDebug.print(floorSensor[tcsSelecter].rgb[i]);
            uartForDebug.print("\t");
        }
    }

    static unsigned long lastTime = millis();
    uartForDebug.print("Period: ");
    uartForDebug.print((millis() - lastTime));

    lastTime = millis();

    uartForDebug.println();
}