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
    }

    lidar.read();

    lidar.updateHistogram();
    lidar.calcCov();

    uartForDebug.print("cov | X:");
    uartForDebug.print("\t");
    uartForDebug.print(lidar.covX);
    uartForDebug.print("\t");
    uartForDebug.print(" Y:");
    uartForDebug.println(lidar.covY);
}
