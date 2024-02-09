#include <Arduino.h>

// #include <IO-Kit.h>
// Output ledBuiltin = Output(PB12);
// Output ledL = Output(PB14);
// // Output ledBuiltin = Output(PA0);

// Output encLED[3] = {Output(PB4), Output(PB5), Output(PB6)};
// Input enc[2] = {Input(PA14), Input(PA15)};

// HardwareSerial uart1(PA10, PA9);

// bool chickPeep(void) {
//     return (millis() / 20) % 2 == 0 && (millis() / 300) % 5 == 0;
// }

// void setup() {
//     uart1.begin(115200);
//     for (int i = 0; i < 3; i++) {
//         encLED[i] = 1;
//     }
// }

// void loop() {
//     ledBuiltin = chickPeep();
//     ledL = (millis() % 1000 < 500) * 1;
// }

#include <Wire.h>
#include <Adafruit_TCS34725.h>

/* Initialise with specific int time and gain values */
Adafruit_TCS34725 tcs =
    Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

void setup(void) {
    Serial.begin(115200);

    // I2Cピン設定 I2C0を使用する
    Wire.setSDA(0);
    Wire.setSCL(1);

    if (tcs.begin()) {
        Serial.println("Found sensor");
    } else {
        Serial.println("No TCS34725 found ... check your connections");
        while (1)
            ;
    }
}

void loop(void) {
    uint16_t r, g, b, c;

    tcs.getRawData(&r, &g, &b, &c);

    Serial.print("R: ");
    Serial.print(r, DEC);
    Serial.print(" ");
    Serial.print("G: ");
    Serial.print(g, DEC);
    Serial.print(" ");
    Serial.print("B: ");
    Serial.print(b, DEC);
    Serial.print(" ");
    Serial.println(" ");
}