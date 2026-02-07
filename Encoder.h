#ifndef ENCODER_H
#define ENCODER_H

#include "pico/stdlib.h"
#include <atomic>

class Encoder {
public:
    /**
     * @brief Constructs an encoder on a GPIO pin
     * @param gpio_pin GPIO pin number connected to the encoder
     */
    Encoder(uint gpio_pin);
    
    /**
     * @brief Destructor - disables the interrupt
     */
    ~Encoder();

    /**
     * @brief Returns the total number of pulses since startup
     * @return Number of pulses
     */
    uint32_t getPulseCount() const;

    /**
     * @brief Calculates the pulse frequency since the last call
     * @return Frequency in Hz (pulses/second)
     */
    float getFrequency();

    /**
     * @brief Resets the pulse counter to zero
     */
    void reset();

private:
    uint pin_;
    std::atomic<uint32_t> pulse_count_;
    uint32_t last_count_;
    absolute_time_t last_time_;

    /**
     * @brief ISR callback called on rising edge
     */
    static void gpio_callback(uint gpio, uint32_t events);

    // Static instance for the callback
    static Encoder* instance_;
};

#endif // ENCODER_H
