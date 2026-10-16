#ifndef ENCODER_DECODE_H
#define ENCODER_DECODE_H

#include <stdint.h>

// Quadrature decode table. Index = (prevState << 2) | newState, state bits:
// bit1 = A, bit0 = B. Positive = clockwise, as encoder.h promises. On this
// board a clockwise turn takes B first (00 -> 01 -> 11 -> 10); the table ran
// the other way until 2026-10-16 and every encoder counted down clockwise.
static const int8_t ENCODER_QEM[16] = {
     0,  1, -1,  0,
    -1,  0,  0,  1,
     1,  0,  0, -1,
     0, -1,  1,  0
};

static inline int8_t encoder_step(uint8_t prevState, uint8_t newState) {
    return ENCODER_QEM[((prevState & 3u) << 2) | (newState & 3u)];
}

#endif // ENCODER_DECODE_H
