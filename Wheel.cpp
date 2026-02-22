#include "Wheel.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include <stdio.h>

// TODO: test it

Wheel::Wheel(uint8_t pwmPin, uint8_t dirPin, Encoder& encoder, PID& pid) 
    : pwmPin(pwmPin), dirPin(dirPin), encoder(encoder), pid(pid) {
    // Initialize the GPIO pins for PWM and direction control
    gpio_init(pwmPin);
    gpio_set_dir(pwmPin, GPIO_OUT);
    gpio_set_function(pwmPin, GPIO_FUNC_PWM);
    sliceNum = pwm_gpio_to_slice_num(pwmPin);
    pwmChannel = pwm_gpio_to_channel(pwmPin); // Get the correct PWM channel (A or B)
    
    // Set frequency (e.g., 1kHz)
    pwm_set_wrap(sliceNum, 12499); 
    pwm_set_clkdiv(sliceNum, 10.0f);
    pwm_set_chan_level(sliceNum, pwmChannel, 0);
    pwm_set_enabled(sliceNum, true);

    gpio_init(dirPin);
    gpio_set_dir(dirPin, GPIO_OUT);
}

void Wheel::Rotate(uint8_t speed, bool direction) {
    // Set the direction pin based on the desired direction
    gpio_put(dirPin, direction);
    
    // Set the PWM pin to the desired speed (0-255)
    pwm_set_chan_level(sliceNum, pwmChannel, speed * 49); // Scale 0-255 to 0-12499
}

void Wheel::SetTargetSpeed(uint8_t target_speed, bool clockwise) {
    uint8_t current_speed = GetCurrentSpeed();
    uint8_t pid_output = pid.compute(target_speed, current_speed);
    Rotate(pid_output, clockwise);
    printf(">Target_speed:%u\n>Current_Speed:%u\n>PID_Output:%u\n", 
           target_speed, current_speed, pid_output);
}

void Wheel::Stop() {
    // Set the PWM level to 0 to stop the motor
    pwm_set_chan_level(sliceNum, pwmChannel, 0);
    pid.reset();
}

uint8_t Wheel::GetCurrentSpeed() {
    uint8_t current_speed = encoder.GetMotorSpeed();
    current_speed = pid.low_pass_filter(current_speed, previous_speed, 0.05f);
    previous_speed = current_speed;
    return current_speed;
}