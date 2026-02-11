#pragma once
#include <stdint.h>

class PID {
private:
    float kp; // Proportional gain
    float ki; // Integral gain
    float kd; // Derivative gain
    float dt; // Time step
    float integral; // Integral term
    float previous_error; // Previous error for derivative calculation
    float anti_windup; // Anti-windup term to prevent integral windup
    float u_min; // Minimum output value
    float u_max; // Maximum output value

public:
    // Constructor
    PID(float kp, float ki, float kd, float dt, float u_min, float u_max);
    
    // Compute PID output
    uint8_t compute(uint8_t setpoint, uint8_t measured_value);
    
    // Reset PID state
    void reset();
    
    // Set PID gains
    void setGains(float kp, float ki, float kd);
    
    // Set time step
    void setTimeStep(float dt);

    int32_t low_pass_filter(int32_t input, int32_t previous_output, float alpha);
};
