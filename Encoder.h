# pragma once

#include "pico/stdlib.h"
#include <atomic>
#include <map>

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
    uint32_t GetPulseCount() const;

    /**
     * @brief Calculates the pulse frequency since the last call
     * @return motor speed value (0-255) corresponding to the frequency (max 360 Hz at 12V)
     */
    uint8_t GetMotorSpeed();

    /**
     * @brief Resets the pulse counter to zero
     */
    void Reset();

private:
    uint pin_;
    std::atomic<uint32_t> pulse_count_;
    uint32_t last_count_;
    absolute_time_t last_time_;
    static std::map<uint, Encoder*> instances_;

    /**
     * @brief ISR callback called on rising edge
     */
    static void gpio_callback(uint gpio, uint32_t events);

    /**
     * @brief Converts frequency (Hz) to motorspeed value (0-255)
     * @param frequency Frequency in Hz (max 360 Hz at 12V)
     * @return Motorspeed value (0-255)
     */
    static uint8_t FrequencyToMotorSpeed(float frequency);
};
