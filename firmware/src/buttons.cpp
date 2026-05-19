#include "buttons.h"
#include "mux_io.h"
#include "multiplexer_map.h"
#include <Bounce2.h>
#include <Arduino.h>

// One Bounce2 debouncer per button.
// Multiple buttons share the same GPIO but use different MUX channels.
// We call selectMuxChannel() immediately before each update() so that
// Bounce2's internal digitalRead() reads the correct button.
static Bounce bouncers[MATRIX_BUTTON_COUNT];
static bool   sawRise[MATRIX_BUTTON_COUNT]; // LOW -> HIGH since last readAndClear
static bool   sawFall[MATRIX_BUTTON_COUNT]; // HIGH -> LOW since last readAndClear

void buttons_init() {
    for (uint8_t i = 0; i < MATRIX_BUTTON_COUNT; i++) {
        // Select the correct MUX channel before attach() so that the initial
        // state captured by Bounce2 is accurate for this specific button.
        selectMuxChannel(MATRIX_BUTTONS[i].muxIndex);
        bouncers[i].attach(MATRIX_BUTTONS[i].gpio, INPUT_PULLUP);
        bouncers[i].interval(5); // 5 ms debounce window
        sawRise[i] = false;
        sawFall[i] = false;
    }
}

void buttons_update() {
    for (uint8_t i = 0; i < MATRIX_BUTTON_COUNT; i++) {
        selectMuxChannel(MATRIX_BUTTONS[i].muxIndex);
        bouncers[i].update();
        if (bouncers[i].rose()) sawRise[i] = true;
        if (bouncers[i].fell()) sawFall[i] = true;
    }
}

void buttons_readAndClear(uint8_t *states, uint8_t count) {
    for (uint8_t i = 0; i < count && i < MATRIX_BUTTON_COUNT; i++) {
        uint8_t bit0 = bouncers[i].read() ? 1 : 0;
        uint8_t bit1 = (sawRise[i] && sawFall[i]) ? 1 : 0;
        states[i]    = (bit1 << 1) | bit0;
        sawRise[i]   = false;
        sawFall[i]   = false;
    }
}
