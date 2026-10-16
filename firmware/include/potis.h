#ifndef POTIS_H
#define POTIS_H

#include <Arduino.h>
#include "config.h"

// All potis are wired directly to ADC pins (no MUX).

// Read every pot once, so the first answer is where they stand.
void potis_init();

// One input scan: sample the next pot into its filter. The filter runs here,
// on the scan's cadence -- not when the host asks (see pot_filter.h).
void potis_update();

// The filtered values into vals[0..count-1], 12-bit: 0..4095 (POT_OUTPUT_MAX). Samples nothing.
void potis_read(uint16_t *vals, uint8_t count);

#endif // POTIS_H
