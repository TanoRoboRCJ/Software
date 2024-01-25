#include "homing.h"

Homing homing;

int Homing::compareLocation(int x, int y) {
    int oldX = location.route[exploring.maximumArray].x;
    int oldY = location.route[exploring.maximumArray].y;

    if ((oldX == x) && (oldY == (y + 1))) {
        return 1;
    } else if ((oldX == (x + 1)) && (oldY == y)) {
        return 2;
    } else if ((oldX == x) && (oldY == (y - 1))) {
        return 3;
    } else if ((oldX == (x - 1)) && (oldY == y)) {
        return 4;
    } else if ((oldX == x) && (oldY == y)) {
        return 0;
    }
}