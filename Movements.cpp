#include "Movements.h"

// TODO : test it

Movements::Movements(Motor& leftMotor, Motor& rightMotor, Encoder& leftEncoder, Encoder& rightEncoder, PID& leftPID, PID& rightPID)
    : leftMotor(leftMotor),
      rightMotor(rightMotor),
      leftEncoder(leftEncoder),
      rightEncoder(rightEncoder),
      leftPID(leftPID),
      rightPID(rightPID) {
}

void Movements::MoveStraight(uint8_t target_speed) {
    // Get current speed from both encoders
    float left_speed = leftEncoder.getFrequency();
    float right_speed = rightEncoder.getFrequency();
    
    // Compute PID correction for each motor independently
    float left_control = PID_Compute(&leftPID, target_speed, left_speed);
    float right_control = PID_Compute(&rightPID, target_speed, right_speed);
    
    // Clamp the control outputs to valid PWM range (0-255)
    uint8_t left_motor_speed = (left_control < 0) ? 0 : 
                               (left_control > 255) ? 255 : 
                               (uint8_t)left_control;
    
    uint8_t right_motor_speed = (right_control < 0) ? 0 : 
                                (right_control > 255) ? 255 : 
                                (uint8_t)right_control;
    
    // Move both motors forward with their individual PID-controlled speeds
    leftMotor.Rotate(left_motor_speed, true);
    rightMotor.Rotate(right_motor_speed, true);
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
    
    // Reset both PIDs to avoid integral windup
    PID_Reset(&leftPID);
    PID_Reset(&rightPID);
}
