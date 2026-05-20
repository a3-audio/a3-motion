#include <Adafruit_NeoPixel.h>

#include "a3_special.h"
#include "buttons.h"
#include "config.h"
#include "encoder.h"
#include "mux_io.h"
#include "multiplexer_map.h"
#include "potis.h"
#include "protocol.h"
#include "usart.h"

Adafruit_NeoPixel strip(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
static bool ledState[NUMPIXELS] = {};
static bool ledPrevPressed[NUMPIXELS] = {};

static inline bool isAssigned(const ButtonConfig &button) {
    return button.muxIndex != DIRECT_INPUT && button.gpio != UNASSIGNED_CHANNEL;
}

void setup() {
    // Configure MUX select lines as outputs
    for (uint8_t i = 0; i < SELECT_PIN_COUNT; i++) {
        pinMode(SELECT_PINS[i], OUTPUT);
        digitalWrite(SELECT_PINS[i], LOW);
    }

    // Button MUX output pins (MUX 0-4)
    for (uint8_t i = 0; i < BUTTON_MUX_COUNT; i++) {
        pinMode(MUX_PINS[i], INPUT_PULLUP);
    }

    // Encoder A direct pins
    for (uint8_t i = 0; i < ENCODER_COUNT; i++) {
        pinMode(ENCODER_A_PINS[i], INPUT_PULLUP);
    }

    // Encoder/extra-button MUX output pins (MUX 5-8)
    for (uint8_t i = BUTTON_MUX_COUNT; i < 9; i++) {
        pinMode(MUX_PINS[i], INPUT_PULLUP);
    }

    // Poti ADC pins
    for (uint8_t i = 0; i < POT_MUX_COUNT; i++) {
        pinMode(POT_PINS[i], INPUT);
    }

    buttons_init();
    encoders_init();
    usart_init(115200);
    delay(20);
    Serial0.println("a3-motion ready");

    strip.begin();
    strip.show();
}

void loop() {
    // Update debouncers and encoder quadrature every iteration
    buttons_update();
    encoders_update();

    // Handle one incoming protocol command per loop if available
    uint8_t cmdByte;
    if (usart_readByte(&cmdByte)) {
        protocol_process(cmdByte);
    }

    // Easter egg: BUTTON_00 + BUTTON_09 held simultaneously
    if (readMuxDigital(BUTTON_00) == LOW && readMuxDigital(BUTTON_09) == LOW) {
        runA3Special(strip, BUTTON_00, BUTTON_09);
        delay(MAIN_LOOP_DELAY_MS);
        return;
    }

    // LED toggle: pressing a button toggles its corresponding LED (non-blocking)
    // This section is now disabled to allow host control only.
    /*
    for (int led = 0; led < NUMPIXELS; led++) {
        if (!isAssigned(LED_MAP[led])) continue;
        bool pressed = (readMuxDigital(LED_MAP[led]) == LOW);
        if (pressed && !ledPrevPressed[led]) {
            ledState[led] = !ledState[led];
            strip.setPixelColor(led, ledState[led] ? strip.Color(0, 0, 255) : 0);
            strip.show();
        }
        ledPrevPressed[led] = pressed;
    }
    */

    delay(MAIN_LOOP_DELAY_MS);
}
