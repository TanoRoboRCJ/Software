#ifndef _RTOS_H_
#define _RTOS_H_

#include <Arduino.h>

#include "../device/device.h"
#include "../kit/RTOS-Kit.h"
#include "../neko/exploring.h"


extern void mainApp(App);
extern void sensorApp(App);
extern void servoApp(App);
extern void ledApp(App);
extern void rightWallApp(App);
extern void adjustmentApp(App);
extern void turnRight(void);
extern void turnLeft(void);
extern void turnReverse(void);
extern void move_1tile(void);
extern void locationApp(App);
extern void victimNotifyApp(App);
extern void monitorApp(App);

#endif