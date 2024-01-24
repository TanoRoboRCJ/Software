#include "device.h"

HardwareSerial uart1(PA10, PA9);
HardwareSerial uart3(PC5, PB10);

const int Period = 10;

// ACTUATOR
Output buzzerPin = Output(PB6);
BUZZER buzzer(&buzzerPin);

HardwareSerial uart5(PD2, PC12);
STS3032 servo(&uart5);

Adafruit_NeoPixel topLED(32, PC1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel rightLED(7, PB13, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leftLED(7, PA15, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel uiLED(14, PB14, NEO_GRB + NEO_KHZ800);
LED led(&rightLED, &topLED, &leftLED, &uiLED);

// SENSOR
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);
Adafruit_NeoPixel stripFloor = Adafruit_NeoPixel(4, PB15, NEO_GRB + NEO_KHZ800);

HardwareSerial uart4(PA1, PA0);
DISTANCE_SENSOR tof(&uart4);

GYRO gyro(&bno);
SWITCH ui;
LOADCELL loadcell;
FLOOR_SENSOR floorSensor;

HardwareSerial uart2(PA3, PA2);
HardwareSerial uart6(PC7, PC6);
CAMERA camera[2] = {CAMERA(&uart6), CAMERA(&uart2)};  // 右左

void initUART(void) {
    uart1.setRx(PA10);
    uart1.setTx(PA9);
    uart1.begin(115200);
    uart3.begin(115200);
}

void initI2C(void) {
    Wire.setSDA(PB9);
    Wire.setSCL(PB8);
    Wire.begin();
}

void initDevice(void) {
    buzzer.bootSound();

    led.bootIllumination();

    initI2C();
    initUART();

    loadcell.init();

    gyro.init();
    gyro.setOffset();

    floorSensor.init();

    led.initCompleteIllumination();
    delay(200);

    // // delay(1`)

    buzzer.bootSound();
}