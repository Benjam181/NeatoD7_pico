#include "Movements.h"
#include <stdio.h>
#include <cmath>

// TODO : test it

Movements::Movements(Wheel& leftWheel, Wheel& rightWheel, PID& pid)
    : leftWheel(leftWheel),
      rightWheel(rightWheel),
      pid(pid) {
}

void Movements::MoveStraight(uint8_t target_speed) {
    // Get current speed from both encoders (frequency in Hz)
     uint8_t left_speed = leftWheel.GetCurrentSpeed();
    uint8_t right_speed = rightWheel.GetCurrentSpeed();
    
    // Right Wheel runs at target speed directly (reference Wheel)
    rightWheel.SetTargetSpeed(target_speed, false);
    int16_t speed_error = right_speed - left_speed;
    // speed_error = pid.low_pass_filter((int16_t)(speed_error * 100), 0, 0.5f) / 100.0f; // Scale and filter the error
    
    uint8_t error_scaled = (uint8_t)fminf(fabs(speed_error), 255.0f);
    uint8_t correction = pid.compute(error_scaled, 0);
    
    int16_t left_pwm = target_speed;
    if (speed_error > 0) {
        left_pwm = target_speed + correction;
    } else {
        left_pwm = target_speed - correction;
    }
    
    if (left_pwm < 0) left_pwm = 0;
    if (left_pwm > 255) left_pwm = 255;

    auto left_pwm_int = (uint8_t)left_pwm;
    
    printf(">Speed_Error:%.2f\n>Correction:%u\n>PWM: %u\n", 
            speed_error, correction, left_pwm_int);

    printf(">Target_Speed:%u\n>Left_Speed:%u\n>Right_Speed:%u\n", 
           target_speed, left_speed, right_speed);
    
    leftWheel.SetTargetSpeed(left_pwm_int, false);
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
    
    // Reset PID to avoid integral windup
    pid.reset();
}
