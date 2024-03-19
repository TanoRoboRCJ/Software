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
    int weight[3] = {0};

    for (int i = 0; i < FIELD_ORIGIN * 2; i++) {
        for (int j = 0; j < FIELD_ORIGIN * 2; j++) {
            reachedCount[i][j] %= 21;
        }
    }

    weight[RIGHT] = rightWeight();
    weight[FRONT] = frontWeight();
    weight[LEFT]  = leftWeight();

    if (tof.rightWallExists == false) {
        movement.CanGoRight = true;
    }
    if (tof.leftWallExists == false) {
        movement.CanGoLeft = true;
    }

    if (movement.CanGoRight == false) {
        weight[RIGHT] = DISABLE;
    }
    if (tof.frontWallExists == true) {
        weight[FRONT] = DISABLE;
    }
    if (movement.CanGoLeft == false) {
        weight[LEFT] = DISABLE;
    }

    // uart1.print(weight[RIGHT]);
    // uart1.print("\t");
    // uart1.print(weight[FRONT]);
    // uart1.print("\t");
    // uart1.print(weight[LEFT]);
    // uart1.print("\t");
    // uart1.print(gyro.deg);
    // uart1.println();

    if (weight[RIGHT] <= weight[FRONT] &&
        weight[RIGHT] <= weight[LEFT]) {  // NOTE 同列の場合は右優先
        return 0;                         // right
    } else if (weight[FRONT] <= weight[RIGHT] &&
               weight[FRONT] <= weight[LEFT]) {
        return 1;  // front
    } else {
        return 2;  // left
    }

    // FIXME: ここには来ないはず（最後のelse ifは安全のためelseにすべき）
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
    if (weight > 1000) {
        weight = 1000;
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
    if (weight > 1000) {
        weight = 1000;
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
    if (weight > 1000) {
        weight = 1000;
    }

    return weight * PASSED_WEIGHT;
}