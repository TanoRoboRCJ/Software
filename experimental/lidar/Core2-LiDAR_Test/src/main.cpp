#include <Arduino.h>
#include <M5Core2.h>
#include "./lidar.h"

LIDAR lidar(&Serial2, 32, 33);

void setup() {
    M5.begin();
    M5.Lcd.printf("Hello World\n");

    Serial.begin(115200);
}

void loop() {
    lidar.read();

    if (Serial.available() > 0) {
        while (Serial.available() != 0) {
            Serial.read();
        }
        M5.Lcd.printf("read:\n");

        for (int i = 0; i < LIDAR::DataBuffLength; i++) {
            Serial.print(i);
            Serial.print("\t");
            Serial.print(lidar.point[i].x);
            Serial.print("\t");
            Serial.println(lidar.point[i].y);
        }
        Serial.println();
    }
}