#include "homing.h"

Homing homing;

int Homing::compareLocation(int x, int y) {
    static int i = 200;
        int oldX = location.route[i].x;
        int oldY = location.route[i].y;

        if(oldX == x && oldY == y){
            return 0;
        }else if(oldX == x && oldY == y + 1){
            return 1;
        }else if(oldX == x && oldY == y - 1){
            return 2;
        }else if(oldX == x + 1 && oldY == y){
            return 3;
        }else if(oldX == x - 1 && oldY == y){
            return 4;
        }
}