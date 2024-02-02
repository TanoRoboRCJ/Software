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
        servo.suspend = false;
        servo.velocity = servo.DefaultSpeed;
        app.delay(Period);
    }  // 次のタイルまで前進
    servo.suspend = true;
    servo.velocity = 0;
}

void Movement::turnNorth(void){
     servo.suspend = true;
    app.delay(_Wait);
    servo.suspend = false;
    servo.angle = 0;
    servo.isCorrectingAngle = 0;
    app.delay(_Wait * 3);
}

void Movement::turnEast(void){
     servo.suspend = true;
    app.delay(_Wait);
    servo.suspend = false;
    servo.angle = 90;
    servo.isCorrectingAngle = 0;
    app.delay(_Wait * 3);
}

void Movement::turnSouth(void){
     servo.suspend = true;
    app.delay(_Wait);
    servo.suspend = false;
    servo.angle = 180;
    servo.isCorrectingAngle = 0;
    app.delay(_Wait * 3);
}

void Movement::turnWest(void){
     servo.suspend = true;
    app.delay(_Wait);
    servo.suspend = false;
    servo.angle = 270;
    servo.isCorrectingAngle = 0;
    app.delay(_Wait * 3);
}
