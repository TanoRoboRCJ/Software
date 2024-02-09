#include "floorSensor.h"

extern Adafruit_NeoPixel stripFloor;

void FLOOR_SENSOR::init(void) {
    stripFloor.begin();
    stripFloor.setBrightness(brightness);
    stripFloor.show();

    pinMode(PC0, INPUT_PULLUP);
}

unsigned long FLOOR_SENSOR::colorRGB(int red, int green, int blue) {
    return stripFloor.Color(red, green, blue);
}

unsigned long FLOOR_SENSOR::colorHSV(int hue, int saturation, int brightness) {
    return stripFloor.ColorHSV(hue * 256, saturation, brightness);
}

void FLOOR_SENSOR::setFloorColor(unsigned long color) {
    for (int i = 0; i < 4; i++) {
        stripFloor.setPixelColor(i, color);
    }
    stripFloor.show();
}

void FLOOR_SENSOR::colorJudgment(void) {
    if (redVal > 910 && blankVal > 910 && blueVal > 910) {
        Color = 1;
        // 黒
    }  else {
        Color = 0;
        // 白
    }//FIXME ここに銀色の判定を入れる
    // else if ( (blankVal > 100 && blankVal < 650) && blueVal < 300) {
    //     color = 2;
    //     // 青
    // } 
   
}

// void FLOOR_SENSOR::tof_of_bottom_read(void) {

// }