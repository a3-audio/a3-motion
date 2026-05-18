#include "mux_io.h"
#include "config.h"

void selectMuxChannel(uint8_t channel) {
    digitalWrite(SELECT_PINS[0], (channel >> 0) & 1);
    digitalWrite(SELECT_PINS[1], (channel >> 1) & 1);
    digitalWrite(SELECT_PINS[2], (channel >> 2) & 1);
    delayMicroseconds(MUX_SETTLE_DELAY_US);
}

int readMuxDigital(const ButtonConfig &button) {
    selectMuxChannel(button.muxIndex);
    return digitalRead(button.gpio);
}
