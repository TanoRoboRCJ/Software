#include "./exploring.h"

int reachedCount[MAP_ORIGIN * 2][MAP_ORIGIN * 2] = {0};

void updateMap(void) {
    static int i = 0;

    Map[i].x = location.x;
    Map[i].y = location.y;

    Map[i].color = floorSensor.Color;
    Map[i].victim = victim.isDetected;

    Map[i].wall[0] = tof.wallExists[NORTH];  // 北
    Map[i].wall[1] = tof.wallExists[EAST];   // 東
    Map[i].wall[2] = tof.wallExists[SOUTH];  // 南
    Map[i].wall[3] = tof.wallExists[WEST];   // 西

    i++;
}

int rightWeight(void) {
    int x = location.x + MAP_ORIGIN;
    int y = location.y + MAP_ORIGIN;

    int weight = 0;

    if (gyro.direction == WEST && tof.wallExists[NORTH] == false) {
        weight = reachedCount[x][y + 1];

    } else if (gyro.direction == NORTH && tof.wallExists[EAST] == false) {
        weight = reachedCount[x + 1][y];

    } else if (gyro.direction == EAST && tof.wallExists[SOUTH] == false) {
        weight = reachedCount[x][y - 1];

    } else if (gyro.direction == SOUTH && tof.wallExists[WEST] == false) {
        weight = reachedCount[x - 1][y];
    }

    return weight * PASSED_WEIGHT;
}

int frontWeight(void) {
    int x = location.x + MAP_ORIGIN;
    int y = location.y + MAP_ORIGIN;

    int weight = 0;

    if (gyro.direction == WEST && tof.wallExists[WEST] == false) {
        weight = reachedCount[x - 1][y];

    } else if (gyro.direction == NORTH && tof.wallExists[NORTH] == false) {
        weight = reachedCount[x][y + 1];

    } else if (gyro.direction == EAST && !tof.wallExists[EAST] == false) {
        weight = reachedCount[x + 1][y];

    } else if (gyro.direction == SOUTH && !tof.wallExists[SOUTH] == false) {
        weight = reachedCount[x][y - 1];
    }

    return weight * PASSED_WEIGHT;
}

int leftWeigt(void) {
    int weight = 0;

    int x = location.x + MAP_ORIGIN;
    int y = location.y + MAP_ORIGIN;

    if (gyro.direction == WEST && tof.wallExists[SOUTH] == false) {
        weight = reachedCount[x][y - 1];

    } else if (gyro.direction == NORTH && tof.wallExists[WEST] == false) {
        weight = reachedCount[x - 1][y];

    } else if (gyro.direction == EAST && tof.wallExists[NORTH] == false) {
        weight = reachedCount[x][y + 1];

    } else if (gyro.direction == SOUTH && tof.wallExists[EAST] == false) {
        weight = reachedCount[x + 1][y];
    }

    return weight * PASSED_WEIGHT;
}

int weighting(void) {
    int weight[3] = {1, 2, 3};  // {right, front, left}

    weight[RIGHT] += rightWeight();
    weight[FRONT] += frontWeight();
    weight[LEFT] += leftWeigt();

    if (tof.val[4] < 170) {
        weight[RIGHT] = DISABLE;
    }
    if (tof.val[0] < 170) {
        weight[FRONT] = DISABLE;
    }
    if (tof.val[12] < 170) {
        weight[LEFT] = DISABLE;
    }
    if (weight[RIGHT] <= weight[FRONT] && weight[RIGHT] <= weight[LEFT]) {
        return 0;  // right
    } else if (weight[FRONT] <= weight[RIGHT] &&
               weight[FRONT] <= weight[LEFT]) {
        return 1;  // front
    } else if (weight[LEFT] <= weight[RIGHT] && weight[LEFT] <= weight[FRONT]) {
        return 2;  // left
    }

    // FIXME: ここには来ないはず
}
