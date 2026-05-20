#ifndef A3_MOTION_FIRMWARE_CONFIG_H
#define A3_MOTION_FIRMWARE_CONFIG_H

#include <Arduino.h>

constexpr uint8_t LED_PIN = 14;
constexpr uint8_t NUMPIXELS = 44;

constexpr uint8_t BUTTON_MUX_COUNT = 5;
constexpr uint8_t MUX_CHANNEL_COUNT = 8;
constexpr uint8_t EXTRA_BUTTON_COUNT = 4;
constexpr uint8_t MATRIX_BUTTON_COUNT = (BUTTON_MUX_COUNT * MUX_CHANNEL_COUNT) + EXTRA_BUTTON_COUNT;
constexpr uint8_t ENCODER_COUNT = 8;
constexpr uint8_t POT_MUX_COUNT = 4;
constexpr uint8_t SELECT_PIN_COUNT = 3;
constexpr uint8_t LED_MATRIX_ROWS = 6;
constexpr uint8_t LED_MATRIX_COLS = 10;
constexpr uint8_t LED_MATRIX_A3_START_ROW = 1;
constexpr uint8_t LED_MATRIX_A3_START_COL = 1;
constexpr uint8_t LED_MATRIX_A3_ROW_COUNT = 5;
constexpr uint8_t LED_MATRIX_A3_COL_COUNT = 9;

constexpr uint8_t BUTTON_RELEASE_DELAY_MS = 5;
constexpr uint8_t MAIN_LOOP_DELAY_MS = 1;
constexpr uint8_t MUX_SETTLE_DELAY_US = 20;
// Number of quadrature transitions per mechanical encoder detent.
// Most common mechanical encoders are 4; set to 2 if movement feels under-reported.
constexpr uint8_t ENCODER_COUNTS_PER_DETENT = 4;
constexpr uint8_t POT_OVERSAMPLE_COUNT = 2;
constexpr uint8_t POT_EMA_SHIFT = 1;      // alpha = 1 / 2^shift
constexpr uint8_t POT_DEADBAND = 2;       // report step threshold after smoothing

constexpr uint8_t MUX_S0 = 39;
constexpr uint8_t MUX_S1 = 38;
constexpr uint8_t MUX_S2 = 48;

constexpr uint8_t MUX_0_GPIO = 41;//MAA01 Button20 bis 51
constexpr uint8_t MUX_1_GPIO = 42;//MAA23 Button22 bis 53
constexpr uint8_t MUX_2_GPIO = 40;//MAA45 Button24 bis 55
constexpr uint8_t MUX_3_GPIO = 21;//MAA67 Button26 bis 57
constexpr uint8_t MUX_4_GPIO = 47;//MAA89 Button28 bis 59
constexpr uint8_t MUX_5_GPIO = 2;//MA  Button00,01 ENC1 und ENC2
constexpr uint8_t MUX_6_GPIO = 15;//MB ENC5 
constexpr uint8_t MUX_7_GPIO = 8;//MC ENC3,ENC6,ENC7 
constexpr uint8_t MUX_8_GPIO = 9;//MD Button09,19 und ENC4,ENC8

constexpr uint8_t ENCODER_1A_PIN = 6;
constexpr uint8_t ENCODER_2A_PIN = 7;
constexpr uint8_t ENCODER_3A_PIN = 11;
constexpr uint8_t ENCODER_4A_PIN = 12;
constexpr uint8_t ENCODER_5A_PIN = 5;
constexpr uint8_t ENCODER_6A_PIN = 18;
constexpr uint8_t ENCODER_7A_PIN = 10;
constexpr uint8_t ENCODER_8A_PIN = 13;

constexpr uint8_t POT_1_PIN = 4;
constexpr uint8_t POT_2_PIN = 16;
constexpr uint8_t POT_3_PIN = 17;
constexpr uint8_t POT_4_PIN = 3;

constexpr uint8_t SELECT_PINS[SELECT_PIN_COUNT] = {
  MUX_S0,
  MUX_S1,
  MUX_S2
};

constexpr uint8_t MUX_PINS[9] = {
  MUX_0_GPIO,
  MUX_1_GPIO,
  MUX_2_GPIO,
  MUX_3_GPIO,
  MUX_4_GPIO,
  MUX_5_GPIO,
  MUX_6_GPIO,
  MUX_7_GPIO,
  MUX_8_GPIO
};

constexpr uint8_t ENCODER_A_PINS[ENCODER_COUNT] = {
  ENCODER_1A_PIN, ENCODER_2A_PIN, ENCODER_3A_PIN, ENCODER_4A_PIN,
  ENCODER_5A_PIN, ENCODER_6A_PIN, ENCODER_7A_PIN, ENCODER_8A_PIN
};

constexpr uint8_t POT_PINS[POT_MUX_COUNT] = {
  POT_1_PIN, POT_2_PIN, POT_3_PIN, POT_4_PIN
};

// Physical daisy-chain order on the board.
// First addressable LED is 09, last is 00.
constexpr uint8_t LED_PHYSICAL_ORDER[NUMPIXELS] = {
  9, 19, 29, 39, 49, 59,
  58, 48, 38, 28,
  27, 37, 47, 57,
  56, 46, 36, 26,
  25, 35, 45, 55,
  54, 44, 34, 24,
  23, 33, 43, 53,
  52, 42, 32, 22,
  21, 31, 41, 51,
  50, 40, 30, 20, 10, 0
};

constexpr int8_t LED_MATRIX[LED_MATRIX_ROWS][LED_MATRIX_COLS] = {
  { 43, -1, -1, -1, -1, -1, -1, -1, -1, 0 },
  { 42, -1, -1, -1, -1, -1, -1, -1, -1, 1 },
  { 41, 34, 33, 26, 25, 18, 17, 10, 9, 2 },
  { 40, 35, 32, 27, 24, 19, 16, 11, 8, 3 },
  { 39, 36, 31, 28, 23, 20, 15, 12, 7, 4 },
  { 38, 37, 30, 29, 22, 21, 14, 13, 6, 5 }
};

#endif
