#include "pico/stdlib.h"
#include "Encoder.h"
#include <stdio.h>

int main() {
    stdio_init_all();

    const uint ENCODER_PIN = 0;

    Encoder encoder(ENCODER_PIN);

    while (true) {
        sleep_ms(100);

        uint32_t total_pulses = encoder.getPulseCount();
        float frequency = encoder.getFrequency();

        printf("Total pulses: %u, Frequency: %.1f Hz\n", 
               total_pulses, frequency);
    }
}
