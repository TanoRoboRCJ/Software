#include "./LCD.h"

void LCD::begin() {
    // flag
    uart4.write('L');
    uart4.write('C');

    uart4.write(highByte((int)gyro.deg));
    uart4.write(lowByte((int)gyro.deg));

    uart4.write(highByte((int)location.coordinateX));
    uart4.write(lowByte((int)location.coordinateX));

    uart4.write(highByte((int)location.coordinateY));
    uart4.write(lowByte((int)location.coordinateY));
}

void LCD::writeRunningStatus(void) {
    uart4.write('L');
    uart4.write('C');

    uart4.write(highByte((int)gyro.deg));
    uart4.write(lowByte((int)gyro.deg));

    uart4.write(highByte((int)location.coordinateX));
    uart4.write(lowByte((int)location.coordinateX));

    uart4.write(highByte((int)location.coordinateY));
    uart4.write(lowByte((int)location.coordinateY));
}