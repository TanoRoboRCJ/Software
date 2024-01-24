#ifndef _RTOS_LCOATION_H_
#define _RTOS_LCOATION_H_

#include "../device/device.h"
#include "../kit/RTOS-Kit.h"
#include "../neko/neko.h"

void locationApp(App) {
    while (1) {
        for (int i = 0; i < 100; i++) {
            location.updateOdometory();
            app.delay(location.Period);
            location.updateMap();
        }

        location.updateObservationData();
    }
}

#endif