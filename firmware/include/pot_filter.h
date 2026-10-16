#ifndef POT_FILTER_H
#define POT_FILTER_H

#include <stdint.h>
#include "pot_config.h"

// The pots' smoothing: an EMA over the oversampled ADC reading, and a
// deadband (wider around the centre, where the ADC is noisiest) before the
// reported value follows it. No Arduino in here, so it is tested on the host
// (`pio test -e native`); potis.cpp feeds it from the ADC.

static inline uint16_t pot_map_clipped_to_output(uint16_t value) {
    if (value < POT_CLIP_MIN) value = POT_CLIP_MIN;
    if (value > POT_CLIP_MAX) value = POT_CLIP_MAX;

    const uint32_t inSpan = (uint32_t)POT_CLIP_MAX - (uint32_t)POT_CLIP_MIN;
    if (inSpan == 0u) return 1u;

    const uint32_t normalized = (uint32_t)value - (uint32_t)POT_CLIP_MIN;
    const uint32_t mapped = 1u + ((normalized * 4095u + (inSpan / 2u)) / inSpan);
    return (uint16_t)(4097u - mapped);
}

static inline uint8_t pot_effective_deadband(uint16_t currentStable) {
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

class PotBank {
public:
    // One oversampled ADC reading of pot `index` into its filter.
    void sample(uint8_t index, uint16_t raw) {
        if (index >= POT_MUX_COUNT) return;

        if (!initialized_[index]) {
            if (raw < POT_CLIP_MIN) raw = POT_CLIP_MIN;
            if (raw > POT_CLIP_MAX) raw = POT_CLIP_MAX;
            ema_[index] = raw;
            stable_[index] = raw;
            initialized_[index] = true;
            return;
        }

        // EMA smoothing in integer math.
        ema_[index] = (uint16_t)(((uint32_t)ema_[index] * ((1u << POT_EMA_SHIFT) - 1u) + raw) >> POT_EMA_SHIFT);
        int32_t diff = (int32_t)ema_[index] - (int32_t)stable_[index];
        if (diff < 0) diff = -diff;
        if ((uint32_t)diff >= pot_effective_deadband(stable_[index])) {
            stable_[index] = ema_[index];
        }

        if (stable_[index] < POT_CLIP_MIN) stable_[index] = POT_CLIP_MIN;
        if (stable_[index] > POT_CLIP_MAX) stable_[index] = POT_CLIP_MAX;
    }

    // What each pot reports, without sampling anything.
    void read(uint16_t *vals, uint8_t count) const {
        for (uint8_t i = 0; i < count && i < POT_MUX_COUNT; i++) {
            vals[i] = pot_map_clipped_to_output(stable_[i]);
        }
    }

private:
    bool initialized_[POT_MUX_COUNT] = {};
    uint16_t ema_[POT_MUX_COUNT] = {};
    uint16_t stable_[POT_MUX_COUNT] = {};
};

#endif // POT_FILTER_H
