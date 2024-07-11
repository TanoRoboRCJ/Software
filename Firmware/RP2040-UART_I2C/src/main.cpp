#include "Wire.h"

#define I2C_ADDR 0x01  // スレーブ側に指定するI2Cアドレス
#define I2C_SDA 6      // SDAにはGP0を使用する
#define I2C_SCL 7      // SCLにはGP1を使用する

char dataBuf[9] = {0};

// マスターから要求があれば返答する。
void onRequest() {
    for (int i = 0; i < 9; i++) {
        Wire.write(dataBuf[i]);
    }
}

void setup() {
    pinMode(D10, OUTPUT);
    analogWrite(D10, 0);
    Wire.setSDA(I2C_SDA);
    Wire.setSCL(I2C_SCL);
    Wire.onRequest(onRequest);
    Wire.begin(I2C_ADDR);

    Serial1.begin(1000000);

    for (int i = 0; i < 100; i++)
    {
        analogWrite(D10, i);
        delay(3);
    }
    

    for(int i = 0; i < 9; i++) {
        dataBuf[i] = 0;
    }
}

void loop() {
    if (Serial1.available() >= 14) {
        char data[12] = {0};

        if (Serial1.read() == 'P') {
            if (Serial1.read() == 'W') {
                for (int i = 0; i < 12; i++) {
                    data[i] = Serial1.read();
                }

                dataBuf[0] = data[3];
                dataBuf[1] = data[4];
                dataBuf[2] = data[5];
                dataBuf[3] = data[6];
                dataBuf[4] = data[7];
                dataBuf[5] = data[8];
                dataBuf[6] = data[9];
                dataBuf[7] = data[10];
                dataBuf[8] = data[11];
            }
        }

        while (Serial1.available() > 0) {
            Serial1.read();
        }
    }
}
