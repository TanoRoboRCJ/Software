#ifndef _LOCATION_H_
#define _LOCATION_H_

#include "../device/device.h"

#define FIELD_ORIGIN 20

// FIXME: いずれはFieldとRouteを統合したい
class Field {
   public:
    bool isPassed = false;
    bool isDetected = false;

    bool northWall = false;  // 起動時の方角を北とする。
    bool southWall = false;
    bool eastWall = false;
    bool westWall = false;

    int floorColor = 0;  // 床の色

    bool isVictimDetected = false;

    unsigned long firstPassedTime = 0;
};

class Route {
   public:
    static const int White = 0;
    static const int Black = 1;
    static const int Blue = 2;
    static const int Silver = 3;

    static const int unreached = 1000;

    int x = unreached;
    int y = unreached;

    int color = White;                            // 床色
    bool victim = false;                          // 被災者の有無
    bool wall[4] = {false, false, false, false};  // 北東南西の順, 絶対方位

    int isReached() {
        if (x == unreached) {
            return 0;
        } else {
            return 1;
        }
    };
};

class Location {
   private:
    // CONSTANT
    double _VelocityConstant = 0.00168;//0.00159

   public:
    // CONSTANT
    static const int Period = 10;

    // FUNCTION
    Location();
    void updateOdometory(void);
    void updateObservationData(void);
    void updateMap(void);

    // VARIABLE
    Field field[FIELD_ORIGIN * 2][FIELD_ORIGIN * 2];
    Route route[200];

    int x = 0;
    int y = 0;

    double coordinateX = 0;  // 絶対座標 [mm]
    double coordinateY = 0;  // 絶対座標 [mm]

    int IndexOfSensorFacingNorth = 0;

    unsigned long lastTrustX = 0;
    unsigned long lastTrustY = 0;

    unsigned long lastCorrection = 0;
};

#endif