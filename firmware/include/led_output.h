#ifndef LED_OUTPUT_H
#define LED_OUTPUT_H

#include <Adafruit_NeoPixel.h>

// strip.show(), but never over LED_BUDGET_MA (a3-motion-ui#21). Everything
// that lights the keys goes through here.
//
// Over budget, all pixels are dimmed by one factor for this show only: the
// strip's buffer keeps the colours that were asked for, so the next partial
// update (one set_led) starts from them instead of from an already dimmed
// picture -- dimming in place would compound with every update.
void show_within_budget(Adafruit_NeoPixel &strip);

#endif
