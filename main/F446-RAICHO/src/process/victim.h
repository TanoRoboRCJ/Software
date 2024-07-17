#ifndef _VICTIM_H_
#define _VICTIM_H_

#include "../device/device.h"
#include "./location.h"
#include "../algorithm/exploring.h"

extern Location location;

#define H 0
#define S 1
#define U 2
#define R 3
#define G 4
#define Y 5

// #define FIELD_3D_VICTIM

#ifndef FIELD_3D_VICTIM

class VICTIM {
   public:
    bool isDetected = false;
    int isRightOrLeft = NONE;  // RIGHT: 0 LEFT: 2 NONE: 5
    int id = 0;

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
            // uart1.print(camera[i].data);
            if (camera[i].data != 'N') {
                this->id = camera[i].data;
                timer = millis();

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

#else

class VICTIM {
   private:
    static const int FloorNum = 2;

   public:
    bool isDetected = false;
    int isRightOrLeft = NONE;  // RIGHT: 0 LEFT: 2 NONE: 5
    int id = 0;

    // char _kindOfVictimX[FIELD_ORIGIN * 2][FIELD_ORIGIN * 2][3] = {0};
    // char _kindOfVictimY[FIELD_ORIGIN * 2][FIELD_ORIGIN * 2][3] = {0};

    struct _kindOfVictimX_ {
        char val = 0;
        int8_t z = -120;
    };
    struct _kindOfVictimY_ {
        char val = 0;
        int8_t z = -120;
    };

    struct _kindOfVictimX_ _kindOfVictimX[FIELD_ORIGIN * 2][FIELD_ORIGIN * 2]
                                         [FloorNum];
    struct _kindOfVictimY_ _kindOfVictimY[FIELD_ORIGIN * 2][FIELD_ORIGIN * 2]
                                         [FloorNum];

    char returnValOfVictimY(int x, int y) {
        int Z_range = 150;
        for (int i = 0; i < FloorNum; i++) {
            if (_kindOfVictimY[x][y][i].z <
                    (location.coordinateZ + Z_range) / 10 &&
                _kindOfVictimY[x][y][i].z >
                    (location.coordinateZ - Z_range) / 10) {
                return _kindOfVictimY[x][y][i].val;
            }
        }

        return 0;
    }

    char returnValOfVictimX(int x, int y) {
        int Z_range = 150;
        for (int i = 0; i < FloorNum; i++) {
            if (_kindOfVictimX[x][y][i].z <
                    (location.coordinateZ + Z_range) / 10 &&
                _kindOfVictimX[x][y][i].z >
                    (location.coordinateZ - Z_range) / 10) {
                return _kindOfVictimX[x][y][i].val;
            }
        }

        return 0;
    }

    bool returnKindOfvictimY(int x, int y, int id) {
        char tmp = returnValOfVictimY(x, y);
        for (int i = 0; i < id; i++) {
            tmp = tmp >> 1;
        }
        tmp &= 0x01;

        return tmp;
    }

    bool returnKindOfvictimX(int x, int y, int id) {
        char tmp = returnValOfVictimX(x, y);
        for (int i = 0; i < id; i++) {
            tmp = tmp >> 1;
        }
        tmp &= 0x01;

        return tmp;
    }

    void setKindOfvictimY(int x, int y, int id) {
        int Z_range = 150;
        for (int i = 0; i < FloorNum; i++) {
            if (_kindOfVictimY[x][y][i].z <
                    (location.coordinateZ + Z_range) / 10 &&
                _kindOfVictimY[x][y][i].z >
                    (location.coordinateZ - Z_range) / 10) {
                _kindOfVictimY[x][y][i].val |= (1 << id);
                return;
            }
        }

        for (int i = 0; i < FloorNum; i++) {
            if (_kindOfVictimY[x][y][i].val == 0) {
                _kindOfVictimY[x][y][i].z = (location.coordinateZ / 10);
                _kindOfVictimY[x][y][i].val |= (1 << id);

                return;
            }
        }
        // _kindOfVictimY[x][y] |= (1 << id);
    }

    void setKindOfvictimX(int x, int y, int id) {
        // _kindOfVictimX[x][y] |= (1 << id);
        int Z_range = 150;
        for (int i = 0; i < FloorNum; i++) {
            if (_kindOfVictimX[x][y][i].z <
                    (location.coordinateZ + Z_range) / 10 &&
                _kindOfVictimX[x][y][i].z >
                    (location.coordinateZ - Z_range) / 10) {
                _kindOfVictimX[x][y][i].val |= (1 << id);
                return;
            }
        }

        for (int i = 0; i < FloorNum; i++) {
            if (_kindOfVictimX[x][y][i].val == 0) {
                _kindOfVictimX[x][y][i].z = (location.coordinateZ / 10);
                _kindOfVictimX[x][y][i].val |= (1 << id);

                return;
            }
        }
    }

    void read(void) {
        // CHECK: このtimerなんなん
        static unsigned long timer = 0;
        for (int i = 0; i < 2; i++) {
            camera[i].read();
            // uart1.print(camera[i].data);
            if (camera[i].data != 'N') {
                this->id = camera[i].data;
                timer = millis();

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

#endif