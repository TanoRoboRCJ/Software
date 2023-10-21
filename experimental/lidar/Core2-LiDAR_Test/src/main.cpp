#include <Arduino.h>
// #include <M5Core2.h>
// #include "./lidar.h"

void setup() {
    // M5.begin();

    Serial.begin(9600);
    Serial2.begin(230400, SERIAL_8N1, 32, 33);

    pinMode(33, OUTPUT);
}

void loop() {
    static int speed = 128;
    static int count = 0;
    // M5.Lcd.printf("Count: %d\n", count);  // LCDに表示
    count++;

    if (Serial2.available()) {
        char data = Serial2.read();
        Serial.println(data);
    }

    if (Serial.available()) {
        speed = Serial.read() * 2;
    }

    analogWrite(33, speed);
}