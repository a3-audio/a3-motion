#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

// Binary protocol: 1 byte request, 1 byte opcode header + data response.
//
// OPCODES (host → ESP, 1 byte):
//   0x01  PING
//   0x02  GET_POTS
//   0x03  GET_ENCODERS
//   0x04  GET_BUTTONS
//   0x05  SET_LED
//   0x06  SET_ALL_LEDS
//
// RESPONSES (ESP → host):
//
//   PING (0x01)
//     [0x01]                                        1 byte
//
//   GET_POTS (0x02)
//     [0x02] [u16 LE] [u16 LE] [u16 LE] [u16 LE]   9 bytes
//     4 × ADC value 0..4095 (12-bit, direct-wired potentiometers)
//
//   GET_ENCODERS (0x03)
//     [0x03] (8 × [i16 LE delta] [u8 switchState])  25 bytes
//     delta:       signed rotation since last read (positive = CW); cleared on read
//     switchState: 2-bit — 0=steady pressed, 1=steady released,
//                           2=brief release+re-press, 3=CLICK
//
//   GET_BUTTONS (0x04)
//     [0x04] [11 packed bytes]                      12 bytes
//     44 buttons × 2-bit state, packed LSB-first, 4 buttons per byte.
//     Byte 0 bits[1:0]=btn0, [3:2]=btn1, [5:4]=btn2, [7:6]=btn3, ...
//     Button order matches MATRIX_BUTTONS[] in multiplexer_map.h.
//     Transition flags cleared on read.
//
//   ERR (0xFF)  — unknown opcode received
//     [0xFF] [echo of unknown byte]                 2 bytes
//
//   SET_LED (0x05)
//     [0x05] [u8 led_id] [u32 color]               13 bytes
//     Set the color of a specific LED.
//
//   SET_ALL_LEDS (0x06)
//     [0x06] [u32 color]                          5 bytes
//     Set the color of all LEDs.

void protocol_process(uint8_t cmd);

void set_led(uint8_t led_id, uint32_t color);
void set_all_leds(uint32_t color);

#endif // PROTOCOL_H
