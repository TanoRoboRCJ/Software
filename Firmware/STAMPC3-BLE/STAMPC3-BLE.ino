// #include <Arduino.h>

#include <FastLED.h>
CRGB led[1];

#include "./BLE_Peripheral.h"
BLE_PERIPHERAL BLE_Peripheral("ONAMAZU BLE-Kit");

char sendDataArr[140] = { 0 };

void setup() {
  Serial.begin(115200);

  // BLE_Peripheral.enableDebugMode();
  BLE_Peripheral.init();

  FastLED.addLeds<WS2812B, 2, GRB>(led, 1);
}

void loop() {
  if (BLE_Peripheral.checkConnection()) {
    if (Serial.available() != 0) {
      int dataSize = 0;
      char sendDataArr[140] = { 0 };
      while (Serial.available() != 0) {
        sendDataArr[dataSize] = Serial.read();
        dataSize++;
      }

      BLE_Peripheral.write(sendDataArr, dataSize);
    }

    if ((millis() / 20) % 2 == 0 && (millis() / 300) % 5 == 0) {
      led[0] = CRGB::Cyan;
    } else {
      led[0] = CRGB::Black;
    }

    FastLED.show();
  } else {
    led[0] = CRGB::Orange;
    FastLED.show();
  }
}