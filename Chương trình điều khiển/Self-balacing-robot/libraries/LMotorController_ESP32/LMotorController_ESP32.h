#ifndef LMotorController_ESP32_h
#define LMotorController_ESP32_hh

#include "Arduino.h"

class LMotorController_ESP32 {
public:
    LMotorController_ESP32(int ena, int in1, int in2, int enb, int in3, int in4,
                     double motorAConst = 1.0, double motorBConst = 1.0,
                     int pwmFreq = 20000, int pwmResolution = 8);

    void move(int leftSpeed, int rightSpeed, int minAbsSpeed);
    void move(int speed, int minAbsSpeed);
    void move(int speed);
    void turnLeft(int speed, bool kick = false);
    void turnRight(int speed, bool kick = false);
    void stopMoving();

    void setKickStart(bool enable, int power = 255, int duration = 100);

private:
    int _ena, _in1, _in2;
    int _enb, _in3, _in4;
    int _pwmChannelA = 0;
    int _pwmChannelB = 1;

    double _motorAConst;
    double _motorBConst;

    int _currentSpeed = 0;

    bool _kickStartEnabled = false;
    int _kickStartPower = 255;
    int _kickStartDuration = 100;
};

#endif