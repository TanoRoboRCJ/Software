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
    if (frontCRGB[0] < 70 && frontCRGB[1] < 35 && frontCRGB[2] < 35 &&
        frontCRGB[3] < 35) {
        frontColor = BLACK;
    } else if (frontCRGB[0] > 100 && frontCRGB[0] < 180 && frontCRGB[1] < 50 &&
               frontCRGB[2] < 60 && frontCRGB[3] < 70) {
        frontColor = BLUE;
    } else if (frontCRGB[0] > 150 && frontCRGB[0] < 250 && frontCRGB[1] < 100 &&
               frontCRGB[2] < 100 && frontCRGB[3] < 100) {
        frontColor = SILVER;
    } else {
        frontColor = WHITE;
    }
}

void FLOOR_SENSOR::backColorJudgment(void) {
    if (backCRGB[0] < 100 && backCRGB[1] < 35 && backCRGB[2] < 35 &&
        backCRGB[3] < 35) {
        backColor = BLACK;
    } else if (backCRGB[0] > 80 && backCRGB[0] < 120 && backCRGB[1] < 45 &&
               backCRGB[2] < 50 && backCRGB[3] < 70) {
        backColor = BLUE;
    } else if (backCRGB[0] > 200 && backCRGB[0] < 250 && backCRGB[1] < 120 &&
               backCRGB[2] < 120 && backCRGB[3] < 120) {
        backColor = SILVER;
    } else {
        backColor = WHITE;
    }
}