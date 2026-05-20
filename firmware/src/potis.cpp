#include "potis.h"
#include "multiplexer_map.h"

static uint16_t map_clipped_to_output(uint16_t value) {
    if (value < POT_CLIP_MIN) value = POT_CLIP_MIN;
    if (value > POT_CLIP_MAX) value = POT_CLIP_MAX;

    const uint32_t inSpan = (uint32_t)POT_CLIP_MAX - (uint32_t)POT_CLIP_MIN;
    if (inSpan == 0u) return 1u;

    const uint32_t normalized = (uint32_t)value - (uint32_t)POT_CLIP_MIN;
    const uint32_t mapped = 1u + ((normalized * 4095u + (inSpan / 2u)) / inSpan);
    return (uint16_t)(4097u - mapped);
}

static uint8_t effective_deadband(uint16_t currentStable) {
    // Add extra hysteresis around the center position where ADC noise is most noticeable.
    const uint16_t center = (uint16_t)((POT_CLIP_MIN + POT_CLIP_MAX) / 2u);
    uint16_t dist = (currentStable > center)
        ? (uint16_t)(currentStable - center)
        : (uint16_t)(center - currentStable);

    if (dist <= POT_CENTER_BAND_RAW) {
        return (uint8_t)(POT_DEADBAND + POT_DEADBAND_CENTER_BONUS);
    }
    return POT_DEADBAND;
}

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
            if (raw < POT_CLIP_MIN) raw = POT_CLIP_MIN;
            if (raw > POT_CLIP_MAX) raw = POT_CLIP_MAX;
            ema[i] = raw;
            stable[i] = raw;
        } else {
            // EMA smoothing in integer math.
            ema[i] = (uint16_t)(((uint32_t)ema[i] * ((1u << POT_EMA_SHIFT) - 1u) + raw) >> POT_EMA_SHIFT);
            int32_t diff = (int32_t)ema[i] - (int32_t)stable[i];
            if (diff < 0) diff = -diff;
            if ((uint32_t)diff >= effective_deadband(stable[i])) {
                stable[i] = ema[i];
            }
        }

        if (stable[i] < POT_CLIP_MIN) stable[i] = POT_CLIP_MIN;
        if (stable[i] > POT_CLIP_MAX) stable[i] = POT_CLIP_MAX;
        vals[i] = map_clipped_to_output(stable[i]);
    }

    initialized = true;
}
