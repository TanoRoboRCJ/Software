#include "./movement.h"

Movement movement;

void Movement::turnRight(void) {
    servo.suspend = true;
    app.delay(_Wait);
    servo.suspend = false;
    servo.angle += 90;
    servo.isCorrectingAngle = 0;
    app.delay(_Wait * 2);
}

void Movement::turnLeft(void) {
    servo.suspend = true;
    app.delay(_Wait);
    servo.suspend = false;
    servo.angle -= 90;
    servo.isCorrectingAngle = 0;
    app.delay(_Wait * 2);
}

void Movement::turnReverse(void) {
    servo.suspend = true;
    app.delay(_Wait);
    servo.suspend = false;
    servo.angle += 180;
    servo.isCorrectingAngle = 0;
    app.delay(_Wait * 3);
}

void Movement::move_1tile(void) {  // 絶妙な位置なら詰める
    _oldCoordinateX = location.coordinateX;
    _oldCoordinateY = location.coordinateY;

    while (abs(location.coordinateX - _oldCoordinateX) < 300 &&
           abs(location.coordinateY - _oldCoordinateY) <
               300) {  // FIXME 誤差が蓄積される
        if (tof.frontWallExists == true) {
            break;
        }
        servo.suspend  = false;
        servo.velocity = servo.DefaultSpeed;
        app.delay(Period);
    }  // 次のタイルまで前進
    while (140 < tof.val[0] && tof.val[0] < 270) {
        servo.suspend  = false;
        servo.velocity = servo.DefaultSpeed;
        app.delay(Period);
    }
    servo.suspend  = true;
    servo.velocity = 0;
}

void Movement::back(void) {
    _oldCoordinateX = location.coordinateX;
    _oldCoordinateY = location.coordinateY;

    while (abs(location.coordinateX - _oldCoordinateX) < 100 &&
           abs(location.coordinateY - _oldCoordinateY) < 100) {
        servo.suspend  = false;
        servo.velocity = -servo.DefaultSpeed;
        app.delay(Period);
    }  // 黒タイルから後退
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

void Movement::angleAdjustment(void) {  // NOTE y = ax + b
    if (tof.rightWallExists == true && tof.leftWallExists == true) {
        servo.isCorrectingAngle =
            map(tof.val[4] - tof.val[12], -100, 100, -20, 20);
    }

    if (tof.rightWallExists == true && tof.leftWallExists == false) {
        servo.isCorrectingAngle = map(tof.val[4] - 130, -100, 100, -20, 20);
    }

    if (tof.rightWallExists == false && tof.leftWallExists == true) {
        servo.isCorrectingAngle = map(130 - tof.val[12], -100, 100, -20, 20);
    }

    if ((tof.rightWallExists == false) &&
        (tof.leftWallExists == false)) {  // FIXME 上手く補正出来てない
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
    if (loadcell.status == RIGHT) {
        app.stop(servoApp);
        if (homing.started == true) {
            app.stop(homingApp);
        } else {
            app.stop(rightWallApp);
        }
        servo.driveAngularVelocity(-30, -45);
        app.delay(500);
        servo.driveAngularVelocity(-30, 45);
        app.delay(500);
        isHit = false;
    }
    if (loadcell.status == LEFT) {
        app.stop(servoApp);
        if (homing.started == true) {
            app.stop(homingApp);
        } else {
            app.stop(rightWallApp);
        }
        servo.driveAngularVelocity(-30, 45);
        app.delay(500);
        servo.driveAngularVelocity(-30, -45);
        app.delay(500);
        isHit = false;
    }
    if (!isHit) {
        servo.velocity = servo.DefaultSpeed;
        app.start(servoApp);
        if (homing.started == true) {
            app.restart(homingApp);
        } else {
            app.restart(rightWallApp);
        }
        isHit = true;
    }
}