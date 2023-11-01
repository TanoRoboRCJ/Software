#include "lidar.h"

LIDAR::LIDAR(HardwareSerial *_uartPtr, int _rxPin, int _pwmPin) {
    static const int UartBaudrate = 230400;

    uartPtr = _uartPtr;
    uartPtr->begin(UartBaudrate, SERIAL_8N1, _rxPin);

    pinMode(_pwmPin, INPUT);  // 誤操作防止のためハイインピーダンスに

    return;
}

void LIDAR::read(void) {
    // データの読み取りのために開始文字を検索
    static const int UartDataLength = 47;
    static const int UartDataStartChar = 0x54;

    while (1) {
        if (uartPtr->available() < UartDataLength) {
            return;
        }

        if (uartPtr->read() == UartDataStartChar) {
            break;
        }
    }

    // 読み取り
    char rawData[UartDataLength - 1] = {0};
    uartPtr->readBytes(rawData, UartDataLength - 1);

    // NOTE:命名は公式ドキュメントに準拠
    // https://www.notion.so/shirokuma89dev/LiDAR-LD06-6998a6b96af247ac8309316f9cc44c03?pvs=4
    const int DataLength = 12;  // Fixed value
    const int RadarSpeed = (rawData[2] << 8) + rawData[1];
    const double StartAngle = ((rawData[4] << 8) + rawData[3]) / 100.0;
    const double EndAngle = ((rawData[42] << 8) + rawData[41]) / 100.0;
    const int TimeStamp = (rawData[44] << 8) + rawData[43];
    const int CrcCheck = rawData[45];

    double leadAngle = EndAngle - StartAngle;
    if (leadAngle < 0.0) {
        leadAngle += 360.0;
    }
    const double step = leadAngle / (DataLength - 1);

    static int circularBufferIndex = 0;
    for (int i = 0; i < DataLength; i++) {
        double angle = StartAngle + step * i;
        int distance = (rawData[6 + i * 3] << 8) + rawData[5 + i * 3];
        int confidence = rawData[7 + i * 3];

        if (distance < 1200 && distance >= 200) {
            // NOTE:動径は時計周り
            point[circularBufferIndex].x =
                (double)distance * sin(radians(angle));
            point[circularBufferIndex].y =
                (double)distance * cos(radians(angle));

            circularBufferIndex++;
            circularBufferIndex %= DataBuffLength;
        }
    }

    return;
}
