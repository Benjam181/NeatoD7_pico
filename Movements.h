# pragma once

#include "Wheel.h"
#include "PID.h"
#include <cstdint>
#include <atomic>

class Movements {
  public:
    Movements(Wheel& leftWheel, Wheel& rightWheel);
    void MoveStraight(uint8_t target_speed, bool direction);
    void Rotate(int16_t target_angle);
    void UpdateControlLoop();
    void Stop();

  private:
    static constexpr uint8_t rotationSpeed = 100;
    static constexpr float pulsesPerDegree = 1.0f;

    Wheel& leftWheel;
    Wheel& rightWheel;
    std::atomic<bool> rotationActive;
    std::atomic<bool> rotationClockwise;
    std::atomic<uint32_t> leftStartPulses;
    std::atomic<uint32_t> rightStartPulses;
    std::atomic<uint32_t> targetRotationPulses;
};