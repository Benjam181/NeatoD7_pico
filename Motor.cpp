#include "Motor.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"

// TODO: test it

Motor::Motor(uint8_t pwmPin, uint8_t dirPin) 
    : pwmPin(pwmPin), dirPin(dirPin) {
    // Initialize the GPIO pins for PWM and direction control
    gpio_init(pwmPin);
    gpio_set_dir(pwmPin, GPIO_OUT);
    gpio_set_function(pwmPin, GPIO_FUNC_PWM);
    sliceNum = pwm_gpio_to_slice_num(pwmPin);
        // Set frequency (e.g., 1kHz)
    pwm_set_wrap(sliceNum, 12499); 
    pwm_set_clkdiv(sliceNum, 10.0f);
    pwm_set_chan_level(sliceNum, PWM_CHAN_A, 0);
    pwm_set_enabled(sliceNum, true);

    gpio_init(dirPin);
    gpio_set_dir(dirPin, GPIO_OUT);
}

void Motor::Rotate(uint8_t speed, bool direction) {
    // Set the direction pin based on the desired direction
    gpio_put(dirPin, direction);
    
    // Set the PWM pin to the desired speed (0-255)
    pwm_set_chan_level(sliceNum, PWM_CHAN_A, speed * 49); // Scale 0-255 to 0-12499
}

void Motor::Stop() {
    // Set the PWM level to 0 to stop the motor
    pwm_set_chan_level(sliceNum, PWM_CHAN_A, 0);
}