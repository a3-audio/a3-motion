#ifndef LED_BUDGET_H
#define LED_BUDGET_H

#include <stddef.h>
#include <stdint.h>

// How much current the key LEDs may draw together, and how to stay under it
// (a3-motion-ui#21). No Arduino in here, so it is tested on the host
// (`pio test -e native`).
//
// Colours are Adafruit_NeoPixel's packed 0x00RRGGBB.

// The WS2812B datasheet's figures: about 20 mA per colour at full
// brightness, and about 1 mA for the chip itself whatever it shows. An
// estimate, and a pessimistic one at 5 V -- which is the side a fuse should
// err on.
constexpr uint32_t LED_MA_PER_CHANNEL_AT_FULL = 20;
constexpr uint32_t LED_MA_IDLE = 1;

inline uint32_t led_channel_sum(uint32_t color) {
    return ((color >> 16) & 0xFF) + ((color >> 8) & 0xFF) + (color & 0xFF);
}

// A dimming factor for every pixel together: 255 leaves the colours as they
// are, which is the answer whenever the estimate fits the budget -- the fuse
// is for an overload, not for everyday light.
inline uint8_t led_budget_scale(const uint32_t *colors, size_t count, uint32_t budget_ma) {
    uint32_t const idle = static_cast<uint32_t>(count) * LED_MA_IDLE;
    if (budget_ma <= idle) return 0;

    uint64_t channels = 0;
    for (size_t i = 0; i < count; ++i) channels += led_channel_sum(colors[i]);

    // Both sides in mA x 255, so no fraction is lost before the comparison.
    uint64_t const drawn = channels * LED_MA_PER_CHANNEL_AT_FULL;
    uint64_t const allowed = static_cast<uint64_t>(budget_ma - idle) * 255u;
    if (drawn <= allowed) return 255;

    return static_cast<uint8_t>(allowed * 255u / drawn);
}

// One colour, each channel scaled by `scale`/255.
inline uint32_t led_scaled(uint32_t color, uint8_t scale) {
    uint32_t const r = ((color >> 16) & 0xFF) * scale / 255u;
    uint32_t const g = ((color >> 8) & 0xFF) * scale / 255u;
    uint32_t const b = (color & 0xFF) * scale / 255u;
    return (r << 16) | (g << 8) | b;
}

#endif
