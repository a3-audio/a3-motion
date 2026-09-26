#include "led_output.h"

#include "config.h"
#include "led_budget.h"

void show_within_budget(Adafruit_NeoPixel &strip) {
    uint32_t intended[NUMPIXELS];
    for (uint8_t i = 0; i < NUMPIXELS; i++) {
        intended[i] = strip.getPixelColor(i);
    }

    uint8_t const scale = led_budget_scale(intended, NUMPIXELS, LED_BUDGET_MA);
    if (scale == 255) {
        strip.show();
        return;
    }

    for (uint8_t i = 0; i < NUMPIXELS; i++) {
        strip.setPixelColor(i, led_scaled(intended[i], scale));
    }
    strip.show();
    for (uint8_t i = 0; i < NUMPIXELS; i++) {
        strip.setPixelColor(i, intended[i]);
    }
}
