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
    void colorJudgment(void);

    int Color = 0;

   private:
};

#endif