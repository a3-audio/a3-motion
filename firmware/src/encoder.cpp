#include "encoder.h"
#include "mux_io.h"
#include "multiplexer_map.h"
#include <Bounce2.h>

static int16_t encoderDeltas[ENCODER_COUNT];
static uint8_t lastEncoderState[ENCODER_COUNT]; // (A<<1)|B

static Bounce  switchBouncers[ENCODER_COUNT];
static bool    switchSawRise[ENCODER_COUNT];
static bool    switchSawFall[ENCODER_COUNT];

// Quadrature decode table.
// Index = (prevState << 2) | newState, state bits: bit1=A, bit0=B.
static const int8_t QEM[16] = {
     0, -1,  1,  0,
     1,  0,  0, -1,
    -1,  0,  0,  1,
     0,  1, -1,  0
};

void encoders_init() {
    for (uint8_t i = 0; i < ENCODER_COUNT; i++) {
        encoderDeltas[i] = 0;
        switchSawRise[i] = false;
        switchSawFall[i] = false;

        // Capture initial A/B state for quadrature
        selectMuxChannel(ENCODERS[i].gpioB.muxIndex);
        int a = (digitalRead(ENCODERS[i].gpioA)      == HIGH) ? 1 : 0;
        int b = (digitalRead(ENCODERS[i].gpioB.gpio) == HIGH) ? 1 : 0;
        lastEncoderState[i] = (uint8_t)((a << 1) | b);

        // Attach switch bouncer with the correct MUX channel already active
        // so that the initial state read by attach() is accurate.
        selectMuxChannel(ENCODERS[i].switchInput.muxIndex);
        switchBouncers[i].attach(ENCODERS[i].switchInput.gpio, INPUT_PULLUP);
        switchBouncers[i].interval(5);
    }
}

void encoders_update() {
    for (uint8_t i = 0; i < ENCODER_COUNT; i++) {
        // --- Quadrature decode (A direct GPIO, B via MUX) ---
        selectMuxChannel(ENCODERS[i].gpioB.muxIndex);
        int a = (digitalRead(ENCODERS[i].gpioA)      == HIGH) ? 1 : 0;
        int b = (digitalRead(ENCODERS[i].gpioB.gpio) == HIGH) ? 1 : 0;
        uint8_t newState = (uint8_t)((a << 1) | b);
        encoderDeltas[i] += QEM[(lastEncoderState[i] << 2) | newState];
        lastEncoderState[i] = newState;

        // --- Switch debounce (switch is MUX-based) ---
        selectMuxChannel(ENCODERS[i].switchInput.muxIndex);
        switchBouncers[i].update();
        if (switchBouncers[i].rose()) switchSawRise[i] = true;
        if (switchBouncers[i].fell()) switchSawFall[i] = true;
    }
}

void encoders_readAndClear(int16_t *deltas, uint8_t *switchStates, uint8_t count) {
    for (uint8_t i = 0; i < count && i < ENCODER_COUNT; i++) {
        deltas[i]        = encoderDeltas[i];
        encoderDeltas[i] = 0;

        uint8_t bit0 = switchBouncers[i].read() ? 1 : 0;
        uint8_t bit1 = (switchSawRise[i] && switchSawFall[i]) ? 1 : 0;
        switchStates[i]  = (bit1 << 1) | bit0;

        switchSawRise[i] = false;
        switchSawFall[i] = false;
    }
}
