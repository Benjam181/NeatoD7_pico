# pragma once

#include "Wheel.h"
#include "Encoder.h"
#include "PID.h"
#include <cstdint>

class Movements {
  public:
    Movements(Wheel& leftWheel, Wheel& rightWheel, Encoder& leftEncoder, Encoder& rightEncoder, PID& pid);
    void MoveStraight(uint8_t target_speed);
    void Rotate(uint8_t target_angle);
    void Stop();

  private:
    Wheel& leftWheel;
    Wheel& rightWheel;
    Encoder& leftEncoder;
    Encoder& rightEncoder;
    PID& pid;
};