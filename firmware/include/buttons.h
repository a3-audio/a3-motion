#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>
#include "config.h"

// Initialise all matrix button debouncers.
// Call once in setup() AFTER all pinMode calls.
void buttons_init();

// Poll every matrix button through the MUX, update debouncers,
// and latch transition flags. Must be called on every loop iteration.
void buttons_update();

// Copy the 2-bit debounced state for every button into states[0..count-1],
// then reset all transition flags (but NOT the current debounced state).
//
// State encoding:
//   bit0 = current debounced level  (0 = pressed/LOW, 1 = released/HIGH)
//   bit1 = round-trip seen          (both a press AND a release detected since last call)
//
// Values:
//   0  steady pressed   - button was LOW the whole time
//   1  steady released  - button was HIGH (or just went HIGH, no return)
//   2  brief release    - button was LOW, briefly HIGH, back to LOW
//   3  CLICK            - button was HIGH, went LOW (press), back to HIGH (release)
//
// Button order matches MATRIX_BUTTONS[] in multiplexer_map.h.
void buttons_readAndClear(uint8_t *states, uint8_t count);

#endif // BUTTONS_H
