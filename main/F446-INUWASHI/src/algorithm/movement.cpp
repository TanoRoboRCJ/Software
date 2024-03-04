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

void Movement::move_1tile(void) {
    _oldCoordinateX = location.coordinateX;
    _oldCoordinateY = location.coordinateY;

    while (abs(location.coordinateX - _oldCoordinateX) < 300 &&
           abs(location.coordinateY - _oldCoordinateY) < 300) {
        if (tof.frontWallExists == true) {
            break;
        }
        servo.suspend  = false;
        servo.velocity = servo.DefaultSpeed;
        app.delay(Period);
    }  // 次のタイルまで前進
    servo.suspend  = true;
    servo.velocity = 0;
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
    if (tof.val[4] + tof.val[12] < 300) {
        if (tof.val[4] > tof.val[12]) {
            servo.isCorrectingAngle = 3;
        }
        if (tof.val[12] > tof.val[4]) {
            servo.isCorrectingAngle = -3;
        }
    } else {
        if (tof.val[12] < 120) {
            servo.isCorrectingAngle = 3;
        }
        if (tof.val[4] < 120) {
            servo.isCorrectingAngle = -3;
        }
    }

    if ((tof.rightWallExists == false) && (tof.leftWallExists == true)) {
        if (tof.val[12] > 120) {
            servo.isCorrectingAngle = -3;
        }
    }
    if ((tof.rightWallExists == true) && (tof.leftWallExists == false)) {
        if (tof.val[4] > 120) {
            servo.isCorrectingAngle = 3;
        }
    }
}

void Movement::avoidBarrier(void) {
    if (loadcell.status == RIGHT) {
        app.stop(servoApp);
        servo.driveAngularVelocity(-30, -45);
        app.delay(500);
        servo.driveAngularVelocity(-30, 45);
        app.delay(500);
        isHit = false;
    }
    if (loadcell.status == LEFT) {
        app.stop(servoApp);
        servo.driveAngularVelocity(-30, 45);
        app.delay(500);
        servo.driveAngularVelocity(-30, -45);
        app.delay(500);
        isHit = false;
    }
    if (!isHit) {
        servo.velocity = servo.DefaultSpeed;
        app.start(servoApp);
        isHit = true;
    }
}