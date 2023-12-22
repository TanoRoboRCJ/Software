#include <Arduino.h>
#include <Wire.h>

class TCS34725 {
   private:
    const char Address = 0x29;
    TwoWire *i2cPtr;

   public:
    TCS34725(TwoWire *wire) {
        this->i2cPtr = wire;
    }

    void init() {
        i2cPtr->begin();
        i2cPtr->beginTransmission(Address);

        delay(100);
        i2cPtr->write(0b10000000 | 0x12);
        i2cPtr->endTransmission(false);
        i2cPtr->requestFrom(Address, 1);

        delay(100);

        if (i2cPtr->available() >= 0) {
            id = i2cPtr->read();
        }
    }

    char id = NULL;
};

TCS34725 colorSensor(&Wire);

void setup() {
    Serial.begin(115200);
    colorSensor.init();
    Serial.print("TCS34725固有ID:");
    Serial.println(colorSensor.id, HEX);
}

void loop() {
    // Serial.println("loop");
}