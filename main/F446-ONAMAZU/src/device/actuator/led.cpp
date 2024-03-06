#include "led.h"

#include "../../kit/RTOS-Kit.h"

LED::LED(Adafruit_NeoPixel* right, Adafruit_NeoPixel* top,
         Adafruit_NeoPixel* left) {
    ptrArr[TOP] = top;
    ptrArr[RIGHT] = right;
    ptrArr[LEFT] = left;

    // init
    for (int i = 0; i < 3; i++) {
        ptrArr[i]->begin();
        ptrArr[i]->setBrightness(0);
        ptrArr[i]->clear();
        ptrArr[i]->show();
    }
}

void LED::bootIllumination(void) {
    for (int brightCtr = 0; brightCtr < 255; brightCtr += 4) {
        for (int i = 0; i < 3; i++) {
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

    for (int i = 0; i < 3; i++) {
        setColor(i, blank);
    }

    showAll();
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
    for (int i = 0; i < 3; i++) {
        show(i);
    }
}

void LED::show(int led) {
    if (disableAll || *(disablePtr[led])) return;

    ptrArr[led]->show();
}

void LED::setColorBar(int position, unsigned long color) {
}

void LED::setGlowColor(void) {
    for (int i = 0; i < 3; i++) {
        setBrightness(i, 255);
    }

    setColor(LEFT, blank);
    setColor(RIGHT, blank);
    setColor(TOP, lowWhite);

    for (int i = 8; i < 13; i++) {
        ptrArr[TOP]->setPixelColor(i, white);
    }

    for (int i = 20; i < 25; i++) {
        ptrArr[TOP]->setPixelColor(i, white);
    }
}