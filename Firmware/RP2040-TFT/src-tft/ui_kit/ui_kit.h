#ifndef _UI_KIT_
#define _UI_KIT_

#include "./img/blend.h"

class UI_KIT {
   public:
    UI_KIT(void) {
    }

    int settingMode = 0;

    void init(void);
    void publish(void);

   private:
};

#endif