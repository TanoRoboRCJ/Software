// #include <Arduino.h>

#include <FastLED.h>
CRGB led[1];

#include "./BLE_Peripheral.h"
BLE_PERIPHERAL BLE_Peripheral("RAICHO BLE-Kit");

char sendDataArr[140] = { 0 };
int dataRing = 0;

void setup() {
  Serial.begin(115200);

  // BLE_Peripheral.enableDebugMode();
  BLE_Peripheral.init();

  FastLED.addLeds<WS2812B, 2, GRB>(led, 1);
}

void loop() {
  if (BLE_Peripheral.checkConnection()) {
    if (Serial.available() != 0) {
      while (Serial.available() != 0) {
        sendDataArr[dataRing] = Serial.read();

        if (sendDataArr[dataRing] == '\n') {
          BLE_Peripheral.write(sendDataArr, dataRing);
          dataRing = 0;
          for (int i = 0; i < 140; i++) {
            sendDataArr[i] = 0;
          }

          break;
        }
        
        dataRing++;
      }
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