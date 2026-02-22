#include "Movements.h"
#include <stdio.h>
#include <cmath>

// TODO : test it

Movements::Movements(Wheel& leftWheel, Wheel& rightWheel)
    : leftWheel(leftWheel),
      rightWheel(rightWheel) {
}

void Movements::MoveStraight(uint8_t target_speed, bool direction) {
    // todo set a PID if robot don't move straight
    rightWheel.SetTargetSpeed(target_speed, direction);
    leftWheel.SetTargetSpeed(target_speed, direction);
}

void Movements::Rotate(uint8_t target_angle) {
    // For rotation, one Wheel goes forward, the other backward
    // The speed can be adjusted based on the target angle
    
    uint8_t rotation_speed = 100; // Default rotation speed
    
    if (target_angle > 180) {
        // Rotate right - left Wheel forward, right Wheel backward
        leftWheel.SetTargetSpeed(rotation_speed, true);
        rightWheel.SetTargetSpeed(rotation_speed, false);
    } else {
        // Rotate left - right Wheel forward, left Wheel backward
        leftWheel.SetTargetSpeed(rotation_speed, false);
        rightWheel.SetTargetSpeed(rotation_speed, true);
    }
}

void Movements::Stop() {
    // Stop both Wheels
    leftWheel.Stop();
    rightWheel.Stop();
}
