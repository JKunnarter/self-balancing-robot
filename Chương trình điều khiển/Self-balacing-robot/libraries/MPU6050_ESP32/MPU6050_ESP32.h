#ifndef MPU6050_ESP32_H
#define MPU6050_ESP32_H

#include <MPU6050_6Axis_MotionApps20.h>
#include <Wire.h>

class MPU6050_ESP32 {
public:
    MPU6050_ESP32(uint8_t sdaPin, uint8_t sclPin, uint8_t intPin);
    bool begin();
    bool update();
    float getPitch();

private:
    MPU6050 mpu;
    uint8_t _sdaPin, _sclPin, _intPin;
    bool dmpReady = false;
    uint8_t mpuIntStatus;
    uint16_t packetSize;
    uint16_t fifoCount;
    uint8_t fifoBuffer[64];
    Quaternion q;
    VectorFloat gravity;
    float ypr[3];
    static void dmpDataReady();
    static volatile bool mpuInterrupt;
};

#endif
