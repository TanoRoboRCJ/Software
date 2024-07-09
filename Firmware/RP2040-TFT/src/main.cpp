#include <Arduino.h>

#include "./ui_kit/ui_kit.h"
UI_KIT ui;

#include "./ui_kit/device/display.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);
DISPLAY_DEVICE display(&tft, &sprite);

void setup() {
    Serial.begin(115200);

    // delay(100);

    ui.init();
}

void loop() {
    display.createSprite();
    display.setBackgroundImage(bootImage);
    display.publish();
}