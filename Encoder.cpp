#include "Encoder.h"
#include <stdio.h>

// Static map instance for the callback
std::map<uint, Encoder*> Encoder::instances_;

Encoder::Encoder(uint gpio_pin) 
    : pin_(gpio_pin), 
      pulse_count_(0),
      last_count_(0),
      last_time_(get_absolute_time()) {
    
    // Initialize the GPIO
    gpio_init(pin_);
    gpio_set_dir(pin_, GPIO_IN);
    gpio_pull_up(pin_);

    // Register this instance for the callback
    instances_[pin_] = this;

    // Attach the ISR on rising edge
    gpio_set_irq_enabled_with_callback(pin_,
        GPIO_IRQ_EDGE_RISE, true, &Encoder::gpio_callback);
}

Encoder::~Encoder() {
    // Disable the interrupt
    gpio_set_irq_enabled(pin_, GPIO_IRQ_EDGE_RISE, false);
    instances_.erase(pin_);
}

uint32_t Encoder::getPulseCount() const {
    return pulse_count_.load();
}

float Encoder::getFrequency() {
    uint32_t current_count = pulse_count_.load();
    uint32_t delta_count = current_count - last_count_;
    
    absolute_time_t current_time = get_absolute_time();
    int64_t dt_us = absolute_time_diff_us(last_time_, current_time);
    
    float frequency = 0.0f;
    if(dt_us > 0) {
        float dt_seconds = (float)dt_us / 1000000.0f;
        frequency = (float)delta_count / dt_seconds;
    }
    
    // Update for the next call
    last_count_ = current_count;
    last_time_ = current_time;
    
    return frequency;
}

void Encoder::reset() {
    pulse_count_.store(0);
    last_count_ = 0;
    last_time_ = get_absolute_time();
}

void Encoder::gpio_callback(uint gpio, uint32_t events) {
    (void)gpio;
    (void)events;
    
    auto it = instances_.find(gpio);
    if (it != instances_.end() && it->second != nullptr) {
        it->second->pulse_count_++;
    }
}
