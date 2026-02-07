#include "pico/stdlib.h"
#include "Encoder.h"
#include <stdio.h>

int main() {
    stdio_init_all();

    const uint ENCODER_PIN = 0; // Change according to your wiring

    // Create the encoder
    Encoder encoder(ENCODER_PIN);

    while (true) {
        sleep_ms(100); // Measure every 100 ms

        uint32_t total_pulses = encoder.getPulseCount();
        float frequency = encoder.getFrequency();

        printf("Total pulses: %u, Vitesse: %.1f Hz\n", 
               total_pulses, frequency);
    }
}
