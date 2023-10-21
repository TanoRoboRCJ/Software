#include <Arduino.h>

class LIDAR {
   public:
    HardwareSerial *uartPtr;
    LIDAR(HardwareSerial *_uartPtr, int _rxPin, int _pwmPin);

   private:
    const int baudrate = 230400;

    int pwmPin = NULL;
};