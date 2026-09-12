#include "encoder.h"
#include "mux_io.h"
#include "multiplexer_map.h"
#include <Bounce2.h>

static int16_t encoderDeltas[ENCODER_COUNT];
static int16_t encoderRemainder[ENCODER_COUNT];
static uint8_t lastEncoderState[ENCODER_COUNT]; // (A<<1)|B
static int8_t lastMoveDir[ENCODER_COUNT];

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
        encoderRemainder[i] = 0;
        lastMoveDir[i] = 0;
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
        uint8_t prevState = lastEncoderState[i];
        int8_t delta = QEM[(prevState << 2) | newState];

        // Recover occasional skipped intermediate quadrature state caused by
        // multiplexed sampling (two-bit jump: 00<->11 or 01<->10).
        if (delta == 0 && newState != prevState) {
            bool oppositeJump = ((prevState ^ newState) == 0x03u);
            if (oppositeJump && lastMoveDir[i] != 0) {
                delta = (int8_t)(2 * lastMoveDir[i]);
            }
        }

        if (delta > 0) {
            lastMoveDir[i] = 1;
        } else if (delta < 0) {
            lastMoveDir[i] = -1;
        }

        encoderDeltas[i] += delta;
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
        int16_t raw = encoderDeltas[i] + encoderRemainder[i];
        int16_t step = raw / ENCODER_COUNTS_PER_DETENT;

        // Fast turns on a multiplexed B channel can occasionally miss one of
        // the 4 transitions belonging to a mechanical detent. If we saw at
        // least (N-1) transitions in one direction, count it as one detent.
        if (step == 0) {
            int16_t absRaw = (raw < 0) ? (int16_t)(-raw) : raw;
            if (absRaw >= (int16_t)(ENCODER_COUNTS_PER_DETENT - 1) && absRaw > 0) {
                step = (raw > 0) ? 1 : -1;
            }
        }

        encoderRemainder[i] = raw - (step * ENCODER_COUNTS_PER_DETENT);

        deltas[i]        = step;
        encoderDeltas[i] = 0;

        uint8_t bit0 = switchBouncers[i].read() ? 1 : 0;
        uint8_t bit1 = (switchSawRise[i] && switchSawFall[i]) ? 1 : 0;
        switchStates[i]  = (bit1 << 1) | bit0;

        switchSawRise[i] = false;
        switchSawFall[i] = false;
    }
}
