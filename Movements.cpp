#include "Movements.h"
#include <stdio.h>
#include <cmath>

// TODO : test it

Movements::Movements(Wheel& leftWheel, Wheel& rightWheel, Encoder& leftEncoder, Encoder& rightEncoder, PID& pid)
    : leftWheel(leftWheel),
      rightWheel(rightWheel),
      leftEncoder(leftEncoder),
      rightEncoder(rightEncoder),
      pid(pid) {
}

void Movements::MoveStraight(uint8_t target_speed) {
    // Get current speed from both encoders (frequency in Hz)
    float left_speed = leftEncoder.getFrequency();
    float right_speed = rightEncoder.getFrequency();

    printf("Target Speed: %u | Left Speed: %.2f Hz | Right Speed: %.2f Hz\n", 
           target_speed, left_speed, right_speed);
    
    // Right Wheel runs at target speed directly (reference Wheel)
    rightWheel.Rotate(target_speed, true);
    
    // Calculate speed error (in Hz)
    float speed_error = right_speed - left_speed;
    speed_error = pid.low_pass_filter((int32_t)(speed_error * 100), 0, 0.5f) / 100.0f; // Scale and filter the error
    
    // Use PID to compute a correction (not absolute PWM)
    // We scale the error to a reasonable range for PID
    uint8_t error_scaled = (uint8_t)fminf(fabs(speed_error), 255.0f);
    uint8_t correction = pid.compute(error_scaled, 0);
    
    // Apply correction to target speed
    int16_t left_pwm = target_speed;
    if (speed_error > 0) {
        // Left Wheel is slower, increase PWM
        left_pwm = target_speed + correction;
    } else {
        // Left Wheel is faster, decrease PWM
        left_pwm = target_speed - correction;
    }
    
    // Clamp to valid PWM range
    if (left_pwm < 0) left_pwm = 0;
    if (left_pwm > 250) left_pwm = 250;

    auto left_pwm_int = (uint8_t)left_pwm;
    
    printf("Speed Error: %.2f Hz | Correction: %u | Left PWM: %u\n", 
           speed_error, correction, left_pwm_int);
    
    // Move left Wheel with corrected speed
    leftWheel.Rotate(left_pwm_int, false);
}

void Movements::Rotate(uint8_t target_angle) {
    // For rotation, one Wheel goes forward, the other backward
    // The speed can be adjusted based on the target angle
    
    uint8_t rotation_speed = 100; // Default rotation speed
    
    if (target_angle > 180) {
        // Rotate right - left Wheel forward, right Wheel backward
        leftWheel.Rotate(rotation_speed, true);
        rightWheel.Rotate(rotation_speed, false);
    } else {
        // Rotate left - right Wheel forward, left Wheel backward
        leftWheel.Rotate(rotation_speed, false);
        rightWheel.Rotate(rotation_speed, true);
    }
}

void Movements::Stop() {
    // Stop both Wheels
    leftWheel.Stop();
    rightWheel.Stop();
    
    // Reset PID to avoid integral windup
    pid.reset();
}
