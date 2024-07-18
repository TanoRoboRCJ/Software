#include "floorSensor.h"

void FLOOR_SENSOR::read(void) {
    frontCRGB[0] = bottom.tcsClear[0];
    frontCRGB[1] = bottom.tcsRed[0];
    frontCRGB[2] = bottom.tcsGreen[0];
    frontCRGB[3] = bottom.tcsBlue[0];
    backCRGB[0]  = bottom.tcsClear[1];
    backCRGB[1]  = bottom.tcsRed[1];
    backCRGB[2]  = bottom.tcsGreen[1];
    backCRGB[3]  = bottom.tcsBlue[1];

    frontColorJudgment();
    backColorJudgment();
}

void FLOOR_SENSOR::frontColorJudgment(void) {
    if (frontCRGB[0] < 130 && frontCRGB[1] < 40 && frontCRGB[2] < 45 &&
        frontCRGB[3] < 45) {
        frontColor = BLACK;
    } else if (frontCRGB[0] > 130 && frontCRGB[0] < 210 && frontCRGB[1] < 60 &&
               frontCRGB[2] < 80 && frontCRGB[3] < 90) {
        frontColor = BLUE;
    } else if (frontCRGB[0] > 270 && frontCRGB[0] < 360 && frontCRGB[1] < 120 &&
               frontCRGB[2] < 130 && frontCRGB[3] < 110) {
        frontColor = SILVER;
    } else {
        frontColor = WHITE;
    }
}

void FLOOR_SENSOR::backColorJudgment(void) {
    if (backCRGB[0] < 100 && backCRGB[1] < 35 && backCRGB[2] < 45 &&
        backCRGB[3] < 45) {
        backColor = BLACK;
    } else if (backCRGB[0] > 100 && backCRGB[0] < 200 && backCRGB[1] < 50 &&
               backCRGB[2] < 80 && backCRGB[3] < 85) {
        backColor = BLUE;
    } else if (backCRGB[0] > 300 && backCRGB[0] < 390 && backCRGB[1] < 130 &&
               backCRGB[2] < 155 && backCRGB[3] < 130){
        backColor = SILVER;
    } else {
        backColor = WHITE;
    }
}