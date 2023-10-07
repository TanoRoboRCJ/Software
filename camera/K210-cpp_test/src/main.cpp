#include <Arduino.h>
#include <WS2812FX.h>
#include <IO-Kit.h>

#include "./def.h"

uint32_t baudrate = 9600;

WS2812FX led = WS2812FX(numLEDs, rgbLED, NEO_GRB + NEO_KHZ800);

Input buttonA = Input(buttonAPin);
Input buttonB = Input(buttonBPin);

uint32_t red = led.Color(255, 0, 0);
uint32_t blue = led.Color(0, 0, 255);
uint32_t black = led.Color(0, 0, 0);

void setup() {
    Serial.begin(baudrate);

    Serial.println("--------------");
    Serial.println("K210 Test");
    Serial.println("baudrate: " + String(baudrate));
    Serial.println("--------------");
    delay(2000);

    led.init();
    led.setBrightness(255);
    led.show();
}

void loop() {
    Serial.print(millis());

    if (!buttonA) {
        Serial.print("\tButton A is pressed!");
        led.setPixelColor(0, red);
    } else if (!buttonB) {
        Serial.print("\tButton B is pressed!");
        led.setPixelColor(0, blue);
    } else {
        led.setPixelColor(0, black);
    }
    led.show();

    Serial.println();
}