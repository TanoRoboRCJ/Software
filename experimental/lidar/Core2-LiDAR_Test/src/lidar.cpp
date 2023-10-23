#include "lidar.h"

LIDAR::LIDAR(HardwareSerial *_uartPtr, int _rxPin, int _pwmPin) {
    uartPtr = _uartPtr;
    uartPtr->begin(LIDAR_BAUDRATE, SERIAL_8N1, _rxPin);

    // NOTE: 128という数字に特に意味はない、PWMならどれでもいい
    // pinMode(_pwmPin, OUTPUT);
    // analogWrite(_pwmPin, 128);

    return;
}

void LIDAR::read(void) {
    while (1) {
        if (uartPtr->available() < LIDAR_DATA_LENGTH) {
            return;
        }

        if (uartPtr->read() == LIDAR_DATA_START_CHAR) {
            break;
        }
    }

    char rawData[LIDAR_DATA_LENGTH - 1] = {0};
    uartPtr->readBytes(rawData, LIDAR_DATA_LENGTH - 1);

    // NOTE:命名は公式ドキュメントに準拠
    // https://www.notion.so/shirokuma89dev/LiDAR-LD06-6998a6b96af247ac8309316f9cc44c03?pvs=4
    const int dataLength = 12;  // Fixed value
    const int radarSpeed = (rawData[2] << 8) + rawData[1];
    const double startAngle = ((rawData[4] << 8) + rawData[3]) / 100.0;
    const double endAngle = ((rawData[42] << 8) + rawData[41]) / 100.0;
    const int timeStamp = (rawData[44] << 8) + rawData[43];
    const int crcCheck = rawData[45];

    double leadAngle = endAngle - startAngle;
    if (leadAngle < 0.0) {
        leadAngle += 360.0;
    }
    const double step = leadAngle / (dataLength - 1);

    static int circularBufferIndex = 0;
    for (int i = 0; i < dataLength; i++) { 
        double angle = startAngle + step * i;
        int distance = (rawData[6 + i * 3] << 8) + rawData[5 + i * 3];
        int confidence = rawData[7 + i * 3];

        if (distance < 1200 && distance >= 200) {
            // NOTE:動径は時計周り
            point[circularBufferIndex].x = (double)distance * sin(radians(angle));
            point[circularBufferIndex].y = (double)distance * cos(radians(angle));

            circularBufferIndex++;
            circularBufferIndex %= LIDAR_POINT_BUFFER_SIZE;
        }
    }

    return;
}
