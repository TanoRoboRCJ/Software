#include "display.h"

#include <functional>

DISPLAY_DEVICE::DISPLAY_DEVICE(TFT_eSPI* tftPtr, TFT_eSprite* spritePtr) {
    pinMode(backlightPin, OUTPUT);

    digitalWrite(backlightPin, HIGH);

    this->tftPtr = tftPtr;
    this->spritePtr = spritePtr;
}

void DISPLAY_DEVICE::init(void) {
    setSPIClockFast();

    spritePtr->createSprite(320, 240);
    spritePtr->setColorDepth(5);

    tftPtr->begin();
    tftPtr->setRotation(3);

    pinMode(TFT_CS, OUTPUT);
}

void DISPLAY_DEVICE::setSPIClockFast(void) {
    SPI.beginTransaction(SPISettings(SPI_FREQUENCY, MSBFIRST, TFT_SPI_MODE));
}

void DISPLAY_DEVICE::setBackgroundImage(const uint16_t* imagePtr) {
    spritePtr->pushImage(0, 0, 320, 240, imagePtr);
}

void DISPLAY_DEVICE::setBackgroundImagePNG(uint8_t* imagePtr, int imageLen) {
    if (!spritePtr) {
        Serial.println("Error: spritePtr is not initialized.");
        return;
    }

    int16_t xpos = 0;
    int16_t ypos = 0;

    // pngDrawをメンバ関数として定義
    std::function<void(PNGDRAW*)> pngDrawCallback = [this](PNGDRAW* pDraw) {
        this->pngDraw(pDraw);
    };
    png.openRAM(imagePtr, imageLen, pngDrawCallback.target<void(PNGDRAW*)>());

    if (png.decode(NULL, 0) == PNG_SUCCESS) {
        Serial.println("Successfully opened png file");
        Serial.printf("image specs: (%d x %d), %d bpp, pixel type: %d\n",
                      png.getWidth(), png.getHeight(), png.getBpp(),
                      png.getPixelType());
        Serial.printf("image data length: %d\n", imageLen);
        Serial.printf("First few bytes of image data: %02X %02X %02X %02X\n",
                      imagePtr[0], imagePtr[1], imagePtr[2], imagePtr[3]);

        spritePtr->pushSprite(0, 0);
    } else {
        Serial.printf("Error opening PNG file: %s\n", png.getLastError());
    }
}

void DISPLAY_DEVICE::pngDraw(PNGDRAW* pDraw) {
    uint16_t* lineBuffer = (uint16_t*)malloc(png.getWidth() * sizeof(uint16_t)); // 動的にメモリを確保

    Serial.printf("pngDraw: y=%d, width=%d\n", pDraw->y, pDraw->iWidth);
    png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
    Serial.printf("lineBuffer: %04X %04X %04X...\n", lineBuffer[0], lineBuffer[1], lineBuffer[2]);

    spritePtr->pushImage(0, 0 + pDraw->y, pDraw->iWidth, 1, lineBuffer);

    free(lineBuffer); // メモリを解放
}

// void DISPLAY_DEVICE::setBackgroundImagePNG(uint8_t* imagePtr, int imageLen) {
//     if (!spritePtr) {  // spritePtrが有効でない場合はエラー
//         Serial.println("Error: spritePtr is not initialized.");
//         return;
//     }

//     PNG png;
//     int16_t xpos = 0;
//     int16_t ypos = 0;

//     std::function<void(PNGDRAW*)> pngDraw =
//         [this, png](PNGDRAW* pDraw) mutable {  // mutableキーワードを追加
//             Serial.println("pngDraw called");
//             uint16_t lineBuffer[MAX_IMAGE_WIDTH * 2];
//             png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN,
//                                 0xffffffff);
//             Serial.println(lineBuffer[0], HEX);
//             spritePtr->pushImage(0, 0 + pDraw->y, pDraw->iWidth, 1,
//             lineBuffer);
//         };

//     int16_t rc =
//         png.openRAM(imagePtr, imageLen, pngDraw.target<void(PNGDRAW*)>());

//     if (rc == PNG_SUCCESS) {
//         Serial.println("Successfully opened png file");
//         Serial.printf("image specs: (%d x %d), %d bpp, pixel type: %d\n",
//                       png.getWidth(), png.getHeight(), png.getBpp(),
//                       png.getPixelType());

//         spritePtr->pushSprite(0, 0);  // スプライトをTFTに転送
//     } else {
//         Serial.printf("Error opening PNG file: %s\n", png.getLastError());
//         Serial.printf("Image data length: %d\n",
//                       imageLen);  // 画像データ長を出力
//         Serial.printf("First few bytes of image data: %02X %02X %02X %02X\n",
//                       imagePtr[0], imagePtr[1], imagePtr[2],
//                       imagePtr[3]);  // 先頭の数バイトを出力
//     }
// }

void DISPLAY_DEVICE::publish(int x, int y) {
    setSPIClockFast();
    spritePtr->pushSprite(x, y);
    spritePtr->deleteSprite();
    // digitalWrite(backlightPin, LOW);
}

void DISPLAY_DEVICE::createSprite(int x, int y) {
    spritePtr->createSprite(x, y);
    spritePtr->setColorDepth(5);
}