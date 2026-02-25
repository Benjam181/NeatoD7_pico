#include "Movements.h"
#include <stdio.h>
#include <cmath>

// TODO : test it

Movements::Movements(Wheel& leftWheel, Wheel& rightWheel)
    : leftWheel(leftWheel),
      rightWheel(rightWheel),
      rotationActive(false),
      rotationClockwise(true),
      leftStartPulses(0),
      rightStartPulses(0),
      targetRotationPulses(0) {
}

void Movements::MoveStraight(uint8_t target_speed, bool direction) {
    rotationActive.store(false);
    rightWheel.SetTargetSpeed(target_speed, direction);
    leftWheel.SetTargetSpeed(target_speed, direction);
}

void Movements::Rotate(int16_t target_angle) {
    if (target_angle == 0) {
        Stop();
        return;
    }

    bool clockwise = target_angle > 0;
    uint32_t target_pulses = static_cast<uint32_t>(std::roundf(std::fabs(static_cast<float>(target_angle)) * pulsesPerDegree));
    if (target_pulses == 0) {
        target_pulses = 1;
    }

    leftStartPulses.store(leftWheel.GetPulseCount());
    rightStartPulses.store(rightWheel.GetPulseCount());
    targetRotationPulses.store(target_pulses);
    rotationClockwise.store(clockwise);
    rotationActive.store(true);

    if (clockwise) {
        leftWheel.SetTargetSpeed(rotationSpeed, true);
        rightWheel.SetTargetSpeed(rotationSpeed, false);
    } else {
        leftWheel.SetTargetSpeed(rotationSpeed, false);
        rightWheel.SetTargetSpeed(rotationSpeed, true);
    }
}

void Movements::UpdateControlLoop() {
    if (rotationActive.load()) {
        uint32_t left_progress = leftWheel.GetPulseCount() - leftStartPulses.load();
        uint32_t right_progress = rightWheel.GetPulseCount() - rightStartPulses.load();
        uint32_t average_progress = (left_progress + right_progress) / 2;

        if (average_progress >= targetRotationPulses.load()) {
            Stop();
            return;
        }
    }

    leftWheel.UpdateControl();
    rightWheel.UpdateControl();
}

void Movements::Stop() {
    rotationActive.store(false);
    targetRotationPulses.store(0);
    leftWheel.Stop();
    rightWheel.Stop();
}
