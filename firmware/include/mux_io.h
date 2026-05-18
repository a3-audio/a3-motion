#ifndef MUX_IO_H
#define MUX_IO_H

#include <Arduino.h>
#include "multiplexer_map.h"

// Set the active MUX channel (0-7) on all select pins and wait for the
// signal to settle.
void selectMuxChannel(uint8_t channel);

// Select the channel indicated by button.muxIndex, then return the
// digitalRead result of button.gpio (LOW = pressed with INPUT_PULLUP).
int readMuxDigital(const ButtonConfig &button);

#endif // MUX_IO_H
