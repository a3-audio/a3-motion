#ifndef A3_SPECIAL_H
#define A3_SPECIAL_H

#include <Adafruit_NeoPixel.h>
#include "mux_io.h"
#include "multiplexer_map.h"

void runA3Special(Adafruit_NeoPixel &strip, const ButtonConfig &button00, const ButtonConfig &button09);

#endif // A3_SPECIAL_H