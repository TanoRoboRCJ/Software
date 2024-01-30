#include "homing.h"

Homing homing;

int Homing::compareLocation(int x, int y) {
    int oldX = location.route[exploring.maximumArray - 1].x;
    int oldY = location.route[exploring.maximumArray - 1].y;

    if (x == oldX && y == oldY) {
        return 0;
    } else if (x == oldX && (y + 1) == oldY) {
        return 1;
    } else if ((x + 1) == oldX && y == oldY) {
        return 2;
    } else if (x == oldX && (y - 1) == oldY) {
        return 3;
    } else if ((x - 1) == oldX && y == oldY) {
        return 4;
    } else {
        return 5;
    }
}