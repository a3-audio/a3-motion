#ifndef MULTIPLEXER_MAP_H
#define MULTIPLEXER_MAP_H

/*
 * ESP32 Controller Platine - Eingänge
 *
 * INPUT-METHODEN:
 * 1. 5x 74HC4051 Multiplexer für Buttons (40 Eingänge + 4 Reserve)
 * 2. 8x Encoder + 4x Potentiometer direkt an GPIO
 * 3. 44x WS2812 LEDs an GPIO 14
 */

// ============================================================================
// SELECT PINS (für alle 5 Button-Multiplexer gemeinsam)
// ============================================================================
#define MUX_S0 39   // GPIO 2  (ms0)
#define MUX_S1 38   // GPIO 15 (ms1)
#define MUX_S2 48   // GPIO 9  (ms2)

// ============================================================================
// MUX OUT PINS (diese Leitungen werden eingelesen)
// ============================================================================
#define MUX_OUT_A 2   // (MA)
#define MUX_OUT_B 15  // (MB)
#define MUX_OUT_C 8   // (MC)
#define MUX_OUT_D 9   // (MD)

// Kanalbelegung MUX A
// A0 ENC1_SW, A1 BTN10, A2 BTN00, A5 ENC1_B, A6 ENC2_B, A7 ENC2_SW
// Kanalbelegung MUX B
// A5 ENC5_SW, A7 ENC5_B
// Kanalbelegung MUX C
// A0 ENC3_SW, A1 ENC6_B, A2 ENC6_SW, A3 ENC3_B, A5 ENC7_SW, A7 ENC7_B
// Kanalbelegung MUX D
// A0 ENC4_SW, A3 ENC4_B, A4 ENC8_B, A5 BTN19, A6 ENC8_SW, A7 BTN09



// ============================================================================
// 5 BUTTON-MULTIPLEXER OUTPUT PINS
// ============================================================================
#define MUX_BUTTON_0_OUT 41 // Button40, Button30, Button20, Button50, Button21, Button51, Button31, Button41

#define MUX_BUTTON_1_OUT 42 // Button42, Button32, Button22, Button52, Button23, Button53, Button33, Button43
#define MUX_BUTTON_2_OUT 40 // Button44, Button34, Button24, Button54, Button25, Button55, Button35, Button45
#define MUX_BUTTON_3_OUT 21 // Button46, Button36, Button26, Button56, Button27, Button57, Button37, Button47
#define MUX_BUTTON_4_OUT 47 // Button48, Button38, Button28, Button58, Button29, Button59, Button39, Button49


// ============================================================================
// BUTTON-MATRIX: 5 MUX × 8 Kanäle = 40 Buttons
// ============================================================================
// 40 Matrix-Buttons auf den 5 Button-MUXen plus 4 zusätzliche Buttons auf MUX A/D.
#define BUTTON_COUNT 44

#define EXTRA_BUTTON_10_MUX_INDEX 0
#define EXTRA_BUTTON_10_CHANNEL 1

#define EXTRA_BUTTON_00_MUX_INDEX 0
#define EXTRA_BUTTON_00_CHANNEL 2

#define EXTRA_BUTTON_19_MUX_INDEX 3
#define EXTRA_BUTTON_19_CHANNEL 5

#define EXTRA_BUTTON_09_MUX_INDEX 3
#define EXTRA_BUTTON_09_CHANNEL 7

// ============================================================================
// ENCODER PINS (direkt an GPIO)
// ============================================================================
// 8x Encoder, je 1x Phase A direkt an GPIO.
// Phase B und Switch werden ueber MUX A-D gelesen.
#define ENCODER_1_PIN 6   // MEA21
#define ENCODER_2_PIN 7   // MEA23
#define ENCODER_3_PIN 11  // MEA25
#define ENCODER_4_PIN 12  // MEA27
#define ENCODER_5_PIN 5   // MEA12
#define ENCODER_6_PIN 18  // MEA14
#define ENCODER_7_PIN 10  // MEA16
#define ENCODER_8_PIN 13  // MEA18

#define ENCODER_1_B_MUX_INDEX 0
#define ENCODER_1_B_CHANNEL 5
#define ENCODER_1_SW_MUX_INDEX 0
#define ENCODER_1_SW_CHANNEL 0

#define ENCODER_2_B_MUX_INDEX 0
#define ENCODER_2_B_CHANNEL 6
#define ENCODER_2_SW_MUX_INDEX 0
#define ENCODER_2_SW_CHANNEL 7

#define ENCODER_3_B_MUX_INDEX 2
#define ENCODER_3_B_CHANNEL 3
#define ENCODER_3_SW_MUX_INDEX 2
#define ENCODER_3_SW_CHANNEL 0

#define ENCODER_4_B_MUX_INDEX 3
#define ENCODER_4_B_CHANNEL 3
#define ENCODER_4_SW_MUX_INDEX 3
#define ENCODER_4_SW_CHANNEL 0

#define ENCODER_5_B_MUX_INDEX 1
#define ENCODER_5_B_CHANNEL 7
#define ENCODER_5_SW_MUX_INDEX 1
#define ENCODER_5_SW_CHANNEL 5

#define ENCODER_6_B_MUX_INDEX 2
#define ENCODER_6_B_CHANNEL 1
#define ENCODER_6_SW_MUX_INDEX 2
#define ENCODER_6_SW_CHANNEL 2

#define ENCODER_7_B_MUX_INDEX 2
#define ENCODER_7_B_CHANNEL 7
#define ENCODER_7_SW_MUX_INDEX 2
#define ENCODER_7_SW_CHANNEL 5

#define ENCODER_8_B_MUX_INDEX 3
#define ENCODER_8_B_CHANNEL 4
#define ENCODER_8_SW_MUX_INDEX 3
#define ENCODER_8_SW_CHANNEL 6

// ============================================================================
// POTENTIOMETER PINS (ADC)
// ============================================================================
// 4x Potentiometer
#define POT_1_PIN 4   // MEA01
#define POT_2_PIN 16  // MEA03
#define POT_3_PIN 17  // MEA05
#define POT_4_PIN 3   // MEA07

// ============================================================================
// LED PINS
// ============================================================================
#define LED_DATA_PIN 14
#define LED_COUNT 44

#endif // MULTIPLEXER_MAP_H