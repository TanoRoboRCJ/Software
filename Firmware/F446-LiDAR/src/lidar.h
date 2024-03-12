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
    static const int HistogramRange = 900;
    static const int HistogramBinWidth = 10;
    static const int GridSize = 300;
    static const int HistogramLength = HistogramRange * 2 / HistogramBinWidth;

    int refWave[HistogramLength] = {0};

    LIDAR(HardwareSerial *_uartPtr, int _pwmPin);

    void read(const int _gyro);

    POINT point[DataBuffLength];
    HISTOGRAM histogarm[HistogramLength];

    void updateHistogram(void);
    void calcCov(const int _phase = 0);

    double covX = 0;
    double covY = 0;

   private:
    HardwareSerial *uartPtr;
    int pwmPin;
};