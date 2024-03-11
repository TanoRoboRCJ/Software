#include "homing.h"

Homing homing;

int Homing::homingWeighting(void) {
    int weight[3] = {1, 2, 3};  // {right, front, left}

    weight[RIGHT] += homingRightWeight();
    weight[FRONT] += homingFrontWeight();
    weight[LEFT] += homingLeftWeight();

    if (tof.rightWallExists == true) {
        weight[RIGHT] += DISABLE * 10;
    }
    if (tof.frontWallExists == true) {
        weight[FRONT] += DISABLE * 10;
    }
    if (tof.leftWallExists == true) {
        weight[LEFT] += DISABLE * 10;
    }

    if (weight[RIGHT] <= weight[FRONT] && weight[RIGHT] <= weight[LEFT]) {
        return 0;  // right
    } else if (weight[FRONT] <= weight[RIGHT] &&
               weight[FRONT] <= weight[LEFT]) {
        return 1;  // front
    } else if (weight[LEFT] <= weight[RIGHT] && weight[LEFT] <= weight[FRONT]) {
        return 2;  // left
    }
}

int Homing::homingRightWeight(void) {
    int x = location.x + FIELD_ORIGIN;
    int y = location.y + FIELD_ORIGIN;

    int weight = 0;

    if (gyro.direction == WEST && tof.wallExists[NORTH] == false) {
        weight = (abs(location.x) + abs(location.y + 1)) +
                 homingReachedCount[x][y + 1] * 10;

    } else if (gyro.direction == NORTH && tof.wallExists[EAST] == false) {
        weight = (abs(location.x + 1) + abs(location.y)) * 10 +
                 homingReachedCount[x + 1][y] * 10;

    } else if (gyro.direction == EAST && tof.wallExists[SOUTH] == false) {
        weight = (abs(location.x) + abs(location.y - 1)) * 10 +
                 homingReachedCount[x][y - 1] * 10;

    } else if (gyro.direction == SOUTH && tof.wallExists[WEST] == false) {
        weight = (abs(location.x - 1) + abs(location.y)) * 10 +
                 homingReachedCount[x - 1][y] * 10;
    }

    return weight;
}

int Homing::homingFrontWeight(void) {
    int x = location.x + FIELD_ORIGIN;
    int y = location.y + FIELD_ORIGIN;

    int weight = 0;

    if (gyro.direction == WEST && tof.wallExists[WEST] == false) {
        weight = (abs(location.x - 1) + abs(location.y)) +
                 homingReachedCount[x - 1][y] * 10;

    } else if (gyro.direction == NORTH && tof.wallExists[NORTH] == false) {
        weight = (abs(location.x) + abs(location.y + 1)) +
                 homingReachedCount[x][y + 1] * 10;

    } else if (gyro.direction == EAST && tof.wallExists[EAST] == false) {
        weight = (abs(location.x + 1) + abs(location.y)) +
                 homingReachedCount[x + 1][y] * 10;

    } else if (gyro.direction == SOUTH && tof.wallExists[SOUTH] == false) {
        weight = (abs(location.x) + abs(location.y - 1)) +
                 homingReachedCount[x][y - 1] * 10;
    }

    return weight;
}

int Homing::homingLeftWeight(void) {
    int x = location.x + FIELD_ORIGIN;
    int y = location.y + FIELD_ORIGIN;

    int weight = 0;

    if (gyro.direction == WEST && tof.wallExists[SOUTH] == false) {
        weight = (abs(location.x) + abs(location.y - 1)) +
                 homingReachedCount[x][y - 1] * 10;

    } else if (gyro.direction == NORTH && tof.wallExists[WEST] == false) {
        weight = (abs(location.x - 1) + abs(location.y)) +
                 homingReachedCount[x - 1][y] * 10;

    } else if (gyro.direction == EAST && tof.wallExists[NORTH] == false) {
        weight = (abs(location.x) + abs(location.y + 1)) +
                 homingReachedCount[x][y + 1] * 10;

    } else if (gyro.direction == SOUTH && tof.wallExists[EAST] == false) {
        weight = (abs(location.x + 1) + abs(location.y)) +
                 homingReachedCount[x + 1][y] * 10;
    }

    return weight;
}