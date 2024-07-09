#include <Arduino.h>

#include "display.h"

#include "ui_kit/img/kuyopoyo.h"
#include "ui_kit/img/ui_image/ui_image.h"

// #include "TFT_eSPI.h"

LGFX_Sprite mainSprite(&display);
LGFX_Sprite textSprite(&mainSprite);

float scale = 1;

void setup() {
    display.init();
    display.fillScreen(TFT_BLACK);
    display.setRotation(1);
    display.setColorDepth(16);

    mainSprite.createSprite(240, 240);
    mainSprite.drawJpg((std::uint8_t*)KuyopoyoImg,
                       (unsigned int)KuyopoyoImg_len);
    mainSprite.pushSprite(0, 0);

    textSprite.createSprite(120 / scale, 50 / scale);
}

void loop() {
    // mainSprite.fillScreen(TFT_WHITE);
    // mainSprite.pushSprite(0, 0);
    // delay(100);

    // mainSprite.fillScreen(TFT_RED);
    // mainSprite.pushSprite(0, 0);
    // delay(100);

    // mainSprite.fillScreen(TFT_BLUE);
    // mainSprite.pushSprite(0, 0);
    // delay(100);

    // mainSprite.fillScreen(TFT_GREEN);
    // mainSprite.pushSprite(0, 0);
    // delay(100);

    mainSprite.drawJpg((std::uint8_t*)KuyopoyoImg,
                       (unsigned int)KuyopoyoImg_len);
    mainSprite.pushSprite(0, 0);
    delay(10);

    // for (int i = 0; i < 100; i++) {
    //     textSprite.fillSprite(TFT_WHITE);
    //     textSprite.setTextColor(TFT_BLACK);
    //     textSprite.setTextSize(0.5 / scale);
    //     textSprite.setFont(&fonts::Font8);
    //     textSprite.setCursor(0, 0);
    //     textSprite.print(i);

    //     float matrix1[6] = {
    //         scale,  // 横2倍
    //         0,      // 横傾き
    //         60,     // X座標100
    //         0,      // 縦傾き
    //         scale,  // 縦3倍
    //         60      // Y座標10
    //     };
    //     textSprite.pushAffineWithAA(&display, matrix1);

    //     textSprite.fillSprite(TFT_WHITE);
    //     textSprite.setTextColor(TFT_BLACK);
    //     textSprite.setTextSize(0.5 / scale);
    //     textSprite.setFont(&fonts::Font8);
    //     textSprite.setCursor(0, 0);
    //     textSprite.print(i);

    //     float matrix2[6] = {
    //         scale,  // 横2倍
    //         0,      // 横傾き
    //         60,     // X座標100
    //         0,      // 縦傾き
    //         scale,  // 縦3倍
    //         100     // Y座標10
    //     };
    //     textSprite.pushAffineWithAA(&display, matrix2);

    //     textSprite.fillSprite(TFT_WHITE);
    //     textSprite.setTextColor(TFT_BLACK);
    //     textSprite.setTextSize(0.5 / scale);
    //     textSprite.setFont(&fonts::Font8);
    //     textSprite.setCursor(0, 0);
    //     textSprite.print(i);

    //     float matrix3[6] = {
    //         scale,  // 横2倍
    //         0,      // 横傾き
    //         60,     // X座標100
    //         0,      // 縦傾き
    //         scale,  // 縦3倍
    //         140     // Y座標10
    //     };
    //     textSprite.pushAffineWithAA(&display, matrix3);
    // }
}
