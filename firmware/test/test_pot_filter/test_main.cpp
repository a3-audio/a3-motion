#include <unity.h>

#include "pot_filter.h"

// The panel's pots lagged behind the hand (2026-10-16). The filter only moved
// when the host asked for the pots, and the host asks every fifth poll --
// about every 25 ms -- so an EMA of a quarter per step needed ~8 asks (~200 ms)
// for 90 % of a move and ~23 (~600 ms) to land. The filter has to run on the
// firmware's own input scan, however often or rarely anyone reads it.

// The input scan runs every INPUT_SCAN_INTERVAL_US = 450 us; 100 scans are
// 45 ms, which a hand does not read as a delay.
static constexpr int scansForAFullMove = 100;

static uint16_t potRaw[POT_MUX_COUNT];

static uint16_t readPot(uint8_t index) { return potRaw[index]; }

static void setAll(uint16_t raw) {
    for (uint8_t i = 0; i < POT_MUX_COUNT; ++i) potRaw[i] = raw;
}

static void settle(PotBank &bank) {
    for (int i = 0; i < 10 * scansForAFullMove; ++i) bank.scan(readPot);
}

void setUp(void) {}
void tearDown(void) {}

void test_a_full_move_reaches_the_output_within_the_scans_alone(void) {
    PotBank bank;
    setAll(POT_CLIP_MIN);
    bank.prime(readPot);

    setAll(POT_CLIP_MAX);
    for (int i = 0; i < scansForAFullMove; ++i) bank.scan(readPot);

    uint16_t vals[POT_MUX_COUNT];
    bank.read(vals, POT_MUX_COUNT);

    // Where the filter comes to rest: a bank fed a hundred readings of every
    // pot. A few counts short of the end, which is the integer EMA's affair
    // and not this test's.
    PotBank reference;
    setAll(POT_CLIP_MIN);
    reference.prime(readPot);
    setAll(POT_CLIP_MAX);
    for (int n = 0; n < scansForAFullMove; ++n) reference.prime(readPot);
    uint16_t landed[POT_MUX_COUNT];
    reference.read(landed, POT_MUX_COUNT);

    TEST_ASSERT_EQUAL_UINT16_ARRAY(landed, vals, POT_MUX_COUNT);
}

void test_reading_does_not_move_the_filter(void) {
    PotBank bank;
    setAll(POT_CLIP_MIN);
    bank.prime(readPot);

    setAll(POT_CLIP_MAX);
    uint16_t before[POT_MUX_COUNT];
    uint16_t after[POT_MUX_COUNT];
    bank.read(before, POT_MUX_COUNT);
    for (int i = 0; i < scansForAFullMove; ++i) bank.read(after, POT_MUX_COUNT);

    TEST_ASSERT_EQUAL_UINT16_ARRAY(before, after, POT_MUX_COUNT);
}

void test_primed_pots_report_where_they_stand(void) {
    PotBank bank;
    setAll(2000);
    bank.prime(readPot);

    uint16_t vals[POT_MUX_COUNT];
    bank.read(vals, POT_MUX_COUNT);
    for (uint8_t i = 0; i < POT_MUX_COUNT; ++i)
        TEST_ASSERT_EQUAL_UINT16(pot_map_clipped_to_output(2000), vals[i]);
}

void test_jitter_inside_the_deadband_stays_put(void) {
    // Sampled forty times as often as before, the deadband still has to hold
    // a pot at rest: the UI reads every change as a hand.
    PotBank bank;
    setAll(3500);
    bank.prime(readPot);
    settle(bank);

    uint16_t rest[POT_MUX_COUNT];
    bank.read(rest, POT_MUX_COUNT);

    for (int i = 0; i < 10 * scansForAFullMove; ++i) {
        setAll((i & 1) ? 3500 + POT_DEADBAND - 1 : 3500 - (POT_DEADBAND - 1));
        bank.scan(readPot);
    }

    uint16_t vals[POT_MUX_COUNT];
    bank.read(vals, POT_MUX_COUNT);
    TEST_ASSERT_EQUAL_UINT16_ARRAY(rest, vals, POT_MUX_COUNT);
}

int main(int, char **) {
    UNITY_BEGIN();
    RUN_TEST(test_a_full_move_reaches_the_output_within_the_scans_alone);
    RUN_TEST(test_reading_does_not_move_the_filter);
    RUN_TEST(test_primed_pots_report_where_they_stand);
    RUN_TEST(test_jitter_inside_the_deadband_stays_put);
    return UNITY_END();
}
