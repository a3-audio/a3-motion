#include <unity.h>

#include "led_budget.h"

// The key LEDs hang on the USB 5 V (a3-motion-ui#21). 44 WS2812B at full
// white would draw about 2.6 A from a port that is good for 0.5 A. The
// firmware dims all of them together only when the estimate goes over the
// budget, so the colours the app chose are left alone the rest of the time.

void setUp(void) {}
void tearDown(void) {}

static const uint32_t off = 0x000000;
static const uint32_t white = 0xFFFFFF;

void test_dark_keys_are_left_alone(void) {
    uint32_t colors[44];
    for (auto &c : colors) c = off;
    TEST_ASSERT_EQUAL_UINT8(255, led_budget_scale(colors, 44, 400));
}

void test_a_load_under_the_budget_is_left_alone(void) {
    // Four keys full white: about 4 x 60 mA plus the idle draw of all 44.
    uint32_t colors[44];
    for (auto &c : colors) c = off;
    for (int i = 0; i < 4; ++i) colors[i] = white;
    TEST_ASSERT_EQUAL_UINT8(255, led_budget_scale(colors, 44, 400));
}

void test_a_load_over_the_budget_is_dimmed_to_fit(void) {
    uint32_t colors[44];
    for (auto &c : colors) c = white;
    auto const scale = led_budget_scale(colors, 44, 400);
    TEST_ASSERT_LESS_THAN_UINT8(255, scale);
    TEST_ASSERT_GREATER_THAN_UINT8(0, scale);

    // What is left after dimming must fit: 60 mA per full-white LED at
    // scale 255, plus 1 mA each for the chip itself.
    uint32_t const dynamic = 44u * 60u * scale / 255u;
    TEST_ASSERT_TRUE(dynamic + 44u <= 400u);
}

void test_more_light_is_never_dimmed_less(void) {
    uint32_t colors[44];
    for (auto &c : colors) c = off;
    uint8_t last = 255;
    for (int lit = 0; lit <= 44; ++lit) {
        if (lit > 0) colors[lit - 1] = white;
        auto const scale = led_budget_scale(colors, 44, 400);
        TEST_ASSERT_TRUE(scale <= last);
        last = scale;
    }
}

void test_a_budget_below_the_idle_draw_turns_them_off(void) {
    uint32_t colors[44];
    for (auto &c : colors) c = white;
    TEST_ASSERT_EQUAL_UINT8(0, led_budget_scale(colors, 44, 10));
}

void test_scaling_a_colour_scales_each_channel(void) {
    TEST_ASSERT_EQUAL_HEX32(0xFF8000, led_scaled(0xFF8000, 255));
    TEST_ASSERT_EQUAL_HEX32(0x000000, led_scaled(0xFF8000, 0));
    TEST_ASSERT_EQUAL_HEX32(0x804000, led_scaled(0xFF8000, 128));
}

int main(int, char **) {
    UNITY_BEGIN();
    RUN_TEST(test_dark_keys_are_left_alone);
    RUN_TEST(test_a_load_under_the_budget_is_left_alone);
    RUN_TEST(test_a_load_over_the_budget_is_dimmed_to_fit);
    RUN_TEST(test_more_light_is_never_dimmed_less);
    RUN_TEST(test_a_budget_below_the_idle_draw_turns_them_off);
    RUN_TEST(test_scaling_a_colour_scales_each_channel);
    return UNITY_END();
}
