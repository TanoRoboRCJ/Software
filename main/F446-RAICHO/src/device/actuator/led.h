#ifndef _LED_H_
#define _LED_H_

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define TOP 1
#define RIGHT 0
#define LEFT 2
#define UI 3
#define CAM 4

class LED {
   private:
    // settings:
    const int TopBrightness = 50;
    const int SideBrightness = 20;
    const bool disableAll = false;

    bool disableTop = false;
    bool disableSide = false;
    bool disableUI = false;
    bool disableCam = false;

    const bool* disablePtr[5] = {&disableSide, &disableTop, &disableSide, &disableUI, &disableCam};

    Adafruit_NeoPixel* ptrArr[5];

   public:
    LED(Adafruit_NeoPixel* right, Adafruit_NeoPixel* top,
        Adafruit_NeoPixel* left, Adafruit_NeoPixel* ui, Adafruit_NeoPixel* cam);

    // functions:
    void bootIllumination(void);
    void initCompleteIllumination(void);

    void setColorBar(int position, unsigned long color);

    void setColor(int led, int r, int g, int b);
    void setColor(int led, unsigned long color);
    void setBrightness(int led, int brightness);
    void setBrightnessRaw(int led, int brightness);

    void showAll(void);
    void show(int led);

    // colors:
    const unsigned long pink = colorHSV(0, 125, 255);
    const unsigned long red = colorHSV(0, 255, 255);
    const unsigned long yellow = colorHSV(45, 255, 255);
    const unsigned long green = colorHSV(90, 255, 255);
    const unsigned long cyan = colorHSV(140, 255, 255);
    const unsigned long blue = colorHSV(180, 255, 255);
    const unsigned long white = colorRGB(255, 255, 255);
    const unsigned long lowWhite = colorRGB(255, 255, 255);
    const unsigned long blank = colorHSV(0, 0, 0);

   private:
    // funcitons:
    unsigned long colorRGB(int red, int green, int blue);
    unsigned long colorHSV(int hue, int saturation, int value);
};

#endif