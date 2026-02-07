# pragma once

#include "Motor.h"
#include "Encoder.h"
#include "PID.h"
#include <cstdint>

class Movements {
  public:
    Movements(Motor& leftMotor, Motor& rightMotor, Encoder& leftEncoder, Encoder& rightEncoder, PID& leftPID, PID& rightPID);
    void MoveStraight(uint8_t target_speed);
    void Rotate(uint8_t target_angle);
    void Stop();

  private:
    Motor& leftMotor;
    Motor& rightMotor;
    Encoder& leftEncoder;
    Encoder& rightEncoder;
    PID& leftPID;
    PID& rightPID;
};