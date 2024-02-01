#include "homing.h"

Homing homing;

int Homing::homingWeighting(void) {
    int weight[3] = {1, 2, 3};  // {right, front, left}

    weight[RIGHT] += homingRightWeight();
    weight[FRONT] += homingFrontWeight();
    weight[LEFT] += homingLeftWeight();

    if (tof.val[4] < 170) {
        weight[RIGHT] += DISABLE * 5;
    }
    if (tof.val[0] < 170) {
        weight[FRONT] += DISABLE * 5;
    }
    if (tof.val[12] < 170) {
        weight[LEFT] += DISABLE * 5;
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

    int weight = (abs(location.x) + abs(location.y)) * 10;

    if (gyro.direction == WEST && tof.wallExists[NORTH] == false) {
        weight += homingReachedCount[x][y + 1] * 100;

    } else if (gyro.direction == NORTH && tof.wallExists[EAST] == false) {
        weight += homingReachedCount[x + 1][y] * 100;

    } else if (gyro.direction == EAST && tof.wallExists[SOUTH] == false) {
        weight += homingReachedCount[x][y - 1] * 100;

    } else if (gyro.direction == SOUTH && tof.wallExists[WEST] == false) {
        weight += homingReachedCount[x - 1][y] * 100;
    }

    return weight;
}

int Homing::homingFrontWeight(void) {
    int x = location.x + FIELD_ORIGIN;
    int y = location.y + FIELD_ORIGIN;

    int weight = (abs(location.x) + abs(location.y)) * 10;

    if (gyro.direction == WEST && tof.wallExists[WEST] == false) {
        weight += homingReachedCount[x - 1][y] * 100;

    } else if (gyro.direction == NORTH && tof.wallExists[NORTH] == false) {
        weight += homingReachedCount[x][y + 1] * 100;

    } else if (gyro.direction == EAST && !tof.wallExists[EAST] == false) {
        weight += homingReachedCount[x + 1][y] * 100;

    } else if (gyro.direction == SOUTH && !tof.wallExists[SOUTH] == false) {
        weight += homingReachedCount[x][y - 1] * 100;
    }

    return weight;
}

int Homing::homingLeftWeight(void) {
    int x = location.x + FIELD_ORIGIN;
    int y = location.y + FIELD_ORIGIN;

    int weight = (abs(location.x) + abs(location.y)) * 10;

    if (gyro.direction == WEST && tof.wallExists[SOUTH] == false) {
        weight += homingReachedCount[x][y - 1] * 100;

    } else if (gyro.direction == NORTH && tof.wallExists[WEST] == false) {
        weight += homingReachedCount[x - 1][y] * 100;

    } else if (gyro.direction == EAST && tof.wallExists[NORTH] == false) {
        weight += homingReachedCount[x][y + 1] * 100;

    } else if (gyro.direction == SOUTH && tof.wallExists[EAST] == false) {
        weight += homingReachedCount[x + 1][y] * 100;
    }

    return weight;
}