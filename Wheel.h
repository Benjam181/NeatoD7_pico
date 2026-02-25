#pragma once

#include <cstdint>
#include <atomic>
#include "PID.h"
#include "Encoder.h"

class Wheel {
  public:
    Wheel(uint8_t pwmPin1, uint8_t pwmPin2, Encoder& encoder, PID& pid);
    void SetTargetSpeed(uint8_t target_speed, bool clockwise);
    void UpdateControl();
    void Stop();
    uint8_t GetCurrentSpeed();
    uint32_t GetPulseCount() const;

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
    std::atomic<uint8_t> current_speed;
    std::atomic<uint8_t> target_speed;
    std::atomic<bool> target_clockwise;
    std::atomic<bool> target_initialized;
    bool last_direction; // Track last direction to detect changes
    bool direction_initialized; // Flag to check if direction has been set
};