#include "MPU6050_ESP32.h"

volatile bool MPU6050_ESP32::mpuInterrupt = false;

MPU6050_ESP32::MPU6050_ESP32(uint8_t sdaPin, uint8_t sclPin, uint8_t intPin)
    : _sdaPin(sdaPin), _sclPin(sclPin), _intPin(intPin) {}

bool MPU6050_ESP32::begin() {
    Wire.begin(_sdaPin, _sclPin, 400000);
    mpu.initialize();
    if (!mpu.testConnection()) return false;

    uint8_t devStatus = mpu.dmpInitialize();
    if (devStatus != 0) return false;

    mpu.setDMPEnabled(true);
    attachInterrupt(digitalPinToInterrupt(_intPin), dmpDataReady, RISING);

    mpuIntStatus = mpu.getIntStatus();
    dmpReady = true;
    packetSize = mpu.dmpGetFIFOPacketSize();
    return true;
}

bool MPU6050_ESP32::update() {
    if (!dmpReady || !mpuInterrupt) return false;

    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();
    fifoCount = mpu.getFIFOCount();

    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        mpu.resetFIFO();
        return false;
    } else if (mpuIntStatus & 0x02) {
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        fifoCount -= packetSize;

        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
        return true;
    }
    return false;
}

float MPU6050_ESP32::getPitch() {
    return ypr[1] * 180 / M_PI + 180;
}

void MPU6050_ESP32::dmpDataReady() {
    mpuInterrupt = true;
}
