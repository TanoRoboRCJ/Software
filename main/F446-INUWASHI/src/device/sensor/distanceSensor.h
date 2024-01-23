/** distanceSensor.h
 * VL53L0Xからの情報を監視します。
 * XIAOと繋がっているuartのポインタを渡して初期化すること。
 */

#ifndef _DISTANCE_SENSOR_H_
#define _DISTANCE_SENSOR_H_

#include <Arduino.h>

class DISTANCE_SENSOR {
   public:
    DISTANCE_SENSOR(HardwareSerial *p);

    HardwareSerial *serialPtr;

    int val[16] = {0};

    int vecX[16]    = {0};
    int vecY[16]    = {0};

    bool wallExist[4] = {false};

    int read(void);
    void calc(int angle);  // 定義しときました〜
    void direction(void);

   private:
    int _valTemp[16] = {0};
};

#endif