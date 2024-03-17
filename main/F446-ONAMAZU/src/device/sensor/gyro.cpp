#include "gyro.h"

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

extern HardwareSerial uart1;

#ifdef BNO055_MODE

GYRO::GYRO(Adafruit_BNO055 *p) {
    sensorPtr = p;
}

void GYRO::init(void) {
    sensorPtr->begin(OPERATION_MODE_IMUPLUS);

    int eeAddress = 0;
    long bnoID;
    bool foundCalib = false;

    EEPROM.get(eeAddress, bnoID);

    adafruit_bno055_offsets_t calibrationData;
    sensor_t sensor;

    sensorPtr->getSensor(&sensor);
    if (bnoID != sensor.sensor_id) {
        uart1.println("\nNo Calibration Data for this sensor exists in EEPROM");
        delay(500);
    } else {
        uart1.println("\nFound Calibration for this sensor in EEPROM.");
        eeAddress += sizeof(long);
        EEPROM.get(eeAddress, calibrationData);

        displaySensorOffsets(calibrationData);

        uart1.println("\n\nRestoring Calibration data to the BNO055...");
        sensorPtr->setSensorOffsets(calibrationData);

        uart1.println("\n\nCalibration data loaded into BNO055");
        foundCalib = true;
    }

    uint8_t system_status, self_test_results, system_error;
    system_status = self_test_results = system_error = 0;
    sensorPtr->getSystemStatus(&system_status, &self_test_results,
                               &system_error);

    sensorPtr->setExtCrystalUse(true);
}

int GYRO::read(void) {
    sensors_event_t event;
    sensorPtr->getEvent(&event);

    if (isGyroDisabled) {  // 地磁気
        magnetic = event.magnetic.x;
        deg = (int)(magnetic - offset + 360) % 360;
    } else {
        deg = (int)(event.orientation.x - offset + 360) % 360;
    }

    slope = (int)(event.orientation.y - slopeOffset + 360) % 360;

    if (slope >= 180) {
        slope -= 360;
    }
    slope *= -1;

    if (abs(slope) <= 4) {
        slope = 0;
    }
    directionDecision();

    // static int prevDeg = 0;
    // // 前回と±10度以上変わったらエラー
    // // ただし360と0とかは許容

    // if (abs(deg - prevDeg) > 10 && abs(deg - prevDeg) < 350) {
    //     setOffset();
    //     offset += prevDeg;
    //     offset %= 360;
    // }

    // prevDeg = deg;

    return deg;
}

void GYRO::setOffset(void) {
    sensors_event_t event;
    sensorPtr->getEvent(&event);
    offset = event.magnetic.x;

    if (isGyroDisabled) {  // 地磁気
        offset = event.magnetic.x;
    } else {
        offset = event.orientation.x;
    }

    // offset = 0;

    slopeOffset = event.orientation.y;
}

void GYRO::directionDecision(void) {
    if (deg >= 315 || deg < 45) {
        direction = NORTH;
    } else if (deg >= 45 && deg < 135) {
        direction = EAST;
    } else if (deg >= 135 && deg < 225) {
        direction = SOUTH;
    } else if (deg >= 225 && deg < 315) {
        direction = WEST;
    }
}

void GYRO::displaySensorOffsets(const adafruit_bno055_offsets_t &calibData) {
    uart1.print("Accelerometer: ");
    uart1.print(calibData.accel_offset_x);
    uart1.print(" ");
    uart1.print(calibData.accel_offset_y);
    uart1.print(" ");
    uart1.print(calibData.accel_offset_z);
    uart1.print(" ");

    uart1.print("\nGyro: ");
    uart1.print(calibData.gyro_offset_x);
    uart1.print(" ");
    uart1.print(calibData.gyro_offset_y);
    uart1.print(" ");
    uart1.print(calibData.gyro_offset_z);
    uart1.print(" ");

    uart1.print("\nMag: ");
    uart1.print(calibData.mag_offset_x);
    uart1.print(" ");
    uart1.print(calibData.mag_offset_y);
    uart1.print(" ");
    uart1.print(calibData.mag_offset_z);
    uart1.print(" ");

    uart1.print("\nAccel Radius: ");
    uart1.print(calibData.accel_radius);

    uart1.print("\nMag Radius: ");
    uart1.print(calibData.mag_radius);
}

#else

euler_t ypr;

GYRO::GYRO(Adafruit_BNO08x* p) {
    sensorPtr = p;
}

void GYRO::init(void) {
    // CHECK:これ多分いらない
    Wire.setSDA(PB9);
    Wire.setSCL(PB8);
    Wire.begin();
    delay(100);
    sensorPtr->begin_I2C();
    sensorPtr->enableReport(SH2_ARVR_STABILIZED_RV, 5000);
}

void GYRO::quaternionToEuler(float qr, float qi, float qj, float qk,
                             euler_t* ypr, bool degrees) {
    float sqr = sq(qr);
    float sqi = sq(qi);
    float sqj = sq(qj);
    float sqk = sq(qk);

    ypr->yaw = atan2(2.0 * (qi * qj + qk * qr), (sqi - sqj - sqk + sqr));
    ypr->pitch = asin(-2.0 * (qi * qk - qj * qr) / (sqi + sqj + sqk + sqr));
    ypr->roll = atan2(2.0 * (qj * qk + qi * qr), (-sqi - sqj + sqk + sqr));

    if (degrees) {
        ypr->yaw *= RAD_TO_DEG;
        ypr->pitch *= RAD_TO_DEG;
        ypr->roll *= RAD_TO_DEG;
    }
}

void GYRO::quaternionToEulerRV(sh2_RotationVectorWAcc_t* rotational_vector,
                               euler_t* ypr, bool degrees) {
    quaternionToEuler(rotational_vector->real, rotational_vector->i,
                      rotational_vector->j, rotational_vector->k, ypr, degrees);
}

int GYRO::read(void) {
    sh2_SensorValue_t sensorValue;
    if (sensorPtr->wasReset()) {
        sensorPtr->enableReport(SH2_ARVR_STABILIZED_RV, 5000);
    }

    // FIXME:
    // この辺でswitch文するのはどこいったんですか？（Adafruit公式ドキュメント見て）
    if (sensorPtr->getSensorEvent(&sensorValue)) {
        quaternionToEulerRV(&sensorValue.un.arvrStabilizedRV, &ypr, true);
    }

    deg = (int)(ypr.yaw * -1 - offset + 720) % 360;
    slope = (int)(ypr.pitch - slopeOffset + 720) % 360;

    if (slope >= 180) {
        slope -= 360;
    }
    slope *= -1;

    if (abs(slope) <= 4) {
        slope = 0;
    }

    directionDecision();

    return deg;
}

void GYRO::setOffset(void) {
    sh2_SensorValue_t sensorValue;
    // CHECK:この辺りの仕様を理解してから使おう
    if (sensorPtr->wasReset()) {
        sensorPtr->enableReport(SH2_ARVR_STABILIZED_RV, 5000);
    }

    if (sensorPtr->getSensorEvent(&sensorValue)) {
        quaternionToEulerRV(&sensorValue.un.arvrStabilizedRV, &ypr, true);
    }

    offset = (int)(ypr.yaw * -1 + 720) % 360;
    slopeOffset = (int)(ypr.pitch + 720) % 360;
}

void GYRO::directionDecision(void) {
    if (deg >= 315 || deg < 45) {
        direction = NORTH;
    } else if (deg >= 45 && deg < 135) {
        direction = EAST;
    } else if (deg >= 135 && deg < 225) {
        direction = SOUTH;
    } else if (deg >= 225 && deg < 315) {
        direction = WEST;
    }
}

void GYRO::displaySensorOffsets(const adafruit_bno055_offsets_t& calibData) {
    uart1.print("Accelerometer: ");
    uart1.print(calibData.accel_offset_x);
    uart1.print(" ");
    uart1.print(calibData.accel_offset_y);
    uart1.print(" ");
    uart1.print(calibData.accel_offset_z);
    uart1.print(" ");

    uart1.print("\nGyro: ");
    uart1.print(calibData.gyro_offset_x);
    uart1.print(" ");
    uart1.print(calibData.gyro_offset_y);
    uart1.print(" ");
    uart1.print(calibData.gyro_offset_z);
    uart1.print(" ");

    uart1.print("\nMag: ");
    uart1.print(calibData.mag_offset_x);
    uart1.print(" ");
    uart1.print(calibData.mag_offset_y);
    uart1.print(" ");
    uart1.print(calibData.mag_offset_z);
    uart1.print(" ");

    uart1.print("\nAccel Radius: ");
    uart1.print(calibData.accel_radius);

    uart1.print("\nMag Radius: ");
    uart1.print(calibData.mag_radius);
}

#endif