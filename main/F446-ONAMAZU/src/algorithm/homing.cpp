#include "homing.h"

Homing homing;

int Homing::homingWeighting(void) {
    int weight[4] = {0};

    weight[RIGHT] = homingRightWeight();
    weight[FRONT] = homingFrontWeight();
    weight[LEFT]  = homingLeftWeight();
    weight[BACK]  = homingBackWeight();

    if (tof.rightWallExists == true) {
        weight[RIGHT] = DISABLE * 10;
    }
    if (tof.frontWallExists == true) {
        weight[FRONT] = DISABLE * 10;
    }
    if (tof.leftWallExists == true) {
        weight[LEFT] = DISABLE * 10;
    }
    if (tof.behindWallExists == true) {
        weight[BACK] = DISABLE * 10;
    }

    if (weight[RIGHT] <= weight[FRONT] && weight[RIGHT] <= weight[LEFT] &&
        weight[RIGHT] <= weight[BACK]) {  // NOTE 同列の場合は右優先
        return 0;                         // right
    } else if (weight[FRONT] <= weight[RIGHT] &&
               weight[FRONT] <= weight[LEFT]
               && weight[FRONT] <= weight[BACK]) {
        return 1;  // front
    } else if (weight[LEFT] <= weight[RIGHT] && weight[LEFT] <= weight[FRONT]
               && weight[LEFT] <= weight[BACK]) {
        return 2;  // left
    }else if (weight[BACK] <= weight[RIGHT] && weight[BACK] <= weight[FRONT]
               && weight[BACK] <= weight[LEFT]) {
        return 3;  // back
    }
}

int Homing::homingRightWeight(void) {
    int x = location.x + FIELD_ORIGIN;
    int y = location.y + FIELD_ORIGIN;

    int weight = 0;

    if (gyro.direction == WEST && tof.wallExists[NORTH] == false) {
        weight = (abs(location.x) + abs(location.y + 1)) * 5 +
                 homingReachedCount[x][y + 1] * 20;

    } else if (gyro.direction == NORTH && tof.wallExists[EAST] == false) {
        weight = (abs(location.x + 1) + abs(location.y)) * 5 +
                 homingReachedCount[x + 1][y] * 20;

    } else if (gyro.direction == EAST && tof.wallExists[SOUTH] == false) {
        weight = (abs(location.x) + abs(location.y - 1)) * 5 +
                 homingReachedCount[x][y - 1] * 20;

    } else if (gyro.direction == SOUTH && tof.wallExists[WEST] == false) {
        weight = (abs(location.x - 1) + abs(location.y)) * 5 +
                 homingReachedCount[x - 1][y] * 20;
    }
    if (weight > 1000) {
        weight = 1000;
    }
    return weight;
}

int Homing::homingFrontWeight(void) {
    int x = location.x + FIELD_ORIGIN;
    int y = location.y + FIELD_ORIGIN;

    int weight = 0;

    if (gyro.direction == WEST && tof.wallExists[WEST] == false) {
        weight = (abs(location.x - 1) + abs(location.y)) * 5 +
                 homingReachedCount[x - 1][y] * 100;

    } else if (gyro.direction == NORTH && tof.wallExists[NORTH] == false) {
        weight = (abs(location.x) + abs(location.y + 1)) * 5 +
                 homingReachedCount[x][y + 1] * 100;

    } else if (gyro.direction == EAST && tof.wallExists[EAST] == false) {
        weight = (abs(location.x + 1) + abs(location.y)) * 5 +
                 homingReachedCount[x + 1][y] * 100;

    } else if (gyro.direction == SOUTH && tof.wallExists[SOUTH] == false) {
        weight = (abs(location.x) + abs(location.y - 1)) * 5 +
                 homingReachedCount[x][y - 1] * 100;
    }
    if (weight > 1000) {
        weight = 1000;
    }

    return weight;
}

int Homing::homingLeftWeight(void) {
    int x = location.x + FIELD_ORIGIN;
    int y = location.y + FIELD_ORIGIN;

    int weight = 0;

    if (gyro.direction == WEST && tof.wallExists[SOUTH] == false) {
        weight = (abs(location.x) + abs(location.y - 1)) * 5 +
                 homingReachedCount[x][y - 1] * 100;

    } else if (gyro.direction == NORTH && tof.wallExists[WEST] == false) {
        weight = (abs(location.x - 1) + abs(location.y)) * 5 +
                 homingReachedCount[x - 1][y] * 100;

    } else if (gyro.direction == EAST && tof.wallExists[NORTH] == false) {
        weight = (abs(location.x) + abs(location.y + 1)) * 5 +
                 homingReachedCount[x][y + 1] * 100;

    } else if (gyro.direction == SOUTH && tof.wallExists[EAST] == false) {
        weight = (abs(location.x + 1) + abs(location.y)) * 5 +
                 homingReachedCount[x + 1][y] * 100;
    }
    if (weight > 1000) {
        weight = 1000;
    }

    return weight;
}

int Homing::homingBackWeight(void) {
    int x = location.x + FIELD_ORIGIN;
    int y = location.y + FIELD_ORIGIN;

    int weight = 0;

    if (gyro.direction == WEST && tof.wallExists[EAST] == false) {
        weight = (abs(location.x + 1) + abs(location.y)) * 5 +
                 homingReachedCount[x + 1][y] * 100;

    } else if (gyro.direction == NORTH && tof.wallExists[SOUTH] == false) {
        weight = (abs(location.x) + abs(location.y - 1)) * 5 +
                 homingReachedCount[x][y - 1] * 100;

    } else if (gyro.direction == EAST && tof.wallExists[WEST] == false) {
        weight = (abs(location.x - 1) + abs(location.y)) * 5 +
                 homingReachedCount[x - 1][y] * 100;

    } else if (gyro.direction == SOUTH && tof.wallExists[NORTH] == false) {
        weight = (abs(location.x) + abs(location.y + 1)) * 5 +
                 homingReachedCount[x][y + 1] * 100;
    }
    if (weight > 1000) {
        weight = 1000;
    }

    return weight;
}