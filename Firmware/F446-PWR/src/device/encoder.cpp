#include "encoder.h"

Encoder::Encoder(void) {
    for (int i = 0; i < 3; i++) {
        encRGB[i] <<= 255;
    }
}

void Encoder::bootIllumination(void) {
    for (double brightness = 0; brightness <= 1.0; brightness += 0.05) {
        for (int i = 0; i < 3; i++) {
            encRGB[i] <<= 255 - Blue[i] * brightness;
        }
        delay(10);
    }
}