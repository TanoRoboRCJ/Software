#include <Arduino.h>

#define LIDAR_BAUDRATE 230400
#define LIDAR_POINT_BUFFER_SIZE 500

#define LIDAR_DATA_LENGTH 47
#define LIDAR_DATA_START_CHAR 0x54


class POINT {
   public:
    double x = 0;
    double y = 0;
};

class LIDAR {
   public:
    HardwareSerial *uartPtr;
    LIDAR(HardwareSerial *_uartPtr, int _rxPin, int _pwmPin);

    POINT point[LIDAR_POINT_BUFFER_SIZE];

    void read(void);
   private:
    int pwmPin;
};