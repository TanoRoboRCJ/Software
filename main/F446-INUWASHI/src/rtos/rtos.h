/**
 * RTOS.h
 * RTOSの大御所的存在
 * 基本あまりいじることはない
 */

#ifndef _RTOS_H_
#define _RTOS_H_

#include "../device/device.h"
#include "../kit/RTOS-Kit.h"
#include "../process/process.h"

// mainApp.cpp
extern void mainApp(App);

// daemon.cpp
extern void startDaemon(void);
extern void sensorApp(App);
extern void servoApp(App);
extern void ledApp(App);
extern void monitorApp(App);

// algorithm.cpp
extern void rightWallApp(App);
extern void adjustmentApp(App);
extern void homingApp(App);

// process.cpp
extern void locationApp(App);
extern void victimNotifyApp(App);

#endif