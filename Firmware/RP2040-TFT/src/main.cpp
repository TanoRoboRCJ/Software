#include <Arduino.h>
#include <TFT_eSPI.h>
#include <Wire.h>

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

int coordX = 0;
int coordY = 0;
int coordZ = 0;
int deg = 0;

char dataBuf[9] = {0};

#define I2C_ADDR 0x01  // スレーブ側に指定するI2Cアドレス
#define I2C_SDA 6      // SDAにはGP0を使用する
#define I2C_SCL 7      // SCLにはGP1を使用する

void textX1(void);
void textY1(void);
void textCoordinate(void);
void textDeg(void);

bool deviceScanner(void) {
    char error, address;
    int nDevices;

    Serial.println("Scanning...");

    nDevices = 0;
    for (address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        if (error == 0) {
            Serial.print("I2C device found at address 0x");
            if (address < 16) Serial.print("0");
            Serial.print(address, HEX);
            Serial.println("  !");

            nDevices++;
        } else if (error == 4) {
            Serial.print("Unknown error at address 0x");
            if (address < 16) Serial.print("0");
            Serial.println(address, HEX);
        }
    }
    if (nDevices == 0)
        Serial.println("No I2C devices found\n");
    else
        Serial.println("done\n");

    return (nDevices == 0);
}

byte dt;

void setup() {
    Wire.setSDA(I2C_SDA);  // I2Cアドレスピンを指定して通信開始
    Wire.setSCL(I2C_SCL);
    Wire.begin();

    Serial.begin(115200);

    display.init();
    display.fillScreen(TFT_BLACK);
    display.setRotation(3);
    display.setColorDepth(16);

    // mainSprite.createSprite(240, 240);
    // mainSprite.drawJpg((std::uint8_t*)KuyopoyoImg,
    //                    (unsigned int)KuyopoyoImg_len);
    // mainSprite.pushSprite(0, 0);

    // delay(1000);

    mainSprite.createSprite(240, 240);
    mainSprite.drawPng((std::uint8_t*)BgImgPtr[0],
                       (unsigned int)BgImgLenPtr[0]);
}

void loop() {
    static int prevWallStatus = wallStatus;

    Wire.requestFrom(I2C_ADDR, 9);
    if (Wire.available() >= 9) {
        for (int i = 0; i < 9; i++) {
            dataBuf[i] = Wire.read();
        }

        coordX = (dataBuf[0] << 8) | dataBuf[1];
        coordY = (dataBuf[2] << 8) | dataBuf[3];
        coordZ = (dataBuf[4] << 8) | dataBuf[5];

        if (coordX >= 32768) {
            coordX -= 65536;
        }

        if (coordY >= 32768) {
            coordY -= 65536;
        }

        if (coordZ >= 32768) {
            coordZ -= 65536;
        }
        deg = (dataBuf[6] << 8) | dataBuf[7];
        wallStatus = dataBuf[8];
    }

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

    Serial.println(wallStatus);
}

void setup1() {
}

void loop1() {
    int _wallStatus = 0;
    static unsigned long timer = 0;

    if (refleshFlag || (timer + 1000 < millis())){
        _wallStatus = wallStatus;
        mainSprite.createSprite(240, 240);
        mainSprite.drawPng((std::uint8_t*)BgImgPtr[_wallStatus],
                           (unsigned int)BgImgLenPtr[_wallStatus]);

        // delay(250);
        refleshFlag = false;
        drawFlag = true;

        timer = millis();
    }
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