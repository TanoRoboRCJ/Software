#include <Arduino.h>

#include "display.h"

#include "ui_kit/img/kuyopoyo.h"
#include "ui_kit/img/ui_image/ui_image.h"

LGFX_Sprite mainSprite(&display);
LGFX_Sprite textSprite(&mainSprite);

void setup() {
    display.init();
    display.fillScreen(TFT_BLACK);
    display.setRotation(3);
    display.setColorDepth(16);

    mainSprite.createSprite(240, 240);
    mainSprite.drawJpg((std::uint8_t*)KuyopoyoImg,
                       (unsigned int)KuyopoyoImg_len);
    mainSprite.pushSprite(0, 0);

    textSprite.createSprite(120, 50);
}

void loop() {
    for (int i = 0; i < 100; i++) {
        // mainSprite.drawPng((std::uint8_t*)BgImgPtr[i],
        //                    (unsigned int)BgImgLenPtr[i]);

        textSprite.fillSprite(TFT_BLACK);
        textSprite.setTextColor(TFT_WHITE);
        textSprite.setTextSize(0.5);
        textSprite.setFont(&fonts::Font8);
        textSprite.setCursor(1, 0);
        textSprite.print(i);
        textSprite.pushSprite(&display, 60, 80);

        textSprite.fillSprite(TFT_WHITE);
        textSprite.setTextColor(TFT_BLACK);
        textSprite.setTextSize(1);
        textSprite.setFont(&fonts::Font8);
        textSprite.setCursor(0, 0);
        textSprite.print(i);
        // textSprite.pushSprite(&display, 60, 90);

        // mainSprite.pushSprite(0, 0);

        if (Serial.available()) {
            while (1);
        }
    }
}