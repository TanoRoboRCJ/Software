#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <Arduino.h>
#include <Wire.h>

#include "./sensor.h"
#include "./actuator.h"

#include "./bottom.h"

extern Bottom bottom;

extern HardwareSerial uart1;
extern HardwareSerial uart3;

void initUART(void);
void initI2C(void);
void initDevice(void);

extern const int Period;  // 制御周期

//ACTUATOR
extern Output buzzerPin;
extern BUZZER buzzer;

extern HardwareSerial uart5;
extern STS3032 servo;

extern Adafruit_NeoPixel topLED;
extern Adafruit_NeoPixel rightLED;
extern Adafruit_NeoPixel leftLED;
extern LED led;

//SENSOR
//FIXME: これがexternエラーの正体
#ifdef BNO055_MODE
extern Adafruit_BNO055 bno055;
#else
extern Adafruit_BNO08x bno08x;
#endif

extern HardwareSerial uart4;
extern DISTANCE_SENSOR tof;

extern GYRO gyro;
extern SWITCH ui;
extern LOADCELL loadcell;
extern FLOOR_SENSOR floorSensor;

extern HardwareSerial uart2;
extern HardwareSerial uart6;
extern CAMERA camera[2];

#endif
