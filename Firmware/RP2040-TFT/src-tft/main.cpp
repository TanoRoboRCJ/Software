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
    int16_t rc =
        png.openFLASH((uint8_t *)KuyopoyoImg, sizeof(KuyopoyoImg), pngDraw);
    if (rc == PNG_SUCCESS) {
        Serial.println("Successfully opened png file");
        Serial.printf("image specs: (%d x %d), %d bpp, pixel type: %d\n",
                      png.getWidth(), png.getHeight(), png.getBpp(),
                      png.getPixelType());

        for (int y = 0; y < png.getHeight(); y++) {  // 1行ずつデコード
            rc = png.decode(NULL, y);
            if (rc != PNG_SUCCESS) break;
        }
    } else {
        Serial.printf("Failed to open png file: %d\n", rc);
    }

    Serial.print("hello:");
    Serial.println(neko);
    // delay(3000);
    // display.tftPtr->fillScreen(random(0x10000));
}
