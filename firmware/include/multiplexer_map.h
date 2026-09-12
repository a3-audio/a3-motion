#ifndef MULTIPLEXER_MAP_H
#define MULTIPLEXER_MAP_H

#include "config.h"

constexpr uint8_t DIRECT_INPUT = 0xFF;
constexpr uint8_t UNASSIGNED_CHANNEL = 0xFF;

constexpr uint8_t BUTTON_10_CHANNEL = 1;
constexpr uint8_t BUTTON_00_CHANNEL = 2;
constexpr uint8_t BUTTON_19_CHANNEL = 5;
constexpr uint8_t BUTTON_09_CHANNEL = 7;

constexpr uint8_t ENCODER_1_B_CHANNEL = 5;
constexpr uint8_t ENCODER_1_SW_CHANNEL = 0;
constexpr uint8_t ENCODER_2_B_CHANNEL = 6;
constexpr uint8_t ENCODER_2_SW_CHANNEL = 7;
constexpr uint8_t ENCODER_3_B_CHANNEL = 3;
constexpr uint8_t ENCODER_3_SW_CHANNEL = 0;
constexpr uint8_t ENCODER_4_B_CHANNEL = 3;
constexpr uint8_t ENCODER_4_SW_CHANNEL = 0;
constexpr uint8_t ENCODER_5_B_CHANNEL = 7;
constexpr uint8_t ENCODER_5_SW_CHANNEL = 5;
constexpr uint8_t ENCODER_6_B_CHANNEL = 1;
constexpr uint8_t ENCODER_6_SW_CHANNEL = 2;
constexpr uint8_t ENCODER_7_B_CHANNEL = 7;
constexpr uint8_t ENCODER_7_SW_CHANNEL = 5;
constexpr uint8_t ENCODER_8_B_CHANNEL = 4;
constexpr uint8_t ENCODER_8_SW_CHANNEL = 6;

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

// Button naming follows mux/channel order.
// The last four entries are the special buttons 00, 10, 09 and 19.

constexpr ButtonConfig BUTTON_40 = { 0, MUX_0_GPIO };
constexpr ButtonConfig BUTTON_30 = { 1, MUX_0_GPIO };
constexpr ButtonConfig BUTTON_20 = { 2, MUX_0_GPIO };
constexpr ButtonConfig BUTTON_50 = { 3, MUX_0_GPIO };
constexpr ButtonConfig BUTTON_21 = { 4, MUX_0_GPIO };
constexpr ButtonConfig BUTTON_51 = { 5, MUX_0_GPIO };
constexpr ButtonConfig BUTTON_31 = { 6, MUX_0_GPIO };
constexpr ButtonConfig BUTTON_41 = { 7, MUX_0_GPIO };

constexpr ButtonConfig BUTTON_42 = { 0, MUX_1_GPIO };
constexpr ButtonConfig BUTTON_32 = { 1, MUX_1_GPIO };
constexpr ButtonConfig BUTTON_22 = { 2, MUX_1_GPIO };
constexpr ButtonConfig BUTTON_52 = { 3, MUX_1_GPIO };
constexpr ButtonConfig BUTTON_23 = { 4, MUX_1_GPIO };
constexpr ButtonConfig BUTTON_53 = { 5, MUX_1_GPIO };
constexpr ButtonConfig BUTTON_33 = { 6, MUX_1_GPIO };
constexpr ButtonConfig BUTTON_43 = { 7, MUX_1_GPIO };

constexpr ButtonConfig BUTTON_44 = { 0, MUX_2_GPIO };
constexpr ButtonConfig BUTTON_34 = { 1, MUX_2_GPIO };
constexpr ButtonConfig BUTTON_24 = { 2, MUX_2_GPIO };
constexpr ButtonConfig BUTTON_54 = { 3, MUX_2_GPIO };
constexpr ButtonConfig BUTTON_25 = { 4, MUX_2_GPIO };
constexpr ButtonConfig BUTTON_55 = { 5, MUX_2_GPIO };
constexpr ButtonConfig BUTTON_35 = { 6, MUX_2_GPIO };
constexpr ButtonConfig BUTTON_45 = { 7, MUX_2_GPIO };

constexpr ButtonConfig BUTTON_46 = { 0, MUX_3_GPIO };
constexpr ButtonConfig BUTTON_36 = { 1, MUX_3_GPIO };
constexpr ButtonConfig BUTTON_26 = { 2, MUX_3_GPIO };
constexpr ButtonConfig BUTTON_56 = { 3, MUX_3_GPIO };
constexpr ButtonConfig BUTTON_27 = { 4, MUX_3_GPIO };
constexpr ButtonConfig BUTTON_57 = { 5, MUX_3_GPIO };
constexpr ButtonConfig BUTTON_37 = { 6, MUX_3_GPIO };
constexpr ButtonConfig BUTTON_47 = { 7, MUX_3_GPIO };

constexpr ButtonConfig BUTTON_48 = { 0, MUX_4_GPIO };
constexpr ButtonConfig BUTTON_38 = { 1, MUX_4_GPIO };
constexpr ButtonConfig BUTTON_28 = { 2, MUX_4_GPIO };
constexpr ButtonConfig BUTTON_58 = { 3, MUX_4_GPIO };
constexpr ButtonConfig BUTTON_29 = { 4, MUX_4_GPIO };
constexpr ButtonConfig BUTTON_59 = { 5, MUX_4_GPIO };
constexpr ButtonConfig BUTTON_39 = { 6, MUX_4_GPIO };
constexpr ButtonConfig BUTTON_49 = { 7, MUX_4_GPIO };

constexpr ButtonConfig BUTTON_00 = { BUTTON_00_CHANNEL, MUX_5_GPIO };
constexpr ButtonConfig BUTTON_10 = { BUTTON_10_CHANNEL, MUX_5_GPIO };
constexpr ButtonConfig BUTTON_09 = { BUTTON_09_CHANNEL, MUX_8_GPIO };
constexpr ButtonConfig BUTTON_19 = { BUTTON_19_CHANNEL, MUX_8_GPIO };

constexpr ButtonConfig MATRIX_BUTTONS[MATRIX_BUTTON_COUNT] = {
	BUTTON_40, BUTTON_30, BUTTON_20, BUTTON_50,
	BUTTON_21, BUTTON_51, BUTTON_31, BUTTON_41,
	BUTTON_42, BUTTON_32, BUTTON_22, BUTTON_52,
	BUTTON_23, BUTTON_53, BUTTON_33, BUTTON_43,
	BUTTON_44, BUTTON_34, BUTTON_24, BUTTON_54,
	BUTTON_25, BUTTON_55, BUTTON_35, BUTTON_45,
	BUTTON_46, BUTTON_36, BUTTON_26, BUTTON_56,
	BUTTON_27, BUTTON_57, BUTTON_37, BUTTON_47,
	BUTTON_48, BUTTON_38, BUTTON_28, BUTTON_58,
	BUTTON_29, BUTTON_59, BUTTON_39, BUTTON_49,
	BUTTON_00, BUTTON_10, BUTTON_09, BUTTON_19
};

// NeoPixel order on the board.
constexpr ButtonConfig LED_MAP[NUMPIXELS] = {
	BUTTON_09,
	BUTTON_19,
	BUTTON_29,
	BUTTON_39,
	BUTTON_49,
	BUTTON_59,
	BUTTON_58,
	BUTTON_48,
	BUTTON_38,
	BUTTON_28,
	BUTTON_27,
	BUTTON_37,
	BUTTON_47,
	BUTTON_57,
	BUTTON_56,
	BUTTON_46,
	BUTTON_36,
	BUTTON_26,
	BUTTON_25,
	BUTTON_35,
	BUTTON_45,
	BUTTON_55,
	BUTTON_54,
	BUTTON_44,
	BUTTON_34,
	BUTTON_24,
	BUTTON_23,
	BUTTON_33,
	BUTTON_43,
	BUTTON_53,
	BUTTON_52,
	BUTTON_42,
	BUTTON_32,
	BUTTON_22,
	BUTTON_21,
	BUTTON_31,
	BUTTON_41,
	BUTTON_51,
	BUTTON_50,
	BUTTON_40,
	BUTTON_30,
	BUTTON_20,
	BUTTON_10,
	BUTTON_00
};

constexpr EncoderConfig ENCODERS[ENCODER_COUNT] = {
	{ ENCODER_1A_PIN, { ENCODER_1_B_CHANNEL, MUX_5_GPIO }, { ENCODER_1_SW_CHANNEL, MUX_5_GPIO } },
	{ ENCODER_2A_PIN, { ENCODER_2_B_CHANNEL, MUX_5_GPIO }, { ENCODER_2_SW_CHANNEL, MUX_5_GPIO } },
	{ ENCODER_3A_PIN, { ENCODER_3_B_CHANNEL, MUX_7_GPIO }, { ENCODER_3_SW_CHANNEL, MUX_7_GPIO } },
	{ ENCODER_4A_PIN, { ENCODER_4_B_CHANNEL, MUX_8_GPIO }, { ENCODER_4_SW_CHANNEL, MUX_8_GPIO } },
	{ ENCODER_5A_PIN, { ENCODER_5_B_CHANNEL, MUX_6_GPIO }, { ENCODER_5_SW_CHANNEL, MUX_6_GPIO } },
	{ ENCODER_6A_PIN, { ENCODER_6_B_CHANNEL, MUX_7_GPIO }, { ENCODER_6_SW_CHANNEL, MUX_7_GPIO } },
	{ ENCODER_7A_PIN, { ENCODER_7_B_CHANNEL, MUX_7_GPIO }, { ENCODER_7_SW_CHANNEL, MUX_7_GPIO } },
	{ ENCODER_8A_PIN, { ENCODER_8_B_CHANNEL, MUX_8_GPIO }, { ENCODER_8_SW_CHANNEL, MUX_8_GPIO } }
};

constexpr PotiConfig POTIS[POT_MUX_COUNT] = {
	{ DIRECT_INPUT, POT_1_PIN },
	{ DIRECT_INPUT, POT_2_PIN },
	{ DIRECT_INPUT, POT_3_PIN },
	{ DIRECT_INPUT, POT_4_PIN }
};

static_assert(ENCODERS[0].gpioB.muxIndex == ENCODER_1_B_CHANNEL && ENCODERS[0].gpioB.gpio == MUX_5_GPIO, "ENC1_B mapping mismatch");
static_assert(ENCODERS[0].switchInput.muxIndex == ENCODER_1_SW_CHANNEL && ENCODERS[0].switchInput.gpio == MUX_5_GPIO, "ENC1_SW mapping mismatch");
static_assert(ENCODERS[1].gpioB.muxIndex == ENCODER_2_B_CHANNEL && ENCODERS[1].gpioB.gpio == MUX_5_GPIO, "ENC2_B mapping mismatch");
static_assert(ENCODERS[1].switchInput.muxIndex == ENCODER_2_SW_CHANNEL && ENCODERS[1].switchInput.gpio == MUX_5_GPIO, "ENC2_SW mapping mismatch");
static_assert(ENCODERS[2].gpioB.muxIndex == ENCODER_3_B_CHANNEL && ENCODERS[2].gpioB.gpio == MUX_7_GPIO, "ENC3_B mapping mismatch");
static_assert(ENCODERS[2].switchInput.muxIndex == ENCODER_3_SW_CHANNEL && ENCODERS[2].switchInput.gpio == MUX_7_GPIO, "ENC3_SW mapping mismatch");
static_assert(ENCODERS[3].gpioB.muxIndex == ENCODER_4_B_CHANNEL && ENCODERS[3].gpioB.gpio == MUX_8_GPIO, "ENC4_B mapping mismatch");
static_assert(ENCODERS[3].switchInput.muxIndex == ENCODER_4_SW_CHANNEL && ENCODERS[3].switchInput.gpio == MUX_8_GPIO, "ENC4_SW mapping mismatch");
static_assert(ENCODERS[4].gpioB.muxIndex == ENCODER_5_B_CHANNEL && ENCODERS[4].gpioB.gpio == MUX_6_GPIO, "ENC5_B mapping mismatch");
static_assert(ENCODERS[4].switchInput.muxIndex == ENCODER_5_SW_CHANNEL && ENCODERS[4].switchInput.gpio == MUX_6_GPIO, "ENC5_SW mapping mismatch");
static_assert(ENCODERS[5].gpioB.muxIndex == ENCODER_6_B_CHANNEL && ENCODERS[5].gpioB.gpio == MUX_7_GPIO, "ENC6_B mapping mismatch");
static_assert(ENCODERS[5].switchInput.muxIndex == ENCODER_6_SW_CHANNEL && ENCODERS[5].switchInput.gpio == MUX_7_GPIO, "ENC6_SW mapping mismatch");
static_assert(ENCODERS[6].gpioB.muxIndex == ENCODER_7_B_CHANNEL && ENCODERS[6].gpioB.gpio == MUX_7_GPIO, "ENC7_B mapping mismatch");
static_assert(ENCODERS[6].switchInput.muxIndex == ENCODER_7_SW_CHANNEL && ENCODERS[6].switchInput.gpio == MUX_7_GPIO, "ENC7_SW mapping mismatch");
static_assert(ENCODERS[7].gpioB.muxIndex == ENCODER_8_B_CHANNEL && ENCODERS[7].gpioB.gpio == MUX_8_GPIO, "ENC8_B mapping mismatch");
static_assert(ENCODERS[7].switchInput.muxIndex == ENCODER_8_SW_CHANNEL && ENCODERS[7].switchInput.gpio == MUX_8_GPIO, "ENC8_SW mapping mismatch");

#endif // MULTIPLEXER_MAP_H