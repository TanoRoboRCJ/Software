#ifndef _UI_H_
#define _UI_H_

#include "../device/device.h"
#include "./location.h"

extern Location location;

class LCD {
   public:
    // NOTE: この二つ、大鯰になったことで中身同じになった（後方互換性のために残してある）
    void begin();
    void writeRunningStatus();
};

#endif