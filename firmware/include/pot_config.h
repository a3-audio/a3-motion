#ifndef POT_CONFIG_H
#define POT_CONFIG_H

#include <stdint.h>

// The pots' settings, apart from config.h so that pot_filter.h -- and its
// host-side test -- need no Arduino.

constexpr uint8_t POT_MUX_COUNT = 4;
constexpr uint8_t POT_OVERSAMPLE_COUNT = 8;
constexpr uint8_t POT_EMA_SHIFT = 2;      // stronger smoothing to calm ADC noise
constexpr uint8_t POT_DEADBAND = 5;       // suppress small ADC jitter
constexpr uint16_t POT_CLIP_MIN = 6;
constexpr uint16_t POT_CLIP_MAX = 4088;
constexpr uint16_t POT_CENTER_BAND_RAW = 900;
constexpr uint8_t POT_DEADBAND_CENTER_BONUS = 6;

#endif // POT_CONFIG_H
