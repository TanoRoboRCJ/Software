#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <Arduino.h>
#include <Wire.h>

#include "./sensor.h"
#include "./actuator.h"

extern HardwareSerial uart1;
extern HardwareSerial uart3;

void initUART(void);
void initI2C(void);
void initDevice(void);

#endif
