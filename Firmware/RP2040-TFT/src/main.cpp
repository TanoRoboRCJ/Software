#include <Arduino.h>

#include "./ui_kit/ui_kit.h"
UI_KIT ui;

#include "./ui_kit/device/display.h"

// #include "./ui_kit/img/kuyopoyo.h"
#include "./ui_kit/img/kuyopoyo_png.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);
DISPLAY_DEVICE display(&tft, &sprite);

void setup() {
    Serial.begin(115200);
    ui.init();
}

void loop() {
    // display.createSprite();
    // display.setBackgroundImage(bootImage);
    // display.publish();

    display.createSprite();
    display.setBackgroundImagePNG((uint8_t*)KuyopoyoImg, KuyopoyoImg_len);
    display.publish();
}