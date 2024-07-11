#include <Arduino.h>
#include <TFT_eSPI.h>

#include "display.h"
#include "ui_kit/font/SF-Mono-HeavyItalic60.h"
#include "ui_kit/font/SF-Mono-Medium11.h"
#include "ui_kit/font/SF-Mono-Semibold13.h"
#include "ui_kit/font/SF-Mono-Bold24.h"

#include "ui_kit/img/kuyopoyo.h"
#include "ui_kit/img/ui_image/ui_image.h"

LGFX_Sprite mainSprite(&display);
LGFX_Sprite textSprite(&mainSprite);
LGFX_Sprite miniTextSprite(&mainSprite);
LGFX_Sprite degTextSprite(&mainSprite);

TFT_eSprite sprite = TFT_eSprite(nullptr);
TFT_eSprite miniSprite = TFT_eSprite(nullptr);
TFT_eSprite degSprite = TFT_eSprite(nullptr);

int wallStatus = 0;
volatile bool refleshFlag = false;
volatile bool drawFlag = true;

int coordX = 340;
int coordY = -1602;
int coordZ = 123;
int deg = 36;

/*
（月曜 普通授業）
火 2限 国語 3限 機械運動学
水 1限 数B  2限 ドイツ語
木 1限 計測 2限 制御
金 2限 数D  3限 現代社会と法
（土日）
月 1限 英語A 2限 情報処理 3限 電磁気
火 3限 応用物理
*/

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

    delay(1000);

    mainSprite.createSprite(240, 240);
    mainSprite.drawPng((std::uint8_t*)BgImgPtr[0],
                       (unsigned int)BgImgLenPtr[0]);
}

void textX1(void) {
    String str = (String)((int)round(coordX / 300.0));

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
    String str = (String)((int)round(coordY / 300.0));

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

void textCoordinate(void) {
    String strX = (String)coordX;
    String strY = (String)coordY;
    String strZ = (String)coordZ;

    miniSprite.createSprite(130, 13);
    miniSprite.fillScreen(TFT_WHITE);
    miniSprite.loadFont(SF_Mono_Semibold13_vlw);
    miniSprite.setTextColor(miniSprite.color24to16(0x343438), TFT_WHITE);

    int posX = 20 - miniSprite.textWidth(strX) / 2;
    miniSprite.setCursor(posX, 0);
    miniSprite.print(strX);

    posX = 65 - miniSprite.textWidth(strY) / 2;
    miniSprite.setCursor(posX, 0);
    miniSprite.print(strY);

    posX = 110 - miniSprite.textWidth(strZ) / 2;
    miniSprite.setCursor(posX, 0);
    miniSprite.print(strZ);

    miniTextSprite.createSprite(130, 13);
    uint16_t* imgBufPtr = (uint16_t*)miniSprite.getPointer();
    miniTextSprite.pushImage(0, 0, 130, 13, imgBufPtr);
}

void textDeg(void) {
    String str = ((String)deg);
    str += "°";

    degSprite.createSprite(70, 22);
    degSprite.fillScreen(TFT_WHITE);
    degSprite.loadFont(SF_Mono_Bold24_vlw);
    degSprite.setTextColor(degSprite.color24to16(0x0e0e28), TFT_WHITE);

    int posX = 40 - degSprite.textWidth(str) / 2;
    degSprite.setCursor(posX, 0);
    degSprite.print(str);

    degTextSprite.createSprite(70, 22);
    uint16_t* imgBufPtr = (uint16_t*)degSprite.getPointer();
    degTextSprite.pushImage(0, 0, 70, 22, imgBufPtr);
}

void loop() {
    static int prevWallStatus = wallStatus;

    wallStatus = (millis() / 220) % 16;

    if (wallStatus != prevWallStatus && refleshFlag == false &&
        drawFlag == false) {
        refleshFlag = true;
    }

    if (drawFlag) {
        textX1();
        textSprite.pushSprite(60, 50);

        textY1();
        textSprite.pushSprite(60, 99);

        textCoordinate();
        miniTextSprite.pushSprite(55, 163);

        textDeg();
        degTextSprite.pushSprite(85, 185);

        mainSprite.pushSprite(0, 0);
        drawFlag = false;
    } else {
        textX1();
        textSprite.pushSprite(&display, 60, 50);

        textY1();
        textSprite.pushSprite(&display, 60, 99);

        textCoordinate();
        miniTextSprite.pushSprite(&display, 55, 163);

        textDeg();
        degTextSprite.pushSprite(&display, 85, 185);
    }

    prevWallStatus = wallStatus;
}

void setup1() {
}

void loop1() {
    int _wallStatus = 0;
    if (refleshFlag) {
        _wallStatus = wallStatus;
        mainSprite.createSprite(240, 240);
        mainSprite.drawPng((std::uint8_t*)BgImgPtr[_wallStatus],
                           (unsigned int)BgImgLenPtr[_wallStatus]);

        // delay(250);
        refleshFlag = false;
        drawFlag = true;
    }
}