#ifndef _VICTIM_H_
#define _VICTIM_H_

#include "../device/device.h"
#include "./location.h"

class VICTIM {
   public:
    bool isDetected   = false;
    int isRightOrLeft = NONE;  // RIGHT: 0 LEFT: 2 NONE: 5
    int id            = 0;

    bool place[FIELD_ORIGIN * 2][FIELD_ORIGIN * 2]       = {false};
    int kindOfVictimX[FIELD_ORIGIN * 2][FIELD_ORIGIN * 2] = {0};
    int kindOfVictimY[FIELD_ORIGIN * 2][FIELD_ORIGIN * 2] = {0};

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