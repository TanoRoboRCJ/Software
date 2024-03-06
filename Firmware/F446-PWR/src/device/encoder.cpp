#include "encoder.h"

Encoder::Encoder(void) {
    show(White);
}

void Encoder::bootIllumination(int startupTime) {
    // while (millis() <= startupTime - 1000) {
    // }

    // for (double i = 1.0; i >= 0.0; i -= 0.01) {
    //     show(White, Cyan, i, 255);
    //     delay(5);
    // }

    show(White, 255);
}

void Encoder::show(const uint8_t color[3], const uint8_t brightness) {
    for (int i = 0; i < 3; i++) {
        encRGB[i] <<= 255 - (color[i] * brightness / 255);
    }
}

void Encoder::show(const uint8_t color1[3], const uint8_t color2[3],
                   const double proportion, const uint8_t brightness) {
    int color[3];
    for (int i = 0; i < 3; i++) {
        color[i] = color1[i] * proportion + color2[i] * (1 - proportion);
        encRGB[i] <<= 255 - (color[i] * brightness / 255);
    }
}

void Encoder::read(void) {
    // NOTE: プログラムかけ

    // static bool isFixed = true;

    // if (encAB[0] && encAB[1] && !isFixed) {
    //     // encAB[0] = digitalRead(ENCODER_A);
    //     if (encAB[0] == encAB[1]) {
    //         isFixed = false;
    //     }
    // }
}