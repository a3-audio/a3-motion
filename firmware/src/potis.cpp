#include "potis.h"
#include "multiplexer_map.h"

void potis_read(uint16_t *vals, uint8_t count) {
    static bool initialized = false;
    static uint16_t ema[POT_MUX_COUNT] = {0};
    static uint16_t stable[POT_MUX_COUNT] = {0};

    for (uint8_t i = 0; i < count && i < POT_MUX_COUNT; i++) {
        // All POTIS are DIRECT_INPUT (no MUX): read ADC pin directly.
        uint32_t sum = 0;
        for (uint8_t s = 0; s < POT_OVERSAMPLE_COUNT; s++) {
            sum += (uint16_t)analogRead(POTIS[i].gpio);
        }
        uint16_t raw = (uint16_t)(sum / POT_OVERSAMPLE_COUNT);

        if (!initialized) {
            ema[i] = raw;
            stable[i] = raw;
        } else {
            // EMA smoothing in integer math.
            ema[i] = (uint16_t)(((uint32_t)ema[i] * ((1u << POT_EMA_SHIFT) - 1u) + raw) >> POT_EMA_SHIFT);
            int32_t diff = (int32_t)ema[i] - (int32_t)stable[i];
            if (diff < 0) diff = -diff;
            if ((uint32_t)diff >= POT_DEADBAND) {
                stable[i] = ema[i];
            }
        }

        vals[i] = stable[i];
    }

    initialized = true;
}
