#include "./movement.h"

Movement movement;

// FIXME: 絶対方位指定じゃないとバグります！//NOTE:修正済み
void Movement::turnRight(void) {
    servo.suspend = true;
    app.delay(_Wait);
    servo.suspend = false;
    switch (gyro.direction) {
        case NORTH:
            servo.angle = 90;
            break;
        case EAST:
            servo.angle = 180;
            break;
        case SOUTH:
            servo.angle = 270;
            break;
        case WEST:
            servo.angle = 0;
            break;
    }
    servo.isCorrectingAngle = 0;
    app.delay(_Wait * 2);

    if (isStucked(gyro.direction) == true) {
        // uart1.println("stucked");
        goOverBarrier();
    }
}

// FIXME: 絶対方位指定じゃないとバグります！//NOTE:修正済み
void Movement::turnLeft(void) {
    servo.suspend = true;
    app.delay(_Wait);
    servo.suspend = false;
    switch (gyro.direction) {
        case NORTH:
            servo.angle = 270;
            break;
        case EAST:
            servo.angle = 0;
            break;
        case SOUTH:
            servo.angle = 90;
            break;
        case WEST:
            servo.angle = 180;
            break;
    }
    servo.isCorrectingAngle = 0;
    app.delay(_Wait * 2);

    if (isStucked(gyro.direction) == true) {
        // uart1.println("stucked");
        goOverBarrier();
    }
}

// FIXME: 絶対方位指定じゃないとバグります！//NOTE:修正済み
void Movement::turnReverse(void) {
    servo.suspend = true;
    app.delay(_Wait);
    servo.suspend = false;
    switch (gyro.direction) {
        case NORTH:
            servo.angle = 180;
            break;
        case EAST:
            servo.angle = 270;
            break;
        case SOUTH:
            servo.angle = 0;
            break;
        case WEST:
            servo.angle = 90;
            break;
    }
    servo.isCorrectingAngle = 0;
    app.delay(_Wait * 3);

    if (isStucked(gyro.direction) == true) {
        goOverBarrier();
    }
}

void Movement::move_1tile(void) {  // 絶妙な位置なら詰める
    _oldCoordinateX = location.coordinateX;
    _oldCoordinateY = location.coordinateY;
    exception       = true;
    CanGoRight      = false;
    CanGoLeft       = false;

    while (abs(location.coordinateX - _oldCoordinateX) < 300 &&
           abs(location.coordinateY - _oldCoordinateY) < 300) {
        if (tof.frontWallExists == true &&
            isHit ==
                false) {  // FIXME:
                          // isHit無視してbreakする可能性がある//NOTE:条件式を追加
            if (tof.rightWallExists == false) {
                CanGoRight = true;
            }
            if (tof.leftWallExists == false) {
                CanGoLeft = true;
            }
            break;
        }
        if (abs(location.coordinateX - _oldCoordinateX) > 280 ||
            abs(location.coordinateY - _oldCoordinateY) > 280) {
            if (tof.rightWallExists == false) {
                CanGoRight = true;
            }
            if (tof.leftWallExists == false) {
                CanGoLeft = true;
            }
        }
        servo.suspend  = false;
        servo.velocity = servo.DefaultSpeed;
        exception      = false;

        app.delay(Period);
    }  // 次のタイルまで前進
    while (140 < tof.val[0] && tof.val[0] < 250) {
        app.stop(locationApp);
        servo.suspend  = false;
        servo.velocity = servo.DefaultSpeed;
        app.delay(Period);
    }
    app.start(locationApp);
    if (exception == true && tof.rightWallExists == true &&
        tof.frontWallExists == true && tof.leftWallExists == true) {
        turnReverse();
    }
}

void Movement::back(void) {
    _oldCoordinateX = location.coordinateX;
    _oldCoordinateY = location.coordinateY;

    while (abs(location.coordinateX - _oldCoordinateX) < 100 &&
           abs(location.coordinateY - _oldCoordinateY) < 100) {
        servo.suspend  = false;
        servo.velocity = -servo.DefaultSpeed;
        app.delay(Period);
    }  // NOTE 黒タイルから後退
}

void Movement::turnNorth(void) {
    servo.suspend = true;
    app.delay(_Wait);
    servo.suspend           = false;
    servo.angle             = 0;
    servo.isCorrectingAngle = 0;
    app.delay(_Wait * 3);
}

void Movement::turnEast(void) {
    servo.suspend = true;
    app.delay(_Wait);
    servo.suspend           = false;
    servo.angle             = 90;
    servo.isCorrectingAngle = 0;
    app.delay(_Wait * 3);
}

void Movement::turnSouth(void) {
    servo.suspend = true;
    app.delay(_Wait);
    servo.suspend           = false;
    servo.angle             = 180;
    servo.isCorrectingAngle = 0;
    app.delay(_Wait * 3);
}

void Movement::turnWest(void) {
    servo.suspend = true;
    app.delay(_Wait);
    servo.suspend           = false;
    servo.angle             = 270;
    servo.isCorrectingAngle = 0;
    app.delay(_Wait * 3);
}

void Movement::angleAdjustment(void) {
    if (tof.rightWallExists == true && tof.leftWallExists == true) {
        servo.isCorrectingAngle =
            map(tof.val[4] - tof.val[12], -100, 100, -20, 20);
    }

    if (tof.rightWallExists == true && tof.leftWallExists == false) {
        servo.isCorrectingAngle = map(tof.val[4] - 130, -85, 85, -20, 20);
    }

    if (tof.rightWallExists == false && tof.leftWallExists == true) {
        servo.isCorrectingAngle = map(130 - tof.val[12], -85, 85, -20, 20);
    }

    if ((tof.rightWallExists == false) && (tof.leftWallExists == false)) {
        if (gyro.direction == NORTH || gyro.direction == SOUTH) {
            servo.isCorrectingAngle = map(
                location.coordinateX - 300 * location.x, -150, 150, -10, 10);
        }
        if (gyro.direction == EAST || gyro.direction == WEST) {
            servo.isCorrectingAngle = map(
                location.coordinateY - 300 * location.y, -150, 150, -10, 10);
        }
    }
}

void Movement::avoidBarrier(void) {
    // FIXME:
    // isHitをonにするのは当たった瞬間な気がします、あと39行目のtof.frontWallExistsのところにisHitの条件が入っていないので、回避動作中に壁が視野に入ったら多分breakしてしまう
    if (loadcell.status == RIGHT) {
        // FIXME: 多分ここでisHitを入れるべき//NOTE:修正済み
        isHit = true;
        app.stop(servoApp);
        if (homing.started == true) {
            app.stop(homingApp);
        } else {
            app.stop(rightWallApp);
        }
        servo.driveAngularVelocity(-90, -90);
        app.delay(300);
        servo.driveAngularVelocity(-90, 90);
        app.delay(300);
    }
    if (loadcell.status == LEFT) {
        // FIXME: 多分ここでisHitを入れるべき//NOTE:修正済み
        isHit = true;
        app.stop(servoApp);
        if (homing.started == true) {
            app.stop(homingApp);
        } else {
            app.stop(rightWallApp);
        }
        servo.driveAngularVelocity(-90, 90);
        app.delay(300);
        servo.driveAngularVelocity(-90, -90);
        app.delay(300);
    }
    if (isHit) {
        servo.velocity = servo.DefaultSpeed;
        app.start(servoApp);

        // FIXME: これ、restartではなくてstartじゃない？//NOTE:修正済み
        if (homing.started == true) {
            app.start(homingApp);
        } else {
            app.start(rightWallApp);
        }
        isHit = false;
    }
}

bool Movement::isStucked(int direction) {
    switch (direction) {
        case NORTH:
            if (servo.angle != 0) {
                return true;
            } else {
                return false;
            }
        case EAST:
            if (servo.angle != 90) {
                return true;
            } else {
                return false;
            }
        case SOUTH:
            if (servo.angle != 180) {
                return true;
            } else {
                return false;
            }
        case WEST:
            if (servo.angle != 270) {
                return true;
            } else {
                return false;
            }
        default:
            return false;
    }
}

void Movement::goOverBarrier(void) {
    servo.suspend  = true;
    servo.velocity = 0;
    app.delay(_Wait);
    switch (gyro.direction) {
        case NORTH:
            servo.angle = 0;
            break;
        case EAST:
            servo.angle = 90;
            break;
        case SOUTH:
            servo.angle = 180;
            break;
        case WEST:
            servo.angle = 270;
            break;
    }
    // FIXME:スタックしてるのにlocationの情報使うのは良くない
    _oldCoordinateX = location.coordinateX;
    _oldCoordinateY = location.coordinateY;

    while (abs(location.coordinateX - _oldCoordinateX) < 50 &&
           abs(location.coordinateY - _oldCoordinateY) < 50) {
        servo.suspend  = false;
        servo.velocity = servo.DefaultSpeed;
        app.delay(Period);
    }
}