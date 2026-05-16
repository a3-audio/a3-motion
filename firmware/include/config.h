#ifndef A3_MOTION_FIRMWARE_CONFIG_H
#define A3_MOTION_FIRMWARE_CONFIG_H

#include <Arduino.h>

#include "multiplexer_map.h"

constexpr uint8_t LED_PIN = LED_DATA_PIN;
constexpr uint8_t NUMPIXELS = LED_COUNT;

constexpr uint8_t BUTTON_MUX_COUNT = 5;
constexpr uint8_t MUX_CHANNEL_COUNT = 8;
constexpr uint8_t MATRIX_BUTTON_COUNT = BUTTON_MUX_COUNT * MUX_CHANNEL_COUNT;
constexpr uint8_t ENCODER_COUNT = 8;
constexpr uint8_t POT_MUX_COUNT = 4;
constexpr uint8_t SELECT_PIN_COUNT = 3;
constexpr uint8_t EXTRA_BUTTON_COUNT = 4;
constexpr uint8_t DIRECT_INPUT = 0xFF;
constexpr uint8_t UNASSIGNED_CHANNEL = 0xFF;

constexpr unsigned long DEBUG_PRINT_INTERVAL_MS = 2000;
constexpr uint8_t BUTTON_RELEASE_DELAY_MS = 5;
constexpr uint8_t MAIN_LOOP_DELAY_MS = 20;
constexpr uint8_t MUX_SETTLE_DELAY_US = 20;
constexpr int POT_ACTIVE_THRESHOLD = 100;

struct ButtonConfig {
  uint8_t muxIndex;
  uint8_t gpio;
};

struct EncoderConfig {
  uint8_t gpioA;
  ButtonConfig gpioB;
  ButtonConfig switchInput;
};

struct PotiConfig {
  uint8_t muxIndex;
  uint8_t gpio;
};

constexpr ButtonConfig LED_MAP[NUMPIXELS] = {
  { DIRECT_INPUT, UNASSIGNED_CHANNEL },
  { DIRECT_INPUT, UNASSIGNED_CHANNEL },
  { 4, MUX_BUTTON_4_OUT }, { 6, MUX_BUTTON_4_OUT }, { 7, MUX_BUTTON_4_OUT }, { 5, MUX_BUTTON_4_OUT },
  { 3, MUX_BUTTON_4_OUT }, { 0, MUX_BUTTON_4_OUT }, { 1, MUX_BUTTON_4_OUT }, { 2, MUX_BUTTON_4_OUT },
  { 4, MUX_BUTTON_3_OUT }, { 6, MUX_BUTTON_3_OUT }, { 7, MUX_BUTTON_3_OUT }, { 5, MUX_BUTTON_3_OUT },
  { 3, MUX_BUTTON_3_OUT }, { 0, MUX_BUTTON_3_OUT }, { 1, MUX_BUTTON_3_OUT }, { 2, MUX_BUTTON_3_OUT },
  { 4, MUX_BUTTON_2_OUT }, { 6, MUX_BUTTON_2_OUT }, { 7, MUX_BUTTON_2_OUT }, { 5, MUX_BUTTON_2_OUT },
  { 3, MUX_BUTTON_2_OUT }, { 0, MUX_BUTTON_2_OUT }, { 1, MUX_BUTTON_2_OUT }, { 2, MUX_BUTTON_2_OUT },
  { 4, MUX_BUTTON_1_OUT }, { 6, MUX_BUTTON_1_OUT }, { 7, MUX_BUTTON_1_OUT }, { 5, MUX_BUTTON_1_OUT },
  { 3, MUX_BUTTON_1_OUT }, { 0, MUX_BUTTON_1_OUT }, { 1, MUX_BUTTON_1_OUT }, { 2, MUX_BUTTON_1_OUT },
  { 4, MUX_BUTTON_0_OUT }, { 6, MUX_BUTTON_0_OUT }, { 7, MUX_BUTTON_0_OUT }, { 5, MUX_BUTTON_0_OUT },
  { 3, MUX_BUTTON_0_OUT }, { 0, MUX_BUTTON_0_OUT }, { 1, MUX_BUTTON_0_OUT }, { 2, MUX_BUTTON_0_OUT },
  { DIRECT_INPUT, UNASSIGNED_CHANNEL },
  { DIRECT_INPUT, UNASSIGNED_CHANNEL }
};

constexpr ButtonConfig MATRIX_BUTTONS[MATRIX_BUTTON_COUNT] = {
  { 0, MUX_BUTTON_0_OUT }, { 1, MUX_BUTTON_0_OUT }, { 2, MUX_BUTTON_0_OUT }, { 3, MUX_BUTTON_0_OUT },
  { 4, MUX_BUTTON_0_OUT }, { 5, MUX_BUTTON_0_OUT }, { 6, MUX_BUTTON_0_OUT }, { 7, MUX_BUTTON_0_OUT },
  { 0, MUX_BUTTON_1_OUT }, { 1, MUX_BUTTON_1_OUT }, { 2, MUX_BUTTON_1_OUT }, { 3, MUX_BUTTON_1_OUT },
  { 4, MUX_BUTTON_1_OUT }, { 5, MUX_BUTTON_1_OUT }, { 6, MUX_BUTTON_1_OUT }, { 7, MUX_BUTTON_1_OUT },
  { 0, MUX_BUTTON_2_OUT }, { 1, MUX_BUTTON_2_OUT }, { 2, MUX_BUTTON_2_OUT }, { 3, MUX_BUTTON_2_OUT },
  { 4, MUX_BUTTON_2_OUT }, { 5, MUX_BUTTON_2_OUT }, { 6, MUX_BUTTON_2_OUT }, { 7, MUX_BUTTON_2_OUT },
  { 0, MUX_BUTTON_3_OUT }, { 1, MUX_BUTTON_3_OUT }, { 2, MUX_BUTTON_3_OUT }, { 3, MUX_BUTTON_3_OUT },
  { 4, MUX_BUTTON_3_OUT }, { 5, MUX_BUTTON_3_OUT }, { 6, MUX_BUTTON_3_OUT }, { 7, MUX_BUTTON_3_OUT },
  { 0, MUX_BUTTON_4_OUT }, { 1, MUX_BUTTON_4_OUT }, { 2, MUX_BUTTON_4_OUT }, { 3, MUX_BUTTON_4_OUT },
  { 4, MUX_BUTTON_4_OUT }, { 5, MUX_BUTTON_4_OUT }, { 6, MUX_BUTTON_4_OUT }, { 7, MUX_BUTTON_4_OUT }
};

constexpr EncoderConfig ENCODERS[ENCODER_COUNT] = {
  { ENCODER_1_PIN, { ENCODER_1_B_CHANNEL, MUX_OUT_A }, { ENCODER_1_SW_CHANNEL, MUX_OUT_A } },
  { ENCODER_2_PIN, { ENCODER_2_B_CHANNEL, MUX_OUT_A }, { ENCODER_2_SW_CHANNEL, MUX_OUT_A } },
  { ENCODER_3_PIN, { ENCODER_3_B_CHANNEL, MUX_OUT_C }, { ENCODER_3_SW_CHANNEL, MUX_OUT_C } },
  { ENCODER_4_PIN, { ENCODER_4_B_CHANNEL, MUX_OUT_D }, { ENCODER_4_SW_CHANNEL, MUX_OUT_D } },
  { ENCODER_5_PIN, { ENCODER_5_B_CHANNEL, MUX_OUT_B }, { ENCODER_5_SW_CHANNEL, MUX_OUT_B } },
  { ENCODER_6_PIN, { ENCODER_6_B_CHANNEL, MUX_OUT_C }, { ENCODER_6_SW_CHANNEL, MUX_OUT_C } },
  { ENCODER_7_PIN, { ENCODER_7_B_CHANNEL, MUX_OUT_C }, { ENCODER_7_SW_CHANNEL, MUX_OUT_C } },
  { ENCODER_8_PIN, { ENCODER_8_B_CHANNEL, MUX_OUT_D }, { ENCODER_8_SW_CHANNEL, MUX_OUT_D } }
};

constexpr ButtonConfig EXTRA_BUTTONS[EXTRA_BUTTON_COUNT] = {
  { EXTRA_BUTTON_00_CHANNEL, MUX_OUT_A },
  { EXTRA_BUTTON_09_CHANNEL, MUX_OUT_D },
  { EXTRA_BUTTON_10_CHANNEL, MUX_OUT_A },
  { EXTRA_BUTTON_19_CHANNEL, MUX_OUT_D }
};

constexpr PotiConfig POTIS[POT_MUX_COUNT] = {
  { DIRECT_INPUT, POT_1_PIN },
  { DIRECT_INPUT, POT_2_PIN },
  { DIRECT_INPUT, POT_3_PIN },
  { DIRECT_INPUT, POT_4_PIN }
};

constexpr uint8_t SELECT_PINS[SELECT_PIN_COUNT] = {
  MUX_S0,
  MUX_S1,
  MUX_S2
};

static_assert(ENCODERS[0].gpioB.muxIndex == ENCODER_1_B_CHANNEL && ENCODERS[0].gpioB.gpio == MUX_OUT_A, "ENC1_B mapping mismatch");
static_assert(ENCODERS[0].switchInput.muxIndex == ENCODER_1_SW_CHANNEL && ENCODERS[0].switchInput.gpio == MUX_OUT_A, "ENC1_SW mapping mismatch");
static_assert(ENCODERS[1].gpioB.muxIndex == ENCODER_2_B_CHANNEL && ENCODERS[1].gpioB.gpio == MUX_OUT_A, "ENC2_B mapping mismatch");
static_assert(ENCODERS[1].switchInput.muxIndex == ENCODER_2_SW_CHANNEL && ENCODERS[1].switchInput.gpio == MUX_OUT_A, "ENC2_SW mapping mismatch");
static_assert(ENCODERS[2].gpioB.muxIndex == ENCODER_3_B_CHANNEL && ENCODERS[2].gpioB.gpio == MUX_OUT_C, "ENC3_B mapping mismatch");
static_assert(ENCODERS[2].switchInput.muxIndex == ENCODER_3_SW_CHANNEL && ENCODERS[2].switchInput.gpio == MUX_OUT_C, "ENC3_SW mapping mismatch");
static_assert(ENCODERS[3].gpioB.muxIndex == ENCODER_4_B_CHANNEL && ENCODERS[3].gpioB.gpio == MUX_OUT_D, "ENC4_B mapping mismatch");
static_assert(ENCODERS[3].switchInput.muxIndex == ENCODER_4_SW_CHANNEL && ENCODERS[3].switchInput.gpio == MUX_OUT_D, "ENC4_SW mapping mismatch");
static_assert(ENCODERS[4].gpioB.muxIndex == ENCODER_5_B_CHANNEL && ENCODERS[4].gpioB.gpio == MUX_OUT_B, "ENC5_B mapping mismatch");
static_assert(ENCODERS[4].switchInput.muxIndex == ENCODER_5_SW_CHANNEL && ENCODERS[4].switchInput.gpio == MUX_OUT_B, "ENC5_SW mapping mismatch");
static_assert(ENCODERS[5].gpioB.muxIndex == ENCODER_6_B_CHANNEL && ENCODERS[5].gpioB.gpio == MUX_OUT_C, "ENC6_B mapping mismatch");
static_assert(ENCODERS[5].switchInput.muxIndex == ENCODER_6_SW_CHANNEL && ENCODERS[5].switchInput.gpio == MUX_OUT_C, "ENC6_SW mapping mismatch");
static_assert(ENCODERS[6].gpioB.muxIndex == ENCODER_7_B_CHANNEL && ENCODERS[6].gpioB.gpio == MUX_OUT_C, "ENC7_B mapping mismatch");
static_assert(ENCODERS[6].switchInput.muxIndex == ENCODER_7_SW_CHANNEL && ENCODERS[6].switchInput.gpio == MUX_OUT_C, "ENC7_SW mapping mismatch");
static_assert(ENCODERS[7].gpioB.muxIndex == ENCODER_8_B_CHANNEL && ENCODERS[7].gpioB.gpio == MUX_OUT_D, "ENC8_B mapping mismatch");
static_assert(ENCODERS[7].switchInput.muxIndex == ENCODER_8_SW_CHANNEL && ENCODERS[7].switchInput.gpio == MUX_OUT_D, "ENC8_SW mapping mismatch");

#endif
