#include <Arduino.h>

#include "./lidar.h"
HardwareSerial uart2(PA3, PA2);
LIDAR lidar(&uart2, PA6);

#include <IO-Kit.h>
Output ledBuiltin = Output(PB12);
Output BOOT1 = Output(PB2);

HardwareSerial uartForDebug(PA10, PA9);

HardwareSerial uart4(PA1, PA0);
HardwareSerial uart5(PD2, PC12);

bool chickPeep() {
    return ((millis() / 20) % 2 == 0 && (millis() / 300) % 5 == 0);
}

void setup() {
    uartForDebug.begin(115200);

    uart4.begin(1000000);
    uart5.begin(1000000);

    BOOT1 = LOW;
}

void loop() {
    ledBuiltin = chickPeep();

    static int val[16] = {0};
    static int checkDegit = 0;

    if (uart4.available() >= 35) {
        static int _valTemp[16];
        if (uart4.read() == 'V') {
            if (uart4.read() == 'L') {
                for (int i = 0; i < 16; i++) {
                    char data[2];
                    data[0] = uart4.read();
                    data[1] = uart4.read();

                    // checkDegit += data[0] + data[1];

                    val[i] = (data[0] << 8) + data[1];
                }
            }
        }
        checkDegit = uart4.read();

        // uartForDebug.println(checkDegit);

        uart5.write('V');
        uart5.write('L');
        for (int i = 0; i < 16; i++) {
            uart5.write(highByte(val[i]) & 0xFF);
            uart5.write(lowByte(val[i]) & 0xFF);
        }
        uart5.write(checkDegit);

        float floatCovX = lidar.covX;
        uint8_t *bytePtrX = reinterpret_cast<uint8_t *>(&floatCovX);
        for (size_t i = 0; i < sizeof(float); i++) {
            uart5.write(bytePtrX[i]);
        }

        float floatCovY = lidar.covY;
        uint8_t *bytePtrY = reinterpret_cast<uint8_t *>(&floatCovY);
        for (size_t i = 0; i < sizeof(float); i++) {
            uart5.write(bytePtrY[i]);
        }

        if (lidar.rightWallExists && lidar.leftWallExists) {
            uart5.write('A');
        } else if (lidar.rightWallExists) {
            uart5.write('R');
        } else if (lidar.leftWallExists) {
            uart5.write('L');
        } else {
            uart5.write('N');
        }
    }

    static int gyroDeg = 0;

    if (uart5.available() >= 8) {
        if (uart5.read() == 'L') {
            if (uart5.read() == 'C') {
                gyroDeg = uart5.read() << 8 | uart5.read();

                int trash;
                trash = uart5.read() << 8 | uart5.read();
                trash = uart5.read() << 8 | uart5.read();
            }
        }

        while (uart5.available() > 0) {
            uart5.read();
        }
    }

    lidar.read(gyroDeg);
    lidar.updateHistogram();
    lidar.judgeWall();

    static int count = 0;
    if (count++ % 10 != 0) {
        return;
    }

    lidar.calcCov();

    // gyro

    uartForDebug.print("gyro:");
    uartForDebug.print("\t");
    uartForDebug.print(gyroDeg);

    uartForDebug.print("\t");
    uartForDebug.print("wall | R:");
    uartForDebug.print("\t");
    uartForDebug.print(lidar.rightWallExists);
    uartForDebug.print("\t");
    uartForDebug.print(" L:");
    uartForDebug.print(lidar.leftWallExists);
    uartForDebug.print(" cov | X:");
    uartForDebug.print("\t");
    uartForDebug.print(lidar.covX);
    uartForDebug.print("\t");
    uartForDebug.print(" Y:");
    uartForDebug.println(lidar.covY);
}
