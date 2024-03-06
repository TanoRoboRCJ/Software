#ifndef _ENCODER_H_
#define _ENCODER_H_

#include <Arduino.h>
#include "../kit/IO-Kit.h"

class Encoder {
   private:
    Output encRGB[3] = {Output(PB4), Output(PB5), Output(PB6)};

    int8_t state;
    int8_t prevState;

   public:
    Encoder(void);

    void bootIllumination(int startupTime);
    void read(void);
    void show(const uint8_t color[3], const uint8_t brightness = 255);
    void show(const uint8_t color1[3], const uint8_t color2[3],
              const double proportion = 0.5, const uint8_t brightness = 255);

    const uint8_t Cyan[3] = {0, 200, 255};
    const uint8_t White[3] = {255, 255, 255};
    const uint8_t Blue[3] = {0, 0, 255};

    int rotation = 0;

    Input isPressed = Input(PB7);

    Input encAB[2] = {Input(PA14), Input(PA15)};
};

#endif  // _ENCODER_H_