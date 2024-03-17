#include "device.h"

// CONSTANNT
const int Period = 10;

// COMMUNICATION
HardwareSerial uart1(PA10, PA9);

// BOTTOM UNIT
HardwareSerial uart3(PC5, PB10);
Bottom bottom(&uart3);

// ACTUATOR
Output buzzerPin = Output(PB6);
BUZZER buzzer(&buzzerPin);

HardwareSerial uart5(PD2, PC12);
STS3032 servo(&uart5);

Adafruit_NeoPixel topLED(24, PC1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel rightLED(7, PB13, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leftLED(7, PA15, NEO_GRB + NEO_KHZ800);
LED led(&rightLED, &topLED, &leftLED);

// SENSOR
// #ifdef BNO055_MODE
// Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);
// #else
Adafruit_BNO08x bno08x = Adafruit_BNO08x(-1);
GYRO gyro(&bno08x);
// #endif

HardwareSerial uart4(PA1, PA0);
DISTANCE_SENSOR tof(&uart4);


SWITCH ui;
LOADCELL loadcell;
FLOOR_SENSOR floorSensor;

HardwareSerial uart2(PA3, PA2);
HardwareSerial uart6(PC7, PC6);
CAMERA camera[2] = {CAMERA(&uart2, PB15), CAMERA(&uart6, PB14)};  // 右左

// FUNCTION
void initUART(void) {
    uart1.setRx(PA10);
    uart1.setTx(PA9);
    uart1.begin(115200);
}

void initI2C(void) {
    Wire.setSDA(PB9);
    Wire.setSCL(PB8);
    Wire.begin();
}

void initDevice(void) {
    buzzer.beep();

    led.bootIllumination();

    initI2C();
    initUART();

    loadcell.init();

    gyro.init();
    gyro.setOffset();

    led.initCompleteIllumination();

    buzzer.initCompleteSound();
}