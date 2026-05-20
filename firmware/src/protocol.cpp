#include "protocol.h"
#include "buttons.h"
#include "config.h"
#include "encoder.h"
#include "potis.h"
#include "usart.h"
#include "Adafruit_NeoPixel.h"
#include <stdint.h>

#define CMD_PING         0x01u
#define CMD_GET_POTS     0x02u
#define CMD_GET_ENCODERS 0x03u
#define CMD_GET_BUTTONS  0x04u
#define CMD_SET_LED      0x05u
#define CMD_SET_ALL_LEDS 0x06u
#define RSP_ERR          0xFFu

extern Adafruit_NeoPixel strip;
static bool g_ledsDirty = false;
static uint32_t g_lastLedShowUs = 0;

static bool read_bytes_wait(uint8_t *dst, uint8_t len, uint32_t timeoutMsPerByte) {
    for (uint8_t i = 0; i < len; i++) {
        if (!usart_readByteWait(&dst[i], timeoutMsPerByte)) {
            return false;
        }
    }
    return true;
}

static void write_button_frame(void) {
    uint8_t buf[12];
    buf[0] = CMD_GET_BUTTONS;
    for (uint8_t b = 0; b < 11; b++) buf[1 + b] = 0;

    uint8_t states[MATRIX_BUTTON_COUNT];
    buttons_readAndClear(states, MATRIX_BUTTON_COUNT);
    for (uint8_t i = 0; i < MATRIX_BUTTON_COUNT; i++) {
        uint8_t byteIdx = i / 4;
        uint8_t shift   = (i % 4) * 2;
        buf[1 + byteIdx] |= (states[i] & 0x03u) << shift;
    }
    usart_write(buf, sizeof(buf));
}

void protocol_process(uint8_t cmd) {
    switch (cmd) {

    case CMD_PING: {
        uint8_t rsp = CMD_PING;
        usart_write(&rsp, 1);
        break;
    }

    case CMD_GET_POTS: {
        // [0x02] + 4 × uint16_t LE = 9 bytes
        uint8_t buf[9];
        buf[0] = CMD_GET_POTS;
        uint16_t vals[POT_MUX_COUNT];
        potis_read(vals, POT_MUX_COUNT);
        for (uint8_t i = 0; i < POT_MUX_COUNT; i++) {
            buf[1 + i * 2]     = (uint8_t)(vals[i] & 0xFF);
            buf[1 + i * 2 + 1] = (uint8_t)(vals[i] >> 8);
        }
        usart_write(buf, sizeof(buf));
        break;
    }

    case CMD_GET_ENCODERS: {
        // [0x03] + 8 × (int16_t LE + uint8_t) = 25 bytes
        uint8_t buf[25];
        buf[0] = CMD_GET_ENCODERS;
        int16_t deltas[ENCODER_COUNT];
        uint8_t sw[ENCODER_COUNT];
        encoders_readAndClear(deltas, sw, ENCODER_COUNT);
        for (uint8_t i = 0; i < ENCODER_COUNT; i++) {
            uint16_t raw       = (uint16_t)deltas[i];
            buf[1 + i * 3]     = (uint8_t)(raw & 0xFF);
            buf[1 + i * 3 + 1] = (uint8_t)(raw >> 8);
            buf[1 + i * 3 + 2] = sw[i];
        }
        usart_write(buf, sizeof(buf));
        break;
    }

    case CMD_GET_BUTTONS: {
        // [0x04] + 11 packed bytes (44 × 2-bit, LSB-first, 4 buttons/byte) = 12 bytes
        write_button_frame();
        break;
    }

    case CMD_SET_LED: {
        uint8_t payload[4];
        if (read_bytes_wait(payload, 4, PROTOCOL_PAYLOAD_TIMEOUT_MS)) {
            uint8_t led_id = payload[0];
            uint8_t r = payload[1];
            uint8_t g = payload[2];
            uint8_t b = payload[3];
            uint32_t color = strip.Color(r, g, b);
            set_led(led_id, color);
        }
        break;
    }

    case CMD_SET_ALL_LEDS: {
        uint8_t payload[3];
        if (read_bytes_wait(payload, 3, PROTOCOL_PAYLOAD_TIMEOUT_MS)) {
            uint8_t r = payload[0];
            uint8_t g = payload[1];
            uint8_t b = payload[2];
            uint32_t color = strip.Color(r, g, b);
            set_all_leds(color);
        }
        break;
    }

    default: {
        uint8_t rsp[2] = { RSP_ERR, cmd };
        usart_write(rsp, 2);
        break;
    }
    }
}

void set_led(uint8_t led_id, uint32_t color) {
    if (led_id < NUMPIXELS) {
        strip.setPixelColor(led_id, color);
        g_ledsDirty = true;
    }
}

void set_all_leds(uint32_t color) {
    for (uint8_t i = 0; i < NUMPIXELS; i++) {
        strip.setPixelColor(i, color);
    }
    g_ledsDirty = true;
}

void protocol_led_flush(void) {
    if (!g_ledsDirty) return;

    uint32_t nowUs = micros();
    if ((uint32_t)(nowUs - g_lastLedShowUs) < LED_SHOW_MIN_INTERVAL_US) {
        return;
    }

    strip.show();
    g_lastLedShowUs = nowUs;
    g_ledsDirty = false;
}
