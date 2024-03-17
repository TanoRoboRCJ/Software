#include <Arduino.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <EEPROM.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_BNO08x.h>

#define Serial uart1
HardwareSerial uart1(PA10, PA9);

#include <Arduino.h>
// This demo explores two reports (SH2_ARVR_STABILIZED_RV and
// SH2_GYRO_INTEGRATED_RV) both can be used to give quartenion and euler (yaw,
// pitch roll) angles.  Toggle the FAST_MODE define to see other report. Note
// sensorValue.status gives calibration accuracy (which improves over time)
#include <Adafruit_BNO08x.h>

#define BNO08X_RESET -1

struct euler_t {
    float yaw;
    float pitch;
    float roll;
} ypr;

Adafruit_BNO08x bno08x(BNO08X_RESET);
sh2_SensorValue_t sensorValue;

void setup(void) {
    Wire.setSDA(PB9);
    Wire.setSCL(PB8);
    Wire.begin();

    Serial.begin(115200);
    while (!Serial)
        delay(10);  // will pause Zero, Leonardo, etc until serial console opens

    Serial.println("Adafruit BNO08x test!");

    if (!bno08x.begin_I2C()) {
        Serial.println("Failed to find BNO08x chip");
        while (1) {
            delay(10);
        }
    }
    Serial.println("BNO08x Found!");

    bno08x.enableReport(SH2_ARVR_STABILIZED_RV, 5000);

    Serial.println("Reading events");
    delay(100);
}

void quaternionToEuler(float qr, float qi, float qj, float qk, euler_t* ypr,
                       bool degrees = false) {
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

void quaternionToEulerRV(sh2_RotationVectorWAcc_t* rotational_vector,
                         euler_t* ypr, bool degrees = false) {
    quaternionToEuler(rotational_vector->real, rotational_vector->i,
                      rotational_vector->j, rotational_vector->k, ypr, degrees);
}

void quaternionToEulerGI(sh2_GyroIntegratedRV_t* rotational_vector,
                         euler_t* ypr, bool degrees = false) {
    quaternionToEuler(rotational_vector->real, rotational_vector->i,
                      rotational_vector->j, rotational_vector->k, ypr, degrees);
}

void loop() {
    if (bno08x.wasReset()) {
        Serial.print("sensor was reset ");
        bno08x.enableReport(SH2_ARVR_STABILIZED_RV, 5000);
    }

    if (bno08x.getSensorEvent(&sensorValue)) {
        quaternionToEulerRV(&sensorValue.un.arvrStabilizedRV, &ypr, true);
        
        Serial.print(sensorValue.status);
        Serial.print("\t");  // This is accuracy in the range of 0 to 3
        Serial.print(ypr.yaw);
        Serial.print("\t");
        Serial.print(ypr.pitch);
        Serial.print("\t");
        Serial.println(ypr.roll);
    }
}