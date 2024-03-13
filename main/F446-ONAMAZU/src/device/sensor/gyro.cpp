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
        uart1.println(
            "\nNo Calibration Data for this sensor exists in EEPROM");
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

    if (abs(slope) <= 8) {
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

GYRO::GYRO(Adafruit_BNO055 *p) {
}

void GYRO::init(void) {
    mpu.initialize();
    if (mpu.testConnection() != true) {
        Serial.println("MPU disconection");
        while (true) {
        }
    }
    if (mpu.dmpInitialize() != 0) {
        Serial.println("MPU break");
        while (true) {
        }
    }
    mpu.setXGyroOffset(Gyro_X);
    mpu.setYGyroOffset(Gyro_Y);
    mpu.setZGyroOffset(Gyro_Z);
    mpu.setZAccelOffset(Accel_Z);
    mpu.setDMPEnabled(true);
    mpuIntStatus = mpu.getIntStatus();
    dmpReady = true;
    packetSize = mpu.dmpGetFIFOPacketSize();
}

int GYRO::read(void) {
    mpuIntStatus = false;
    mpuIntStatus = mpu.getIntStatus();
    fifoCount = mpu.getFIFOCount();

    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        mpu.resetFIFO();
    } else if (mpuIntStatus & 0x02) {
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        fifoCount -= packetSize;
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
        slope = degrees(ypr[2]) - slopeOffset;
        deg = degrees(ypr[0]) - offset;

        while (deg < 0) {
            deg += 360;
        }
        deg %= 360;

        while (slope < 0) {
            slope += 360;
        }
        slope %= 360;

        if (slope >= 180) {
            slope -= 360;
        }
    }

    if (abs(slope) <= 8) {
        slope = 0;
    }

    directionDecision();

    return deg;
}

void GYRO::setOffset(void) {
    // mpuIntStatus = false;
    // mpuIntStatus = mpu.getIntStatus();
    // fifoCount = mpu.getFIFOCount();

    // if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
    //     mpu.resetFIFO();
    // } else if (mpuIntStatus & 0x02) {
    //     while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
    //     mpu.getFIFOBytes(fifoBuffer, packetSize);
    //     fifoCount -= packetSize;
    //     mpu.dmpGetQuaternion(&q, fifoBuffer);
    //     mpu.dmpGetGravity(&gravity, &q);
    //     mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
    //     slopeOffset = degrees(ypr[2]);
    //     offset = degrees(ypr[0]);

    //     if (offset < 0) offset += 360;
    //     if (offset > 359) offset %= 360;
    //     if (slopeOffset < 0) slopeOffset += 360;
    //     if (slopeOffset > 359) slopeOffset %= 360;
    // }
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

#endif