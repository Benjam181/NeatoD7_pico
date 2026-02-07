#pragma once

#include <cstdint>

class Motor {
  public:
    Motor(uint8_t pwmPin, uint8_t dirPin);
    void Rotate(uint8_t speed, bool clockwise);
    void Stop();

  private:
    uint8_t pwmPin;
    uint8_t dirPin;
    uint32_t sliceNum;
};