#ifndef _ACTUATOR_H_
#define _ACTUATOR_H_

#include "./actuator/buzzer.h"
#include "./actuator/led.h"
#include "./actuator/sts3032.h"

extern Output buzzerPin;
extern BUZZER buzzer;

extern HardwareSerial uart5;
extern STS3032 servo;

extern Adafruit_NeoPixel topLED;
extern Adafruit_NeoPixel rightLED;
extern Adafruit_NeoPixel leftLED;
extern Adafruit_NeoPixel uiLED;
extern LED led;

#endif