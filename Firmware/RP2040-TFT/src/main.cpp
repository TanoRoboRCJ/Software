#include <Arduino.h>
#include <TFT_eSPI.h>

#include "display.h"

#include "ui_kit/img/kuyopoyo.h"
#include "ui_kit/img/ui_image/ui_image.h"

LGFX_Sprite mainSprite(&display);
LGFX_Sprite textSprite(&mainSprite);

TFT_eSprite sprite = TFT_eSprite(nullptr);

void setup() {
    display.init();
    display.fillScreen(TFT_BLACK);
    display.setRotation(1);
    display.setColorDepth(16);

    mainSprite.createSprite(240, 240);
    mainSprite.drawJpg((std::uint8_t*)KuyopoyoImg,
                       (unsigned int)KuyopoyoImg_len);
    mainSprite.pushSprite(0, 0);
}

void loop() {
    mainSprite.drawJpg((std::uint8_t*)KuyopoyoImg,
                       (unsigned int)KuyopoyoImg_len);
                       
    sprite.createSprite(120, 50);
    sprite.fillScreen(TFT_RED);

    textSprite.createSprite(120, 50);
    uint16_t* imgBufPtr = (uint16_t*)sprite.getPointer();
    textSprite.pushImage(0, 0, 120, 50, imgBufPtr);
    // textSprite.fillSprite(TFT_WHITE);
    textSprite.pushSprite(&mainSprite, 0, 0);
    mainSprite.pushSprite(0, 0);

    delay(2000);

    // mainSprite.pushSprite(0, 0);
    // delay(10);
}
