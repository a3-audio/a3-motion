#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>
#include "config.h"

// Initialise encoder tracking. Call once in setup() AFTER all pinMode calls.
void encoders_init();

// Poll all encoder A/B quadrature signals and accumulate signed deltas.
// Also updates the switch debouncers and latches transition flags.
// Must be called on every loop iteration.
void encoders_update();

// Copy accumulated rotation deltas and 2-bit switch states into the caller
// arrays, then reset all counters and transition flags.
//
// deltas[i]      - signed step count since last call (positive = CW)
// switchStates[i] - 2-bit debounced switch state:
//   bit0 = current state  (0 = pressed/LOW, 1 = released/HIGH)
//   bit1 = round-trip seen (both press and release detected since last call)
//   => 0 steady pressed | 1 steady released | 2 brief-release+re-press | 3 CLICK
void encoders_readAndClear(int16_t *deltas, uint8_t *switchStates, uint8_t count);

#endif // ENCODER_H
