#include "LMotorController_ESP32.h"

LMotorController_ESP32::LMotorController_ESP32(int ena, int in1, int in2, int enb, int in3, int in4,
                                   double motorAConst, double motorBConst,
                                   int pwmFreq, int pwmResolution) {
    _ena = ena; _in1 = in1; _in2 = in2;
    _enb = enb; _in3 = in3; _in4 = in4;

    _motorAConst = motorAConst;
    _motorBConst = motorBConst;

    pinMode(_in1, OUTPUT);
    pinMode(_in2, OUTPUT);
    pinMode(_in3, OUTPUT);
    pinMode(_in4, OUTPUT);

    ledcSetup(_pwmChannelA, pwmFreq, pwmResolution);
    ledcAttachPin(_ena, _pwmChannelA);

    ledcSetup(_pwmChannelB, pwmFreq, pwmResolution);
    ledcAttachPin(_enb, _pwmChannelB);
}

void LMotorController_ESP32::setKickStart(bool enable, int power, int duration) {
    _kickStartEnabled = enable;
    _kickStartPower = constrain(power, 0, 255);
    _kickStartDuration = duration;
}

void LMotorController_ESP32::move(int leftSpeed, int rightSpeed, int minAbsSpeed) {
    leftSpeed = constrain(leftSpeed, -255, 255);
    rightSpeed = constrain(rightSpeed, -255, 255);

    int realLeft = max(abs(leftSpeed), minAbsSpeed) * _motorAConst;
    int realRight = max(abs(rightSpeed), minAbsSpeed) * _motorBConst;

    digitalWrite(_in1, leftSpeed > 0 ? HIGH : LOW);
    digitalWrite(_in2, leftSpeed > 0 ? LOW : HIGH);
    digitalWrite(_in3, rightSpeed > 0 ? HIGH : LOW);
    digitalWrite(_in4, rightSpeed > 0 ? LOW : HIGH);

    ledcWrite(_pwmChannelA, realRight);
    ledcWrite(_pwmChannelB, realLeft);

    _currentSpeed = (leftSpeed + rightSpeed) / 2;
}

void LMotorController_ESP32::move(int speed, int minAbsSpeed) {
    speed = constrain(speed, -255, 255);
    if (speed == _currentSpeed) return;

    int realSpeed = max(abs(speed), minAbsSpeed);
    digitalWrite(_in1, speed > 0 ? HIGH : LOW);
    digitalWrite(_in2, speed > 0 ? LOW : HIGH);
    digitalWrite(_in3, speed > 0 ? HIGH : LOW);
    digitalWrite(_in4, speed > 0 ? LOW : HIGH);

    ledcWrite(_pwmChannelA, realSpeed * _motorAConst);
    ledcWrite(_pwmChannelB, realSpeed * _motorBConst);

    _currentSpeed = speed;
}

void LMotorController_ESP32::move(int speed) {
    move(speed, 0);
}

void LMotorController_ESP32::turnLeft(int speed, bool kick) {
    digitalWrite(_in1, HIGH);
    digitalWrite(_in2, LOW);
    digitalWrite(_in3, LOW);
    digitalWrite(_in4, HIGH);

    if (_kickStartEnabled && (_currentSpeed == 0 || kick)) {
        ledcWrite(_pwmChannelA, _kickStartPower);
        ledcWrite(_pwmChannelB, _kickStartPower);
        delay(_kickStartDuration);
    }

    speed = constrain(speed, 0, 255);
    ledcWrite(_pwmChannelA, speed * _motorAConst);
    ledcWrite(_pwmChannelB, speed * _motorBConst);

    _currentSpeed = speed;
}

void LMotorController_ESP32::turnRight(int speed, bool kick) {
    digitalWrite(_in1, LOW);
    digitalWrite(_in2, HIGH);
    digitalWrite(_in3, HIGH);
    digitalWrite(_in4, LOW);

    if (_kickStartEnabled && (_currentSpeed == 0 || kick)) {
        ledcWrite(_pwmChannelA, _kickStartPower);
        ledcWrite(_pwmChannelB, _kickStartPower);
        delay(_kickStartDuration);
    }

    speed = constrain(speed, 0, 255);
    ledcWrite(_pwmChannelA, speed * _motorAConst);
    ledcWrite(_pwmChannelB, speed * _motorBConst);

    _currentSpeed = speed;
}

void LMotorController_ESP32::stopMoving() {
    digitalWrite(_in1, LOW);
    digitalWrite(_in2, LOW);
    digitalWrite(_in3, LOW);
    digitalWrite(_in4, LOW);
    ledcWrite(_pwmChannelA, 0);
    ledcWrite(_pwmChannelB, 0);
    _currentSpeed = 0;
}