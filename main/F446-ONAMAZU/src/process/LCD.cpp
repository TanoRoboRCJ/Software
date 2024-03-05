#include "./LCD.h"

void LCD::begin() {
    // flag
    char upper[15] = "Hello world";

    uart4.write(250);
    for (int i = 0; i < 40; i++) {
        uart4.write(upper[i]);
    }
}

void LCD::writeRunningStatus(void) {
    char upper[15] = "Hello world";

    uart4.write(250);
    for (int i = 0; i < 40; i++) {
        uart4.write(upper[i]);
    }
}