#ifndef _LOADCELL_H_
#define _LOADCELL_H_

#include <Arduino.h>

#include "../../kit/IO-Kit.h"

#define LEFT 2
#define NONE 5
#define RIGHT 0

class LOADCELL {
   public:
    Input inputPin[2] = {PC2, PC3};  // 0が左

    int load[2] = {0, 0};

    int offset[2];

    int status = NONE;
    int moment = 0;

    void read(void) {
        for (int i = 0; i < 2; i++) {
            load[i] = inputPin[i].raw();
        }

        for (int i = 0; i < 2; i++) {
            load[i] -= offset[i];
            load[i] = constrain(load[i], 0, 1024);
            load[i] = constrain(map(load[i], 0, 1024 - offset[i], 0, 200),0,100);

        }

        if (load[0] > 10 || load[1] > 10) {
            moment = degrees(atan2(load[0], load[1]));

            if (moment < 45) {
                status = LEFT;
            } else {
                status = RIGHT;
            }
        } else {
            status = NONE;
        }
    }

    void init(void) {
        for (int i = 0; i < 2; i++) {
            offset[i] = inputPin[i].raw();
        }
    }

   private:
};

#endif