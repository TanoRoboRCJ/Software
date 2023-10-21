#include "lidar.h"

LIDAR::LIDAR(HardwareSerial *_uartPtr, int _rxPin, int _pwmPin) {
    uartPtr = _uartPtr;
    uartPtr->begin(baudrate, SERIAL_8N1, _rxPin);

    // NOTE: 128という数字に特に意味はない
    // とりあえずPWM波を入力すればLiDARのモーターが回ります
    pinMode(_pwmPin, OUTPUT);
    analogWrite(_pwmPin, 128);
}