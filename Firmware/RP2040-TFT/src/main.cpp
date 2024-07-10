#include <Arduino.h>
#include <TFT_eSPI.h>

#include "display.h"
#include "ui_kit/font/SF-Mono-HeavyItalic60.h"
#include "ui_kit/font/SF-Mono-Medium11.h"
#include "ui_kit/font/SF-Mono-Semibold13.h"

#include "ui_kit/img/kuyopoyo.h"
#include "ui_kit/img/ui_image/ui_image.h"

LGFX_Sprite mainSprite(&display);
LGFX_Sprite textSprite(&mainSprite);
LGFX_Sprite miniTextSprite(&mainSprite);

TFT_eSprite sprite = TFT_eSprite(nullptr);
TFT_eSprite miniSprite = TFT_eSprite(nullptr);

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
    String str = (String)(((int)(millis() / 10) % 100 - 50) * -1);
    // str = -77;

    sprite.createSprite(120, 50);
    sprite.fillScreen(TFT_WHITE);
    sprite.loadFont(SF_Mono_HeavyItalic60_vlw);
    sprite.setTextColor(sprite.color24to16(0x0e0e28), TFT_WHITE);

    int posX = 60 - sprite.textWidth(str) / 2;
    sprite.setCursor(posX, 0);
    sprite.print(str);

    textSprite.createSprite(120, 50);
    uint16_t* imgBufPtr = (uint16_t*)sprite.getPointer();
    textSprite.pushImage(0, 0, 120, 50, imgBufPtr);
}

void textY1(void) {
    String str = (String)(((int)(millis() / 10) % 100) - 50);
    // str = 180;

    sprite.createSprite(120, 50);
    sprite.fillScreen(TFT_WHITE);
    sprite.loadFont(SF_Mono_HeavyItalic60_vlw);
    sprite.setTextColor(sprite.color24to16(0x0e0e28), TFT_WHITE);

    int posX = 60 - sprite.textWidth(str) / 2;
    sprite.setCursor(posX, 0);
    sprite.print(str);

    textSprite.createSprite(120, 50);
    uint16_t* imgBufPtr = (uint16_t*)sprite.getPointer();
    textSprite.pushImage(0, 0, 120, 50, imgBufPtr);
}

void textCoordinateX(void) {
    String str = (String)(((int)(millis() / 10) % 100) - 10000);

    miniSprite.createSprite(40, 13);
    miniSprite.fillScreen(TFT_WHITE);
    miniSprite.loadFont(SF_Mono_Semibold13_vlw);
    miniSprite.setTextColor(miniSprite.color24to16(0x343438), TFT_WHITE);

    int posX = 40 / 2 - miniSprite.textWidth(str) / 2;
    miniSprite.setCursor(posX, 0);
    miniSprite.print(str);

    miniTextSprite.createSprite(40, 13);
    uint16_t* imgBufPtr = (uint16_t*)miniSprite.getPointer();
    miniTextSprite.pushImage(0, 0, 40, 13, imgBufPtr);
}

void loop() {
    static int prevWallStatus = wallStatus;

    wallStatus = (millis() / 200) % 16;

    if (wallStatus != prevWallStatus) {
        refleshFlag = true;
    }

    if (drawFlag) {
        textX1();
        textSprite.pushSprite(60, 50);

        textY1();
        textSprite.pushSprite(60, 99);

        textCoordinateX();
        miniTextSprite.pushSprite(57, 163);

        mainSprite.pushSprite(0, 0);
        drawFlag = false;
    } else {
        textX1();
        textSprite.pushSprite(&display, 60, 50);

        textY1();
        textSprite.pushSprite(&display, 60, 99);

        textCoordinateX();
        miniTextSprite.pushSprite(&display, 57, 163);
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