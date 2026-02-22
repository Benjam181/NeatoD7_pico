#pragma once

#include <cstdint>
#include "PID.h"
#include "Encoder.h"

class Wheel {
  public:
    Wheel(uint8_t pwmPin1, uint8_t pwmPin2, Encoder& encoder, PID& pid);
    void SetTargetSpeed(uint8_t target_speed, bool clockwise);
    void Stop();
    uint8_t GetCurrentSpeed();

  private:
    void Rotate(uint8_t speed, bool clockwise);

    uint8_t pwmPin1;
    uint8_t pwmPin2;
    uint32_t sliceNum1;
    uint32_t pwmChannel1; // PWM channel (A or B) for pin1
    uint32_t sliceNum2;
    uint32_t pwmChannel2; // PWM channel (A or B) for pin2
    Encoder& encoder;
    PID& pid;
    uint8_t previous_speed; // For low-pass filtering
    uint8_t current_speed;
    bool last_direction; // Track last direction to detect changes
    bool direction_initialized; // Flag to check if direction has been set
};