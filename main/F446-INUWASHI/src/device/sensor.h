#ifndef _SENSOR_H_
#define _SENSOR_H_

#include "./sensor/camera.h"
#include "./sensor/distanceSensor.h"
#include "./sensor/floorSensor.h"
#include "./sensor/gyro.h"
#include "./sensor/loadcell.h"
#include "./sensor/switch.h"

extern Adafruit_BNO055 bno;
extern Adafruit_NeoPixel stripFloor;

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