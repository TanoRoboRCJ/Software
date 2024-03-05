#ifndef _BOTTOM_H_
#define _BOTTOM_H_

#include <Arduino.h>

class Bottom {
   private:
    HardwareSerial *serialPtr;

   public:
    Bottom(HardwareSerial *ptr);

    bool read(void);
    void write(void);

    // receive
    int tcsClear[2] = {0, 0};
    int tcsRed[2] = {0, 0};
    int tcsGreen[2] = {0, 0};
    int tcsBlue[2] = {0, 0};
    int tof[2] = {0, 0};

    // write
    bool rescueKit[2] = {true, true};
    char ToggleBrightness = 100;
    char LED_color[3] = {0, 50, 255};
};

#endif  // _BOTTOM_H_