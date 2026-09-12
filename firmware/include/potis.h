#ifndef POTIS_H
#define POTIS_H

#include <Arduino.h>
#include "config.h"

// Read all poti ADC values into vals[0..count-1].
// Values are 12-bit: 0..4095 (ESP32-S3 ADC resolution), pre-filtered on MCU.
// All potis are wired directly to ADC pins (no MUX).
void potis_read(uint16_t *vals, uint8_t count);

#endif // POTIS_H
