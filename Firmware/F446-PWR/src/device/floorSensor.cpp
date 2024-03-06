#include "floorSensor.h"

FloorSensor::FloorSensor(TwoWire* _wirePtr) {
    this->wirePtr = _wirePtr;
}

void FloorSensor::init(void) {
    // ID
    wirePtr->beginTransmission(Addr);
    wirePtr->write(0b10000000 | 0x12);
    wirePtr->endTransmission(false);
    wirePtr->requestFrom(Addr, 1);
    if (wirePtr->available()) {
        id = wirePtr->read();
    }

    // 積分時間
    wirePtr->beginTransmission(Addr);
    wirePtr->write(0b10000000 | 0x01);
    wirePtr->write(Exposure);
    wirePtr->endTransmission();

    // ゲイン
    wirePtr->beginTransmission(Addr);
    wirePtr->write(0b10000000 | 0x0F);
    wirePtr->write(Gain);
    wirePtr->endTransmission();

    // Enable
    const uint8_t EnableCommand = 0b00000011;  // 無効: 割り込み,待機 有効:ADC,電源ON
    wirePtr->beginTransmission(Addr);
    wirePtr->write(0b10000000 | 0x00);
    wirePtr->write(EnableCommand);
    wirePtr->endTransmission();

    delay(10);
}

void FloorSensor::read(void) {
    wirePtr->beginTransmission(Addr);
    wirePtr->write(0b10100000 | 0x14);
    wirePtr->endTransmission(false);
    wirePtr->requestFrom(Addr, 8);

    uint8_t crgb[8] = {0};
    if (wirePtr->available() >= 7) {
        for (int i = 0; i < 8; i++) {
            crgb[i] = wirePtr->read();
        }
    }

    clear = crgb[0] | crgb[1] << 8;
    for (int i = 0; i < 3; i++) {
        rgb[i] = crgb[i * 2 + 2] | crgb[i * 2 + 3] << 8;
    }
}