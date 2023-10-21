#include <Arduino.h>
#include <M5Core2.h>
#include "./lidar.h"

LIDAR lidar(&Serial2, 32, 33);

void setup() {
    M5.begin();

    Serial.begin(9600);
}

void loop() {
    static int speed = 128;
    static int count = 0;
    M5.Lcd.printf("Count: %d\n", count);  // LCDに表示
    count++;
}