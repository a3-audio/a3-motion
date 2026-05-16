#ifndef A3_MOTION_FIRMWARE_CONFIG_H
#define A3_MOTION_FIRMWARE_CONFIG_H

#include <Arduino.h>

#include "multiplexer_map.h"

constexpr uint8_t LED_PIN = LED_DATA_PIN;
constexpr uint8_t NUMPIXELS = LED_COUNT;

constexpr uint8_t BUTTON_MUX_COUNT = 5;
constexpr uint8_t ENCODER_COUNT = 8;
constexpr uint8_t AUX_MUX_COUNT = 4;
constexpr uint8_t POT_MUX_COUNT = 4;
constexpr uint8_t MUX_CHANNEL_COUNT = 8;
constexpr uint8_t SELECT_PIN_COUNT = 3;
constexpr uint8_t EXTRA_BUTTON_COUNT = 4;

constexpr unsigned long DEBUG_PRINT_INTERVAL_MS = 2000;
constexpr uint8_t BUTTON_RELEASE_DELAY_MS = 5;
constexpr uint8_t MAIN_LOOP_DELAY_MS = 20;
constexpr uint8_t MUX_SETTLE_DELAY_US = 20;
constexpr int POT_ACTIVE_THRESHOLD = 100;

struct LedMapEntry {
  int8_t mux;
  int8_t ch;
};

constexpr LedMapEntry LED_MAP[NUMPIXELS] = {
  { -1, -1 }, { -1, -1 }, { 4, 4 }, { 4, 6 }, { 4, 7 }, { 4, 5 }, { 4, 3 }, { 4, 0 },
  { 4, 1 }, { 4, 2 }, { 3, 4 }, { 3, 6 }, { 3, 7 }, { 3, 5 }, { 3, 3 }, { 3, 0 },
  { 3, 1 }, { 3, 2 }, { 2, 4 }, { 2, 6 }, { 2, 7 }, { 2, 5 }, { 2, 3 }, { 2, 0 },
  { 2, 1 }, { 2, 2 }, { 1, 4 }, { 1, 6 }, { 1, 7 }, { 1, 5 }, { 1, 3 }, { 1, 0 },
  { 1, 1 }, { 1, 2 }, { 0, 4 }, { 0, 6 }, { 0, 7 }, { 0, 5 }, { 0, 3 }, { 0, 0 },
  { 0, 1 }, { 0, 2 }, { -1, -1 }, { -1, -1 }
};

constexpr uint8_t MUX_BUTTON_OUT[BUTTON_MUX_COUNT] = {
  MUX_BUTTON_0_OUT,
  MUX_BUTTON_1_OUT,
  MUX_BUTTON_2_OUT,
  MUX_BUTTON_3_OUT,
  MUX_BUTTON_4_OUT
};

constexpr uint8_t ENCODER_A_PINS[ENCODER_COUNT] = {
  ENCODER_1_PIN, ENCODER_2_PIN, ENCODER_3_PIN, ENCODER_4_PIN,
  ENCODER_5_PIN, ENCODER_6_PIN, ENCODER_7_PIN, ENCODER_8_PIN
};

constexpr uint8_t AUX_MUX_OUT[AUX_MUX_COUNT] = {
  MUX_OUT_A,
  MUX_OUT_B,
  MUX_OUT_C,
  MUX_OUT_D
};

struct EncoderMuxInput {
  uint8_t bMux;
  uint8_t bChannel;
  uint8_t swMux;
  uint8_t swChannel;
};

constexpr EncoderMuxInput ENCODER_MUX_INPUTS[ENCODER_COUNT] = {
  { ENCODER_1_B_MUX_INDEX, ENCODER_1_B_CHANNEL, ENCODER_1_SW_MUX_INDEX, ENCODER_1_SW_CHANNEL },
  { ENCODER_2_B_MUX_INDEX, ENCODER_2_B_CHANNEL, ENCODER_2_SW_MUX_INDEX, ENCODER_2_SW_CHANNEL },
  { ENCODER_3_B_MUX_INDEX, ENCODER_3_B_CHANNEL, ENCODER_3_SW_MUX_INDEX, ENCODER_3_SW_CHANNEL },
  { ENCODER_4_B_MUX_INDEX, ENCODER_4_B_CHANNEL, ENCODER_4_SW_MUX_INDEX, ENCODER_4_SW_CHANNEL },
  { ENCODER_5_B_MUX_INDEX, ENCODER_5_B_CHANNEL, ENCODER_5_SW_MUX_INDEX, ENCODER_5_SW_CHANNEL },
  { ENCODER_6_B_MUX_INDEX, ENCODER_6_B_CHANNEL, ENCODER_6_SW_MUX_INDEX, ENCODER_6_SW_CHANNEL },
  { ENCODER_7_B_MUX_INDEX, ENCODER_7_B_CHANNEL, ENCODER_7_SW_MUX_INDEX, ENCODER_7_SW_CHANNEL },
  { ENCODER_8_B_MUX_INDEX, ENCODER_8_B_CHANNEL, ENCODER_8_SW_MUX_INDEX, ENCODER_8_SW_CHANNEL }
};

struct ExtraButtonMap {
  uint8_t mux;
  uint8_t channel;
};

constexpr ExtraButtonMap EXTRA_BUTTONS[EXTRA_BUTTON_COUNT] = {
  { EXTRA_BUTTON_00_MUX_INDEX, EXTRA_BUTTON_00_CHANNEL },
  { EXTRA_BUTTON_09_MUX_INDEX, EXTRA_BUTTON_09_CHANNEL },
  { EXTRA_BUTTON_10_MUX_INDEX, EXTRA_BUTTON_10_CHANNEL },
  { EXTRA_BUTTON_19_MUX_INDEX, EXTRA_BUTTON_19_CHANNEL }
};

constexpr uint8_t POT_MUX_OUT[POT_MUX_COUNT] = {
  POT_1_PIN, POT_2_PIN, POT_3_PIN, POT_4_PIN
};

constexpr uint8_t SELECT_PINS[SELECT_PIN_COUNT] = {
  MUX_S0,
  MUX_S1,
  MUX_S2
};

#endif
