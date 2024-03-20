#include "homing.h"

Homing homing;

int Homing::dijkstra(int destX, int destY, int originX, int originY) {
    // RIGHT 0, FRONT 1, LEFT 2, BACK 3

    // 初期化
    for (int i = 0; i < FIELD_ORIGIN * 2; i++) {
        for (int j = 0; j < FIELD_ORIGIN * 2; j++) {
            homing.dijkstraSteps[i][j] = -1;
        }
    }

    homing.dijkstraSteps[originX + FIELD_ORIGIN][originY + FIELD_ORIGIN] = 0;

    // ダイクストラ法で歩数を計算

    int counter = 0;
    int oldCounter = 0;
    while (1) {
        for (int x = 2; x < FIELD_ORIGIN * 2 - 2; x++) {
            for (int y = 2; y < FIELD_ORIGIN * 2 - 2; y++) {
                // 到達ずみなら次へ
                if (homing.dijkstraSteps[x][y] != -1) {
                    continue;
                }

                // 周囲の歩数を取得
                int northStep = homing.dijkstraSteps[x][y + 1];
                int eastStep = homing.dijkstraSteps[x + 1][y];
                int southStep = homing.dijkstraSteps[x][y - 1];
                int westStep = homing.dijkstraSteps[x - 1][y];

                if (northStep != -1 && location.canGo(x, y, x, y + 1)) {
                    homing.dijkstraSteps[x][y] = northStep + 1;
                    counter++;
                } else if (eastStep != -1 && location.canGo(x, y, x + 1, y)) {
                    homing.dijkstraSteps[x][y] = eastStep + 1;
                    counter++;
                } else if (southStep != -1 && location.canGo(x, y, x, y - 1)) {
                    homing.dijkstraSteps[x][y] = southStep + 1;
                    counter++;
                } else if (westStep != -1 && location.canGo(x, y, x - 1, y)) {
                    homing.dijkstraSteps[x][y] = westStep + 1;
                    counter++;
                }
            }
        }

        if (homing.dijkstraSteps[destX + FIELD_ORIGIN][destY + FIELD_ORIGIN] !=
            -1) {
            break;
        }

        if (oldCounter == counter) {
            return 1000;
        }

        oldCounter = counter;
    }

    return homing.dijkstraSteps[destX + FIELD_ORIGIN][destY + FIELD_ORIGIN];
}

int Homing::homingWeighting(void) {
    int weight[4] = {0};

    weight[RIGHT] = homingRightWeight();
    weight[FRONT] = homingFrontWeight();
    weight[LEFT] = homingLeftWeight();
    weight[BACK] = homingBackWeight();

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
               weight[FRONT] <= weight[LEFT] && weight[FRONT] <= weight[BACK]) {
        return 1;  // front
    } else if (weight[LEFT] <= weight[RIGHT] && weight[LEFT] <= weight[FRONT] &&
               weight[LEFT] <= weight[BACK]) {
        return 2;  // left
    } else if (weight[BACK] <= weight[RIGHT] && weight[BACK] <= weight[FRONT] &&
               weight[BACK] <= weight[LEFT]) {
        return 3;  // back
    }
}

int Homing::dijkstraWeighting(void) {
    int weight[4] = {0};

    // FIXME いけないマスとかあると
    // 10000が帰ってきて、壁がうおーってなるから修正する

    switch (gyro.direction) {
        case NORTH:
            weight[RIGHT] = dijkstra(location.x + 1, location.y) +
                            homingReachedCount[location.x + FIELD_ORIGIN + 1]
                                              [location.y + FIELD_ORIGIN] *
                                100;
            weight[FRONT] = dijkstra(location.x, location.y + 1) +
                            homingReachedCount[location.x + FIELD_ORIGIN]
                                              [location.y + FIELD_ORIGIN + 1] *
                                100;
            weight[LEFT] = dijkstra(location.x - 1, location.y) +
                           homingReachedCount[location.x + FIELD_ORIGIN - 1]
                                             [location.y + FIELD_ORIGIN] *
                               100;
            weight[BACK] = dijkstra(location.x, location.y - 1) +
                           homingReachedCount[location.x + FIELD_ORIGIN]
                                             [location.y + FIELD_ORIGIN - 1] *
                               100;
            break;
        case EAST:
            weight[RIGHT] = dijkstra(location.x, location.y - 1) +
                            homingReachedCount[location.x + FIELD_ORIGIN]
                                              [location.y + FIELD_ORIGIN - 1] *
                                100;
            weight[FRONT] = dijkstra(location.x + 1, location.y) +
                            homingReachedCount[location.x + FIELD_ORIGIN + 1]
                                              [location.y + FIELD_ORIGIN] *
                                100;
            weight[LEFT] = dijkstra(location.x, location.y + 1) +
                           homingReachedCount[location.x + FIELD_ORIGIN]
                                             [location.y + FIELD_ORIGIN + 1] *
                               100;
            weight[BACK] = dijkstra(location.x - 1, location.y) +
                           homingReachedCount[location.x + FIELD_ORIGIN - 1]
                                             [location.y + FIELD_ORIGIN] *
                               100;
            break;
        case SOUTH:
            weight[RIGHT] = dijkstra(location.x - 1, location.y) +
                            homingReachedCount[location.x + FIELD_ORIGIN - 1]
                                              [location.y + FIELD_ORIGIN] *
                                100;
            weight[FRONT] = dijkstra(location.x, location.y - 1) +
                            homingReachedCount[location.x + FIELD_ORIGIN]
                                              [location.y + FIELD_ORIGIN - 1] *
                                100;
            weight[LEFT] = dijkstra(location.x + 1, location.y) +
                           homingReachedCount[location.x + FIELD_ORIGIN + 1]
                                             [location.y + FIELD_ORIGIN] *
                               100;
            weight[BACK] = dijkstra(location.x, location.y + 1) +
                           homingReachedCount[location.x + FIELD_ORIGIN]
                                             [location.y + FIELD_ORIGIN + 1] *
                               100;
            break;
        case WEST:
            weight[RIGHT] = dijkstra(location.x, location.y + 1) +
                            homingReachedCount[location.x + FIELD_ORIGIN]
                                              [location.y + FIELD_ORIGIN + 1] *
                                100;
            weight[FRONT] = dijkstra(location.x - 1, location.y) +
                            homingReachedCount[location.x + FIELD_ORIGIN - 1]
                                              [location.y + FIELD_ORIGIN] *
                                100;
            weight[LEFT] = dijkstra(location.x, location.y - 1) +
                           homingReachedCount[location.x + FIELD_ORIGIN]
                                             [location.y + FIELD_ORIGIN - 1] *
                               100;
            weight[BACK] = dijkstra(location.x + 1, location.y) +
                           homingReachedCount[location.x + FIELD_ORIGIN + 1]
                                             [location.y + FIELD_ORIGIN] *
                               100;
            break;
    }

    for (int i = 0; i < 4; i++) {
        weight[i] = constrain(weight[i], 0, DISABLE * 10 - 1);
    }

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
               weight[FRONT] <= weight[LEFT] && weight[FRONT] <= weight[BACK]) {
        return 1;  // front
    } else if (weight[LEFT] <= weight[RIGHT] && weight[LEFT] <= weight[FRONT] &&
               weight[LEFT] <= weight[BACK]) {
        return 2;  // left
    } else {
        if (millis() < homing.HomingTime + 5000) {
            return 3;  // back
        } else {
            return 0;  // front
        }
    }

    // FIXME: ここには来ないはず（最後のelse ifは安全のためelseにすべき）
}

int Homing::homingRightWeight(void) {
    int x = location.x + FIELD_ORIGIN;
    int y = location.y + FIELD_ORIGIN;

    int weight = 0;

    if (gyro.direction == WEST && tof.wallExists[NORTH] == false) {
        weight = (abs(location.x) + abs(location.y + 1)) * 5 +
                 homingReachedCount[x][y + 1] * 100;

    } else if (gyro.direction == NORTH && tof.wallExists[EAST] == false) {
        weight = (abs(location.x + 1) + abs(location.y)) * 5 +
                 homingReachedCount[x + 1][y] * 100;

    } else if (gyro.direction == EAST && tof.wallExists[SOUTH] == false) {
        weight = (abs(location.x) + abs(location.y - 1)) * 5 +
                 homingReachedCount[x][y - 1] * 100;

    } else if (gyro.direction == SOUTH && tof.wallExists[WEST] == false) {
        weight = (abs(location.x - 1) + abs(location.y)) * 5 +
                 homingReachedCount[x - 1][y] * 100;
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
                 homingReachedCount[x + 1][y] * 300;

    } else if (gyro.direction == NORTH && tof.wallExists[SOUTH] == false) {
        weight = (abs(location.x) + abs(location.y - 1)) * 5 +
                 homingReachedCount[x][y - 1] * 300;

    } else if (gyro.direction == EAST && tof.wallExists[WEST] == false) {
        weight = (abs(location.x - 1) + abs(location.y)) * 5 +
                 homingReachedCount[x - 1][y] * 300;

    } else if (gyro.direction == SOUTH && tof.wallExists[NORTH] == false) {
        weight = (abs(location.x) + abs(location.y + 1)) * 5 +
                 homingReachedCount[x][y + 1] * 300;
    }
    if (weight > 1000) {
        weight = 1000;
    }

    return weight;
}