#include "./exploring.h"

Exploring exploring;

void Exploring::updateMap(void) {
    static int i = 0;

    location.route[i].x = location.x;
    location.route[i].y = location.y;

    // location.route[i].color  = floorSensor.Color;
    location.route[i].victim = victim.isDetected;

    location.route[i].wall[0] = tof.wallExists[NORTH];  // 北
    location.route[i].wall[1] = tof.wallExists[EAST];   // 東
    location.route[i].wall[2] = tof.wallExists[SOUTH];  // 南
    location.route[i].wall[3] = tof.wallExists[WEST];   // 西

    i++;
}

int Exploring::weighting(void) {
    int weight[3] = {1, 2, 3};  // {right, front, left}

    weight[RIGHT] += rightWeight();
    weight[FRONT] += frontWeight();
    weight[LEFT] += leftWeight();

    if (tof.rightWallExists == true) {
        weight[RIGHT] = DISABLE * 10;
    }
    if (tof.frontWallExists == true) {
        weight[FRONT] = DISABLE * 10;
    }
    if (tof.leftWallExists == true) {
        weight[LEFT] = DISABLE * 10;
    }

    // uart1.print(weight[RIGHT]);
    // uart1.print("\t");
    // uart1.print(weight[FRONT]);
    // uart1.print("\t");
    // uart1.print(weight[LEFT]);
    // uart1.println();

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

int Exploring::rightWeight(void) {
    int x = location.x + FIELD_ORIGIN;
    int y = location.y + FIELD_ORIGIN;

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

int Exploring::frontWeight(void) {
    int x = location.x + FIELD_ORIGIN;
    int y = location.y + FIELD_ORIGIN;

    int weight = 0;

    if (gyro.direction == WEST && tof.wallExists[WEST] == false) {
        weight = reachedCount[x - 1][y];

    } else if (gyro.direction == NORTH && tof.wallExists[NORTH] == false) {
        weight = reachedCount[x][y + 1];

    } else if (gyro.direction == EAST && tof.wallExists[EAST] == false) {
        weight = reachedCount[x + 1][y];

    } else if (gyro.direction == SOUTH && tof.wallExists[SOUTH] == false) {
        weight = reachedCount[x][y - 1];
    }

    return weight * PASSED_WEIGHT;
}

int Exploring::leftWeight(void) {
    int x = location.x + FIELD_ORIGIN;
    int y = location.y + FIELD_ORIGIN;

    int weight = 0;

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