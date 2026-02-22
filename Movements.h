# pragma once

#include "Wheel.h"
#include "PID.h"
#include <cstdint>

class Movements {
  public:
    Movements(Wheel& leftWheel, Wheel& rightWheel, PID& pid);
    void MoveStraight(uint8_t target_speed);
    void Rotate(uint8_t target_angle);
    void Stop();

  private:
    Wheel& leftWheel;
    Wheel& rightWheel;
    PID& pid;
};