#ifndef _LOCATION_H_
#define _LOCATION_H_

#include "../device/device.h"

#define MAP_ORIGIN 20

class MAP_Kit {
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

class Location {
   private:
    // CONSTANT
    double _VelocityConstant = 0.00159;

   public:
    // CONSTANT
    static const int Period = 10;

    // FUNCTION
    Location();
    void updateOdometory(void);
    void updateObservationData(void);
    void updateMap(void);

    // VARIABLE
    MAP_Kit mapData[MAP_ORIGIN * 2][MAP_ORIGIN * 2];

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