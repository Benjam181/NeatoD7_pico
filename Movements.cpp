#include "Movements.h"
#include <stdio.h>
#include <cmath>

// TODO : test it

Movements::Movements(Motor& leftMotor, Motor& rightMotor, Encoder& leftEncoder, Encoder& rightEncoder, PID& pid)
    : leftMotor(leftMotor),
      rightMotor(rightMotor),
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
    
    // Right motor runs at target speed directly (reference motor)
    rightMotor.Rotate(target_speed, true);
    
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
        // Left motor is slower, increase PWM
        left_pwm = target_speed + correction;
    } else {
        // Left motor is faster, decrease PWM
        left_pwm = target_speed - correction;
    }
    
    // Clamp to valid PWM range
    if (left_pwm < 0) left_pwm = 0;
    if (left_pwm > 250) left_pwm = 250;

    auto left_pwm_int = (uint8_t)left_pwm;
    
    printf("Speed Error: %.2f Hz | Correction: %u | Left PWM: %u\n", 
           speed_error, correction, left_pwm_int);
    
    // Move left motor with corrected speed
    leftMotor.Rotate(left_pwm_int, false);
}

void Movements::Rotate(uint8_t target_angle) {
    // For rotation, one motor goes forward, the other backward
    // The speed can be adjusted based on the target angle
    
    uint8_t rotation_speed = 100; // Default rotation speed
    
    if (target_angle > 180) {
        // Rotate right - left motor forward, right motor backward
        leftMotor.Rotate(rotation_speed, true);
        rightMotor.Rotate(rotation_speed, false);
    } else {
        // Rotate left - right motor forward, left motor backward
        leftMotor.Rotate(rotation_speed, false);
        rightMotor.Rotate(rotation_speed, true);
    }
}

void Movements::Stop() {
    // Stop both motors
    leftMotor.Stop();
    rightMotor.Stop();
    
    // Reset PID to avoid integral windup
    pid.reset();
}
