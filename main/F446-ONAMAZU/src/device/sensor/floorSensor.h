#ifndef _FLOOR_SENSOR_H_
#define _FLOOR_SENSOR_H_

#include <Arduino.h>

#include "../bottom.h"

extern Bottom bottom;

class FLOOR_SENSOR {
   public:
    int frontCRGB[4];  // Claer R G B
    int backCRGB[4];   // Claer R G B

    void read(void);
    void frontColorJudgment(void);
    void backColorJudgment(void);

    const int WHITE = 0;
    const int BLACK = 1;
    const int BLUE  = 2;
    const int SILVER = 3;

    int checkPointX = 0;
    int checkPointY = 0;;

    int frontColor = 0;
    int backColor  = 0;

   private:
};

#endif