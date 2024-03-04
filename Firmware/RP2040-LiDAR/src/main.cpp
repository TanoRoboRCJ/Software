#include <Arduino.h>

#include <Wire.h>
#include <PCA95x5.h>
#include <VL53L0X.h>

#include <Adafruit_NeoPixel.h>

#define NUMPIXELS 1
#define NEO_PWR 11  // GPIO11
#define NEOPIX 12   // GPIO12

Adafruit_NeoPixel pixels(NUMPIXELS, NEOPIX, NEO_GRB + NEO_KHZ800);

const int numOfSensors = 16;
const char firstAddr = 0x30;

VL53L0X distanceSensor[numOfSensors];

PCA9555 ioex;

using namespace PCA95x5::Port;

const int xshutPin[] = {P02, P01, P00, P17, P16, P11, P04, P03,
                        P05, P06, P07, P10, P15, P14, P13, P12};

const bool disalbledSensor[16] = {false, false, false, false, false, true,
                                  true,  true,  true,  true,  true,  true,
                                  false,  false, false, false};
bool deviceScanner(void);
void ioexInit(void);
void distanceSensorInit(void);

bool flag = false;

void setup() {
    Serial.begin(115200);
    Serial1.begin(1000000);

    Wire.begin();

    pixels.begin();
    pinMode(NEO_PWR, OUTPUT);
    digitalWrite(NEO_PWR, HIGH);

    delay(10);

    ioexInit();
    distanceSensorInit();

    flag = true;
}

int rawData[12] = {0};

void loop() {
    for (int i = 0; i < numOfSensors; i++) {
        if (disalbledSensor[i]) {
            rawData[i] = 8190;
            continue;
        }

        rawData[i] = distanceSensor[i].readRangeContinuousMillimeters();
    }

    int checkDigit = 0;
    Serial1.write("VL");

    for (int i = 0; i < numOfSensors; i++) {
        Serial.print(rawData[i]);
        Serial.print("\t");
        Serial1.write(highByte(rawData[i]) & 0xFF);
        Serial1.write(lowByte(rawData[i]) & 0xFF);

        checkDigit += highByte(rawData[i]) & 0xFF;
        checkDigit += lowByte(rawData[i]) & 0xFF;
    }
    Serial.println();

    Serial1.write(checkDigit % 256);
}

bool deviceScanner(void) {
    char error, address;
    int nDevices;

    Serial.println("Scanning...");

    nDevices = 0;
    for (address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        if (error == 0) {
            Serial.print("I2C device found at address 0x");
            if (address < 16) Serial.print("0");
            Serial.print(address, HEX);
            Serial.println("  !");

            nDevices++;
        } else if (error == 4) {
            Serial.print("Unknown error at address 0x");
            if (address < 16) Serial.print("0");
            Serial.println(address, HEX);
        }
    }
    if (nDevices == 0)
        Serial.println("No I2C devices found\n");
    else
        Serial.println("done\n");

    return (nDevices == 0);
}

void ioexInit(void) {
    ioex.attach(Wire);
    ioex.polarity(PCA95x5::Polarity::ORIGINAL_ALL);
    ioex.direction(PCA95x5::Direction::OUT_ALL);
    ioex.write(PCA95x5::Level::L_ALL);
}

void setup1() {
}

void loop1() {
    if (flag) {
        if ((millis() / 20) % 2 == 0 && (millis() / 300) % 5 == 0) {
            pixels.setPixelColor(0, 0, 80, 255);
            pixels.setBrightness(255);
            pixels.show();
        } else {
            pixels.setBrightness(0);
            pixels.show();
        }
    }
}

void distanceSensorInit(void) {
    for (int i = 0; i < numOfSensors; i++) {
        if (disalbledSensor[i]) {
            continue;
        }

        ioex.write(static_cast<PCA95x5::Port::Port>(xshutPin[i]),
                   PCA95x5::Level::H);  // VL53L0Xの電源をON
        delay(10);
        distanceSensor[i].setTimeout(100);

        if (!distanceSensor[i].init()) {  // ERROR

            while (1) {
                Serial.print(
                    "ERR1: Failed to detect and initialize sensor!\nPORT:");
                Serial.println(i);
                Serial1.write("ERR1");  // F446にエラーを通知

                pixels.setPixelColor(0, 255, 0, 0);
                pixels.setBrightness(255);
                pixels.show();
                delay(100);

                pixels.setPixelColor(0, 0, 0, 0);
                pixels.setBrightness(255);
                pixels.show();
                delay(100);
            }
        }

        distanceSensor[i].setAddress(firstAddr + i);  // 順にアドレスを割り当て

        // 測定距離を広げる
        distanceSensor[i].setSignalRateLimit(0.1);
        distanceSensor[i].setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
        distanceSensor[i].setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange,
                                              14);

        //    distanceSensor[i].setMeasurementTimingBudget(20000);  //高速化
        //    distanceSensor[i].setMeasurementTimingBudget(200000);//精度重視

        //    distanceSensor[i].setMeasurementTimingBudget(50000);

        distanceSensor[i].startContinuous();  // 計測開始
    }
}
