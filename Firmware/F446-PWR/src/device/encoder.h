#ifndef _ENCODER_H_
#define _ENCODER_H_

#include <Arduino.h>
#include "../kit/IO-Kit.h"

class Encoder {
   private:
    Output encRGB[3] = {Output(PB4), Output(PB5), Output(PB6)};
    Input encSW = Input(PB7);

    int8_t state;
    int8_t prevState;

    const uint8_t Cyan[3] = {0, 255, 255};
    const uint8_t White[3] = {255, 255, 255};
    const uint8_t Blue[3] = {0, 0, 255};

   public:
    Encoder(void);

    void bootIllumination(void);
    void read(void);
};

#endif  // _ENCODER_H_