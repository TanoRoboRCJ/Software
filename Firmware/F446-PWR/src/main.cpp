#include <Arduino.h>
#include <VL53L0X.h>

#include <IO-Kit.h>

VL53L0X distanceSensor[2];
Output xshutPin[2] = {Output(PA5), Output(PA6)};
const int addr[2] = {0x30, 0x31};

#include <Wire.h>
#include <SPI.h>
#include "./lib/Adafruit_TCS34725.h"

Adafruit_TCS34725 tcs =
    Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_60MS, TCS34725_GAIN_1X);

HardwareSerial uartForDebug(PA10, PA9);

void invalidA(void) {
    while (1) {
        uartForDebug.println("invalid A: VL53L0X ERR");
        delay(100);
    }
}

void invalidB(void) {
    while (1) {
        uartForDebug.println("invalid B: TCS34725 ERR");
        delay(100);
    }
}

void* invalid[2];

void setup(void) {
    uartForDebug.begin(115200);

    Wire.setSDA(PB9);
    Wire.setSCL(PB8);
    Wire.begin();

    for (int i = 0; i < 2; i++) {
        xshutPin[i] = LOW;
    }

    delay(100);

    for (int i = 0; i < 2; i++) {
        xshutPin[i] = HIGH;
        delay(10);
        distanceSensor[i].setTimeout(100);
        distanceSensor[i].setAddress(addr[i]);
        if (!distanceSensor[i].init()) {
            invalidA();
        }
        distanceSensor[i].setSignalRateLimit(0.1);
        distanceSensor[i].setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
        distanceSensor[i].setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange,
                                              14);
        distanceSensor[i].startContinuous();
    }

    if (!tcs.begin()) {
        invalidB();
    }
}

void loop(void) {
    uint16_t color[3] = {0};
    uint16_t brightness = 0;

    tcs.getRawData(&color[0], &color[1], &color[2], &brightness);

    // Print
    uartForDebug.printf("TCS34725:");
    for (int i = 0; i < 3; i++) {
        uartForDebug.printf("%6d", color[i]);
    }
    uartForDebug.printf("%6d", brightness);

    int rawData = distanceSensor[0].readRangeContinuousMillimeters();
    uartForDebug.printf(" | VL53L0X 1:%4d [mm]", rawData);
    rawData = distanceSensor[1].readRangeContinuousMillimeters();
    uartForDebug.printf(" | VL53L0X 2:%4d [mm]\n", rawData);
}