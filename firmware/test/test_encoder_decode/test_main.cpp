#include <unity.h>

#include "encoder_decode.h"

// The panel's encoders turned the wrong way on every page (device check,
// 2026-10-16): a clockwise detent lowered the value. On this board a
// clockwise turn takes B first, so the states run 00 -> 01 -> 11 -> 10 -> 00
// (state = A<<1 | B) and every one of those steps must count up.

void setUp(void) {}
void tearDown(void) {}

void test_a_clockwise_detent_counts_up(void) {
    uint8_t const cycle[] = { 0b00, 0b01, 0b11, 0b10, 0b00 };
    int sum = 0;
    for (int i = 0; i < 4; ++i)
        sum += encoder_step(cycle[i], cycle[i + 1]);
    TEST_ASSERT_EQUAL_INT(4, sum);
}

void test_a_counter_clockwise_detent_counts_down(void) {
    uint8_t const cycle[] = { 0b00, 0b10, 0b11, 0b01, 0b00 };
    int sum = 0;
    for (int i = 0; i < 4; ++i)
        sum += encoder_step(cycle[i], cycle[i + 1]);
    TEST_ASSERT_EQUAL_INT(-4, sum);
}

void test_no_change_and_a_two_bit_jump_count_nothing(void) {
    TEST_ASSERT_EQUAL_INT(0, encoder_step(0b01, 0b01));
    TEST_ASSERT_EQUAL_INT(0, encoder_step(0b00, 0b11));
    TEST_ASSERT_EQUAL_INT(0, encoder_step(0b01, 0b10));
}

int main(int, char **) {
    UNITY_BEGIN();
    RUN_TEST(test_a_clockwise_detent_counts_up);
    RUN_TEST(test_a_counter_clockwise_detent_counts_down);
    RUN_TEST(test_no_change_and_a_two_bit_jump_count_nothing);
    return UNITY_END();
}
