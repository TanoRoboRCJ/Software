#include "led.h"

#include "../../kit/RTOS-Kit.h"

LED::LED(Adafruit_NeoPixel* right, Adafruit_NeoPixel* top,
         Adafruit_NeoPixel* left, Adafruit_NeoPixel* ui,
         Adafruit_NeoPixel* cam) {
    ptrArr[TOP] = top;
    ptrArr[RIGHT] = right;
    ptrArr[LEFT] = left;
    ptrArr[UI] = ui;
    ptrArr[CAM] = cam;

    // init
    for (int i = 0; i < 5; i++) {
        ptrArr[i]->begin();
        ptrArr[i]->setBrightness(0);
        ptrArr[i]->clear();
        ptrArr[i]->show();
    }
}

void LED::bootIllumination(void) {
    for (int brightCtr = 0; brightCtr < 255; brightCtr += 4) {
        for (int i = 0; i < 5; i++) {
            setBrightness(i, brightCtr);
            setColor(i, white);
            show(i);
        }
        delay(1);
    }
}

void LED::initCompleteIllumination(void) {
    unsigned long timer = millis();
    const int duration = 200;

    for (int i = 0; i < 5; i++) {
        setColor(i, blank);
    }

    showAll();

    // カメラの色温度調整
    setBrightnessRaw(CAM, 100);
    setColor(CAM, colorRGB(255, 0, 255));
    show(CAM);
}

void LED::setColor(int led, int r, int g, int b) {
    ptrArr[led]->fill(ptrArr[led]->Color(r, g, b));
}

void LED::setColor(int led, unsigned long color) {
    ptrArr[led]->fill(color);
}

unsigned long LED::colorRGB(int red, int green, int blue) {
    return ptrArr[0]->Color(red, green, blue);
}

unsigned long LED::colorHSV(int hue, int saturation, int value) {
    return ptrArr[0]->ColorHSV(hue * 256, saturation, value);
}

void LED::setBrightness(int led, int brightness) {
    if (led == TOP)
        ptrArr[led]->setBrightness((int)(brightness * TopBrightness / 255.0));
    else
        ptrArr[led]->setBrightness((int)(brightness * SideBrightness / 255.0));
}

void LED::setBrightnessRaw(int led, int brightness) {
    ptrArr[led]->setBrightness(brightness);
}

void LED::showAll(void) {
    for (int i = 0; i < 4; i++) {
        show(i);
    }
}

void LED::show(int led) {
    if (disableAll || *(disablePtr[led])) return;

    ptrArr[led]->show();
}

void LED::setColorBar(int position, unsigned long color) {
}