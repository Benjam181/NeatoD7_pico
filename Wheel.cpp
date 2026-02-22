#include "Wheel.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include <stdio.h>

// TODO: test it

Wheel::Wheel(uint8_t pwmPin1, uint8_t pwmPin2, Encoder& encoder, PID& pid) 
    : pwmPin1(pwmPin1), pwmPin2(pwmPin2), encoder(encoder), pid(pid), 
      last_direction(true), direction_initialized(false) {
    // Initialize PWM pin 1
    gpio_init(pwmPin1);
    gpio_set_dir(pwmPin1, GPIO_OUT);
    gpio_set_function(pwmPin1, GPIO_FUNC_PWM);
    sliceNum1 = pwm_gpio_to_slice_num(pwmPin1);
    pwmChannel1 = pwm_gpio_to_channel(pwmPin1);

    // Initialize PWM pin 2
    gpio_init(pwmPin2);
    gpio_set_dir(pwmPin2, GPIO_OUT);
    gpio_set_function(pwmPin2, GPIO_FUNC_PWM);
    sliceNum2 = pwm_gpio_to_slice_num(pwmPin2);
    pwmChannel2 = pwm_gpio_to_channel(pwmPin2);
    
    // Set frequency (e.g., 1kHz) - Configure slices only once
    // If both pins are on the same slice, configure it only once
    pwm_set_wrap(sliceNum1, 12499); 
    pwm_set_clkdiv(sliceNum1, 10.0f);
    pwm_set_chan_level(sliceNum1, pwmChannel1, 0);
    
    if (sliceNum2 != sliceNum1) {
        // Different slices, configure the second one
        pwm_set_wrap(sliceNum2, 12499); 
        pwm_set_clkdiv(sliceNum2, 10.0f);
    }
    pwm_set_chan_level(sliceNum2, pwmChannel2, 0);
    
    // Enable the slices
    pwm_set_enabled(sliceNum1, true);
    if (sliceNum2 != sliceNum1) {
        pwm_set_enabled(sliceNum2, true);
    }
}

void Wheel::Rotate(uint8_t speed, bool clockwise) {    
    // Scale 0-255 to 0-12499
    uint16_t pwm_level = speed * 49;
    
    if (clockwise) {
        // Clockwise: pin1 = PWM, pin2 = 0
        pwm_set_chan_level(sliceNum1, pwmChannel1, pwm_level);
    } else {
        // Counter-clockwise: pin1 = 0, pin2 = PWM
        pwm_set_chan_level(sliceNum2, pwmChannel2, pwm_level);
    }
    
    // Update direction tracking
    last_direction = clockwise;
    direction_initialized = true;
}

void Wheel::SetTargetSpeed(uint8_t target_speed, bool clockwise) {
    current_speed = pid.low_pass_filter(encoder.GetMotorSpeed(), previous_speed, 0.05f);
    previous_speed = current_speed;
    uint8_t pid_output = pid.compute(target_speed, current_speed);
    Rotate(pid_output, clockwise);
}

void Wheel::Stop() {
    // Set both PWM levels to 0 to stop the motor
    pwm_set_chan_level(sliceNum1, pwmChannel1, 0);
    pwm_set_chan_level(sliceNum2, pwmChannel2, 0);
    pid.reset();
    direction_initialized = false; // Reset direction tracking
}

uint8_t Wheel::GetCurrentSpeed() {
    return current_speed;
}