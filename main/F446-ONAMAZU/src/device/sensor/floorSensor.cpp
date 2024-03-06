#include "floorSensor.h"

void FLOOR_SENSOR::read(void) {
    frontCRGB[0] = bottom.tcsClear[0];
    frontCRGB[1] = bottom.tcsRed[0];
    frontCRGB[2] = bottom.tcsGreen[0];
    frontCRGB[3] = bottom.tcsBlue[0];
    backCRGB[0] = bottom.tcsClear[1];
    backCRGB[1] = bottom.tcsRed[1];
    backCRGB[2] = bottom.tcsGreen[1];
    backCRGB[3] = bottom.tcsBlue[1];
    
    // colorJudgment();`
}

void FLOOR_SENSOR::colorJudgment(void) {
    // if (redVal > 910 && blankVal > 910 && blueVal > 910) {
    //     Color = 1;
    //     // 黒
    // }  else {
    //     Color = 0;
    //     // 白
    // }//FIXME ここに銀色の判定を入れる
    // // else if ( (blankVal > 100 && blankVal < 650) && blueVal < 300) {
    // //     color = 2;
    // //     // 青
    // // } 
   
}