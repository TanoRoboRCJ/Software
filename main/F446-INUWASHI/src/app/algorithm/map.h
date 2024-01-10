#ifndef _Map_H
#define _Map_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class Map {
   public:
    static const int White  = 0;
    static const int Black  = 1;
    static const int Blue   = 2;
    static const int Silver = 3;

    static const int unreached = 1000;

    int x = unreached;
    int y = unreached;

    int color   = White;                          // 床色
    bool victim = false;                          // 被災者の有無
    bool wall[4] = {false, false, false, false};  // 北東南西の順, 絶対方位

    int isReached() {
        if (x == unreached) {
            return 0;
        } else {
            return 1;
        }
    };
};

Map map[200];