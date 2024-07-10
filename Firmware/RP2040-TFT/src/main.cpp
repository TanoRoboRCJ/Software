#include <Arduino.h>
#include <TFT_eSPI.h>

#include "display.h"
#include "ui_kit/font/SF-Mono-HeavyItalic60.h"

#include "ui_kit/img/kuyopoyo.h"
#include "ui_kit/img/ui_image/ui_image.h"

LGFX_Sprite mainSprite(&display);
LGFX_Sprite textSprite(&mainSprite);

TFT_eSprite sprite = TFT_eSprite(nullptr);

int wallStatus = 0;
volatile bool refleshFlag = false;
volatile bool drawFlag = false;

void setup() {
    Serial.begin(115200);

    display.init();
    display.fillScreen(TFT_BLACK);
    display.setRotation(3);
    display.setColorDepth(16);

    mainSprite.createSprite(240, 240);
    mainSprite.drawJpg((std::uint8_t*)KuyopoyoImg,
                       (unsigned int)KuyopoyoImg_len);
    mainSprite.pushSprite(0, 0);

    // while (Serial.available() == 0) {
    // }
}

void textX1(void) {
    sprite.createSprite(120, 50);
    sprite.fillScreen(TFT_WHITE);
    sprite.loadFont(SF_Mono_HeavyItalic60_vlw);
    sprite.setTextColor(TFT_BLACK, TFT_WHITE);

    String str = (String)(((int)(millis() / 10) % 100) - 50);

    int posX = 60 - sprite.textWidth(str) / 2;
    sprite.setCursor(posX, 0);
    sprite.print(str);
}

void textX2(void) {
    sprite.createSprite(120, 50);
    sprite.fillScreen(TFT_WHITE);
    sprite.loadFont(SF_Mono_HeavyItalic60_vlw);
    sprite.setTextColor(TFT_BLACK, TFT_WHITE);

    String str = (String)(((int)(millis() / 10) % 100) - 50);

    int posX = 60 - sprite.textWidth(str) / 2;
    sprite.setCursor(posX, 0);
    sprite.print(str);
}

void loop() {
    static int prevWallStatus = wallStatus;

    wallStatus = (millis() / 200) % 16;

    if (wallStatus != prevWallStatus) {
        refleshFlag = true;
    }

    if (drawFlag) {
        textX1();

        textSprite.createSprite(120, 50);
        uint16_t* imgBufPtr = (uint16_t*)sprite.getPointer();
        textSprite.pushImage(0, 0, 120, 50, imgBufPtr);
        textSprite.pushSprite(60, 50);

        textX2();

        textSprite.createSprite(120, 50);
        imgBufPtr = (uint16_t*)sprite.getPointer();
        textSprite.pushImage(0, 0, 120, 50, imgBufPtr);
        textSprite.pushSprite(60, 100);

        mainSprite.pushSprite(0, 0);
        drawFlag = false;
    } else {
        textX1();

        textSprite.createSprite(120, 50);
        uint16_t* imgBufPtr = (uint16_t*)sprite.getPointer();
        textSprite.pushImage(0, 0, 120, 50, imgBufPtr);
        textSprite.pushSprite(&display, 60, 50);

        textX2();

        textSprite.createSprite(120, 50);
        imgBufPtr = (uint16_t*)sprite.getPointer();
        textSprite.pushImage(0, 0, 120, 50, imgBufPtr);
        textSprite.pushSprite(&display, 60, 100);
    }

    prevWallStatus = wallStatus;
}

void setup1() {
}

void loop1() {
    if (refleshFlag) {
        mainSprite.createSprite(240, 240);
        mainSprite.drawPng((std::uint8_t*)BgImgPtr[wallStatus],
                           (unsigned int)BgImgLenPtr[wallStatus]);

        refleshFlag = false;
        drawFlag = true;
    }
}