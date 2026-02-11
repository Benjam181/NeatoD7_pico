#include "PID.h"
#include <cmath>

// Constructor
PID::PID(float kp, float ki, float kd, float dt, float u_min, float u_max)
    : kp(kp), ki(ki), kd(kd), dt(dt), u_min(u_min), u_max(u_max),
      integral(0.0f), previous_error(0.0f), anti_windup(100.0f) {
}

uint8_t PID::compute(uint8_t setpoint, uint8_t measured_value) {
    float error = (float)setpoint - (float)measured_value;

    // Proportional term
    float proportional = kp * error;

    // Integral term
    integral += ki * error * dt;

    // Anti-windup
    if (integral > anti_windup) {
        integral = anti_windup;
    } else if (integral < -anti_windup) {
        integral = -anti_windup;
    }

    // Derivative term
    float derivative = kd * (error - previous_error) / dt;

    // Update previous error
    previous_error = error;

    float output = proportional + integral + derivative;
    
    output = fminf(fmaxf(output, u_min), u_max);
    
    return (uint8_t)output;
}

void PID::reset() {
    integral = 0.0f;
    previous_error = 0.0f;
}

void PID::setGains(float kp, float ki, float kd) {
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
}

void PID::setTimeStep(float dt) {
    this->dt = dt;
}

int32_t PID::low_pass_filter(int32_t input, int32_t previous_output, float alpha) {
    // Apply a simple low-pass filter
    return (int32_t)(alpha * input + (1.0f - alpha) * previous_output);
}