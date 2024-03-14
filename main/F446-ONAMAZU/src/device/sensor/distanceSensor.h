/** distanceSensor.h
 * VL53L0Xからの情報を監視します。
 * XIAOと繋がっているuartのポインタを渡して初期化すること。
 */

#ifndef _DISTANCE_SENSOR_H_
#define _DISTANCE_SENSOR_H_

#include <Arduino.h>

#include "../bottom.h"

extern Bottom bottom;

class DISTANCE_SENSOR {
   public:
    DISTANCE_SENSOR(HardwareSerial *p);

    int read(void);
    void calc(int angle);
    void direction(void);
    void wallJudgment(void);

    HardwareSerial *serialPtr;

    int val[16]  = {0};
    int vecX[16] = {0};
    int vecY[16] = {0};

    bool wallExists[4]    = {false};
    bool rightWallExists  = false;
    bool frontWallExists  = false;
    bool behindWallExists = false;
    bool leftWallExists   = false;

    double covX = 0;
    double covY = 0;

    bool canCorrect = false;

   private:
    int _valTemp[16] = {0};
};

#endif