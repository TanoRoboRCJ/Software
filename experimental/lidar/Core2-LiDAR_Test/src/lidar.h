#include <Arduino.h>

class POINT {
   public:
    double x = 0;
    double y = 0;
};

class HISTOGRAM {
   public:
    int x = 0;
    int y = 0;
};

class LIDAR {
   public:
    static const int DataBuffLength = 500;
    static const int HistogramRange = 900 * 2;
    static const int HistogramBinWidth = 10;

    LIDAR(HardwareSerial *_uartPtr, int _rxPin, int _pwmPin);

    void read(void);

    POINT point[DataBuffLength];
    HISTOGRAM histogarm[HistogramRange / HistogramBinWidth];

   private:
    HardwareSerial *uartPtr;
    int pwmPin;

    HISTOGRAM _histogram[HistogramRange / HistogramBinWidth];
};