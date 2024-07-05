#ifndef _VICTIM_H_
#define _VICTIM_H_

#include "../device/device.h"
#include "./location.h"

#define H 0
#define S 1
#define U 2
#define R 3
#define G 4
#define Y 5

class VICTIM {
   public:
    bool isDetected   = false;
    int isRightOrLeft = NONE;  // RIGHT: 0 LEFT: 2 NONE: 5
    int id            = 0;

    // int kindOfVictimX[FIELD_ORIGIN * 2][FIELD_ORIGIN * 2]    = {0};
    // int kindOfVictimY[FIELD_ORIGIN * 2][FIELD_ORIGIN * 2]    = {0};
    char _kindOfVictimX[FIELD_ORIGIN * 2][FIELD_ORIGIN * 2] = {0};
    char _kindOfVictimY[FIELD_ORIGIN * 2][FIELD_ORIGIN * 2] = {0};

    bool returnKindOfvictimY(int x, int y, int id) {
        char tmp = _kindOfVictimY[x][y];
        for (int i = 0; i < id; i++) {
            tmp = tmp >> 1;
        }
        tmp &= 0x01;

        return tmp;
    }

    bool returnKindOfvictimX(int x, int y, int id) {
        char tmp = _kindOfVictimX[x][y];
        for (int i = 0; i < id; i++) {
            tmp = tmp >> 1;
        }
        tmp &= 0x01;

        return tmp;
    }

    void setKindOfvictimY(int x, int y, int id) {
        _kindOfVictimY[x][y] |= (1 << id);
    } 

    void setKindOfvictimX(int x, int y, int id) {
        _kindOfVictimX[x][y] |= (1 << id);
    }

    void read(void) {
        // CHECK: このtimerなんなん
        static unsigned long timer = 0;
        for (int i = 0; i < 2; i++) {
            camera[i].read();
            if (camera[i].data != 'N') {
                this->id = camera[i].data;
                timer    = millis();

                if (i == 0) {
                    this->isRightOrLeft = RIGHT;
                } else {
                    this->isRightOrLeft = LEFT;
                }

                break;
            }
        }
    }

    unsigned long color(int _id) {
        unsigned long returnColor;
        switch (id) {
            case VICTIM_H:
                returnColor = led.pink;
                break;
            case VICTIM_S:
                returnColor = led.blue;
                break;
            case VICTIM_U:
                returnColor = led.white;
                break;
            case VICTIM_RED:
                returnColor = led.red;
                break;
            case VICTIM_YELLOW:
                returnColor = led.yellow;
                break;
            default:
                returnColor = led.green;
                break;
        }

        return returnColor;
    }
};

#endif