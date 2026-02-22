#pragma once

#include <cstdint>
#include "PID.h"
#include "Encoder.h"

class Wheel {
  public:
    Wheel(uint8_t pwmPin, uint8_t dirPin, Encoder& encoder, PID& pid);
    void SetTargetSpeed(uint8_t target_speed, bool clockwise);
    void Stop();
    uint8_t GetCurrentSpeed();

  private:
    void Rotate(uint8_t speed, bool clockwise);

    uint8_t pwmPin;
    uint8_t dirPin;
    uint32_t sliceNum;
    uint32_t pwmChannel; // PWM channel (A or B)
    Encoder& encoder;
    PID& pid;
    uint8_t previous_speed; // For low-pass filtering
};