#ifndef A3_SPECIAL_H
#define A3_SPECIAL_H

#include <Adafruit_NeoPixel.h>

#include "multiplexer_map.h"

int readMuxDigital(const ButtonConfig &button);

void runA3Special(Adafruit_NeoPixel &strip, const ButtonConfig &button00, const ButtonConfig &button09);

#endif // A3_SPECIAL_H