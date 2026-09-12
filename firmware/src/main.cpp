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
    static uint32_t nextInputScanUs = 0;

    uint32_t nowUs = micros();
    if ((int32_t)(nowUs - nextInputScanUs) >= 0) {
        // Scan inputs at a fixed cadence to keep MUX work predictable.
        buttons_update();
        encoders_update();
        nextInputScanUs = nowUs + INPUT_SCAN_INTERVAL_US;

        // Easter egg: BUTTON_00 + BUTTON_09 held simultaneously.
        if (readMuxDigital(BUTTON_00) == LOW && readMuxDigital(BUTTON_09) == LOW) {
            runA3Special(strip, BUTTON_00, BUTTON_09);
            return;
        }
    }

    // Drain several queued commands per loop without starving input scans.
    uint32_t cmdStartUs = micros();
    while ((uint32_t)(micros() - cmdStartUs) < CMD_PROCESS_BUDGET_US) {
        uint8_t cmdByte;
        if (!usart_readByte(&cmdByte)) {
            break;
        }
        protocol_process(cmdByte);
    }

    protocol_led_flush();
}
