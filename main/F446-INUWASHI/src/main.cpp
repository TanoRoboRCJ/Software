/** main.cpp
 * これは聖なるメインプログラム
 * あっぱれ！！あっぱれ！！
 */

#include <Arduino.h>

#include "./kit/RTOS-Kit.h"
#include "./RTOS/RTOS.h"
#include "./device/device.h"

void setup() {
    initDevice();

    // メイン管制
    app.create(mainApp, firstPriority);

    // デーモン
    app.create(monitorApp);
    app.create(locationApp, firstPriority);
    app.create(victimNotifyApp);
    app.create(ledApp);
    app.create(sensorApp, secondPriority);
    app.create(servoApp, secondPriority);

    // 動作系統
    app.create(rightWallApp);
    app.create(adjustmentApp);

    app.start(mainApp);
    app.startRTOS();
}

void loop() {
    /** ここには何も書かない
     * RTOS管理下にあるので書いても意味ないです
     */
}