#include "ui_kit.h"
#include "./device/display.h"

extern TFT_eSPI tft;
extern TFT_eSprite sprite;
extern DISPLAY_DEVICE display;


void UI_KIT::init(void) {
    display.init();

    // display.createSprite();
    // display.setBackgroundImage(bootImage);
    // // display.setBackgroundImagePNG((uint8_t*)KuyopoyoImg, KuyopoyoImg_len);
    // // display.tftPtr->fillScreen(TFT_WHITE);
    // display.publish();
}

void UI_KIT::publish(void) {
    display.publish();
}