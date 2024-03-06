#ifndef _UI_H_
#define _UI_H_

#include "../device/device.h"
#include "./location.h"

extern Location location;

class LCD {
   public:
    void begin();
    void writeRunningStatus();
};

#endif