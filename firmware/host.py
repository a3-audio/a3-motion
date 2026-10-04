#!/usr/bin/env python3
"""
a3-motion host – polls the firmware and prints human-readable input events.

Usage:
    python3 host.py [PORT] [-v|--verbose] [--no-hold]
                    [--poti-divider N] [--poti-threshold N] [--poti-leds] [--enc-leds]  (default: auto)

    -v / --verbose  Print all encoder and pot values every cycle,
                    not just changes. Useful to verify hardware connections.
    --no-hold       Suppress continuously pressed buttons (HOLD).
    --poti-divider  Read pots every Nth cycle (default: 1 = every cycle).
    --poti-threshold  Minimum ADC delta to report in non-verbose mode (default: 2).
    --poti-leds     Drive LEDs 0-43 as a fill bar from POT1 for latency testing.
    --enc-leds      Drive LEDs 0-43 as a fill bar from ENCODER1 for latency testing.

Protocol (firmware/include/protocol.h):
    0x01  PING         → [0x01]                           1 B
    0x02  GET_POTS     → [0x02] + 4 × u16 LE              9 B
    0x03  GET_ENCODERS → [0x03] + 8 × (i16 LE, u8)       25 B
    0x04  GET_BUTTONS  → [0x04] + 11 packed bytes         12 B
"""

import json
import sys
import time
import struct
from pathlib import Path

import serial
from serial.tools import list_ports

# ── button labels in MATRIX_BUTTONS[] order ──────────────────────────────────
BUTTON_LABELS = [
    "40","30","20","50", "21","51","31","41",
    "42","32","22","52", "23","53","33","43",
    "44","34","24","54", "25","55","35","45",
    "46","36","26","56", "27","57","37","47",
    "48","38","28","58", "29","59","39","49",
    "00","10","09","19",
]

# ── 2-bit button-state meanings ───────────────────────────────────────────────
# bit0 = current level (0=pressed/LOW, 1=released/HIGH)
# bit1 = sawRise && sawFall (full cycle happened)
STATE_LABEL = {0: "HOLD", 1: "-", 2: "BOUNCE", 3: "CLICK"}

DEFAULT_POT_DIVIDER = 1  # poll pots every cycle for direct feel
DEFAULT_POT_THRESHOLD = 4

# Pre-built command batches: send all commands for a cycle in one write,
# receive all responses in one blocking read.
#   BTN(12) + ENC(25)          = 37 bytes  (normal cycle)
#   BTN(12) + ENC(25) + POT(9) = 46 bytes  (pot cycle)
_CMD_BTN_ENC     = bytes([0x04, 0x03])
_CMD_BTN_ENC_POT = bytes([0x04, 0x03, 0x02])

_CMD_SET_LED = 0x05
_CMD_SET_ALL_LEDS = 0x06
LED_ON_RGB = (0, 0, 255)
LED_OFF_RGB = (0, 0, 0)

# LED index -> button label mapping, matching firmware LED_MAP[]
LED_BUTTON_LABELS = [
    "09", "19", "29", "39", "49", "59",
    "58", "48", "38", "28",
    "27", "37", "47", "57",
    "56", "46", "36", "26",
    "25", "35", "45", "55",
    "54", "44", "34", "24",
    "23", "33", "43", "53",
    "52", "42", "32", "22",
    "21", "31", "41", "51",
    "50", "40", "30", "20", "10", "00",
]

_BUTTON_INDEX_BY_LABEL = {label: i for i, label in enumerate(BUTTON_LABELS)}
_LED_BUTTON_INDEX = [_BUTTON_INDEX_BY_LABEL[label] for label in LED_BUTTON_LABELS]

# Timeout long enough for 3 × firmware loop delay (3 × 20 ms) + margin.
# A single port.read(n) call blocks until all n bytes arrive or this elapses.
SERIAL_TIMEOUT = 0.25
TEST_LED_START = 0
TEST_LED_COUNT = 44
ENC_LED_MIN_POS = 0
ENC_LED_MAX_POS = TEST_LED_COUNT


# The board file is the one place that says which USB device the panel is:
# PlatformIO finds the upload port by it, and so does this script. The tty
# number is not an identity -- it is ttyACM0 on one machine and ttyACM1 on
# the next, depending on what else is plugged in.
BOARD_FILE = Path(__file__).resolve().parent / "boards" / "esp32-s3-devkitc-1-n16r8.json"

PING_ATTEMPTS = 5
PING_RETRY_S = 0.4
BOOT_WAIT_S = 0.6


def panel_usb_ids(board_file: Path = BOARD_FILE) -> set:
    hwids = json.loads(Path(board_file).read_text())["build"]["hwids"]
    return {(int(vid, 16), int(pid, 16)) for vid, pid in hwids}


def panel_candidates(ports, usb_ids) -> list:
    return sorted(p.device for p in ports if (p.vid, p.pid) in usb_ids)


def answers_ping(device: str) -> bool:
    """Whether the panel's firmware answers on this port.

    DTR and RTS run to the ESP32's auto-reset circuit, so they are set low
    before the port opens; the wait and the retries cover a board that reset
    anyway and is still printing its ROM banner.
    """
    port = serial.Serial()
    port.port = device
    port.baudrate = 115200
    port.timeout = SERIAL_TIMEOUT
    port.dtr = False
    port.rts = False
    try:
        port.open()
    except serial.SerialException:
        return False
    try:
        time.sleep(BOOT_WAIT_S)
        for _ in range(PING_ATTEMPTS):
            port.reset_input_buffer()
            port.write(b"\x01")
            if port.read(1) == b"\x01":
                return True
            time.sleep(PING_RETRY_S)
        return False
    finally:
        port.close()


def _format_usb_ids(usb_ids) -> str:
    return ", ".join(f"{vid:04X}:{pid:04X}" for vid, pid in sorted(usb_ids))


def pick_panel_port(candidates, ping=answers_ping, usb_ids=()) -> str:
    if len(candidates) == 1:
        return candidates[0]
    if not candidates:
        raise RuntimeError(
            f"No A3 Motion panel found (USB ID {_format_usb_ids(usb_ids)}). "
            "Connect it or pass PORT explicitly.")
    answering = [c for c in candidates if ping(c)]
    if len(answering) == 1:
        return answering[0]
    raise RuntimeError(
        f"{len(answering)} of {', '.join(candidates)} answered PING; "
        "pass PORT explicitly.")


def _auto_detect_port() -> str:
    usb_ids = panel_usb_ids()
    candidates = panel_candidates(list_ports.comports(), usb_ids)
    return pick_panel_port(candidates, answers_ping, usb_ids)


def read_exact(port: serial.Serial, n: int) -> bytes:
    """Block until exactly n bytes are received (relies on SERIAL_TIMEOUT)."""
    data = port.read(n)
    if len(data) != n:
        raise TimeoutError(f"expected {n} B, got {len(data)}")
    return data


def _validate_frame(raw: bytes, with_pots: bool) -> None:
    """Validate response markers and return dynamic frame offsets.

    Some USB-CDC stacks may deliver command responses in ENC+BTN order.
    Accept both layouts and parse by detected markers.
    """
    if len(raw) < (46 if with_pots else 37):
        raise ValueError(f"short frame: {len(raw)} bytes")

    # Layout A: BTN(12) + ENC(25) [+ POT(9)]
    if raw[0] == 0x04 and raw[12] == 0x03:
        if with_pots and raw[37] != 0x02:
            raise ValueError(f"bad pot frame marker: {raw[37]}")
        return 0, 12, (37 if with_pots else None)

    # Layout B: ENC(25) + BTN(12) [+ POT(9)]
    if raw[0] == 0x03 and raw[25] == 0x04:
        if with_pots and raw[37] != 0x02:
            raise ValueError(f"bad pot frame marker: {raw[37]}")
        return 25, 0, (37 if with_pots else None)

    raise ValueError(f"bad markers: b0={raw[0]} b12={raw[12]} b25={raw[25]}")


def _process_buttons(raw: bytes, offset: int, prev_down: list, show_hold: bool) -> None:
    """Parse 12-byte button frame and print button edge events."""
    if raw[offset] != 0x04:
        return
    packed = raw
    base = offset + 1
    events = []
    for i in range(44):
        state = (packed[base + (i >> 2)] >> ((i & 3) << 1)) & 0x03
        # Current debounced level: 0/2 => pressed, 1/3 => released.
        is_down = state in (0, 2)
        if is_down and not prev_down[i]:
            events.append(f"BTN_{BUTTON_LABELS[i]}=PRESS")
        elif (not is_down) and prev_down[i]:
            events.append(f"BTN_{BUTTON_LABELS[i]}=RELEASE")

        if show_hold and state == 0:
            events.append(f"BTN_{BUTTON_LABELS[i]}=HOLD")

        # Keep explicit click info when firmware reports a full round-trip.
        if state == 3:
            events.append(f"BTN_{BUTTON_LABELS[i]}=CLICK")

        prev_down[i] = is_down
    if events:
        print("BUTTONS  ", "  ".join(events))


def _decode_button_states(raw: bytes, offset: int) -> list:
    """Return 44 unpacked button states from a 12-byte button frame."""
    if raw[offset] != 0x04:
        return [1] * 44
    packed = raw
    base = offset + 1
    return [
        (packed[base + (i >> 2)] >> ((i & 3) << 1)) & 0x03
        for i in range(44)
    ]


def _set_led(port: serial.Serial, led_id: int, r: int, g: int, b: int) -> None:
    """Send one SET_LED command (no response expected)."""
    port.write(bytes([_CMD_SET_LED, led_id, r, g, b]))


def _apply_pot_led_test(port: serial.Serial, pot_vals: list, prev_level: int | None) -> int:
    """Use POT1 as a 44-step fill bar on LEDs 0..43."""
    level = (pot_vals[0] * TEST_LED_COUNT + 2047) // 4095  # 0..44 with rounded mapping
    if prev_level == level:
        return level

    if prev_level is None:
        # First update initializes the complete bar state.
        for step in range(TEST_LED_COUNT):
            led_id = TEST_LED_START + step
            if step < level:
                _set_led(port, led_id, 0, 0, 255)
            else:
                _set_led(port, led_id, 0, 0, 0)
        return level

    # Only touch LEDs that actually changed between previous and new level.
    if level > prev_level:
        for step in range(prev_level, level):
            _set_led(port, TEST_LED_START + step, 0, 0, 255)
    else:
        for step in range(level, prev_level):
            _set_led(port, TEST_LED_START + step, 0, 0, 0)
    return level


def _apply_encoder_led_test(
    port: serial.Serial,
    enc_deltas: list,
    enc_position: int,
    prev_level: int | None,
):
    """Use ENCODER1 as a 44-step fill bar on LEDs 0..43."""
    enc_position = max(ENC_LED_MIN_POS, min(ENC_LED_MAX_POS, enc_position + enc_deltas[0]))
    level = enc_position

    if prev_level == level:
        return enc_position, level

    if prev_level is None:
        for step in range(TEST_LED_COUNT):
            led_id = TEST_LED_START + step
            if step < level:
                _set_led(port, led_id, 0, 255, 40)
            else:
                _set_led(port, led_id, 0, 0, 0)
        return enc_position, level

    if level > prev_level:
        for step in range(prev_level, level):
            _set_led(port, TEST_LED_START + step, 0, 255, 40)
    else:
        for step in range(level, prev_level):
            _set_led(port, TEST_LED_START + step, 0, 0, 0)
    return enc_position, level


def _apply_button_led_toggle(port: serial.Serial, states: list, led_on: list, prev_pressed: list) -> None:
    """Replicate firmware behavior: toggle LED on new button press edge."""
    for led_id, button_idx in enumerate(_LED_BUTTON_INDEX):
        pressed = (states[button_idx] == 0)
        if pressed and not prev_pressed[button_idx]:
            led_on[led_id] = not led_on[led_id]
            r, g, b = LED_ON_RGB if led_on[led_id] else LED_OFF_RGB
            _set_led(port, led_id, r, g, b)
        prev_pressed[button_idx] = pressed


def _process_encoders(
    raw: bytes,
    offset: int,
    verbose: bool,
    prev_sw_down: list,
    show_hold: bool,
) -> None:
    """Parse 25-byte encoder frame starting at raw[offset]."""
    if raw[offset] != 0x03:
        return [0] * 8
    events = []
    deltas = [0] * 8
    base = offset + 1
    for i in range(8):
        o = base + i * 3
        delta = struct.unpack_from("<h", raw, o)[0]
        deltas[i] = delta
        sw    = raw[o + 2]

        if verbose and delta != 0:
            direction = "CW" if delta > 0 else ("CCW" if delta < 0 else "--")
            events.append(f"ENC{i+1}={direction}({delta:+d})")

        if not verbose and delta != 0:
            direction = "CW" if delta > 0 else "CCW"
            events.append(f"ENC{i+1}={direction}({delta:+d})")

        # Current debounced level: 0/2 => pressed, 1/3 => released.
        is_down = sw in (0, 2)
        if is_down and not prev_sw_down[i]:
            events.append(f"ENC{i+1}_SW=PRESS")
        elif (not is_down) and prev_sw_down[i]:
            events.append(f"ENC{i+1}_SW=RELEASE")

        if show_hold and sw == 0:
            events.append(f"ENC{i+1}_SW=HOLD")
        if sw == 3:
            events.append(f"ENC{i+1}_SW=CLICK")

        prev_sw_down[i] = is_down
    if events:
        print("ENCODERS ", "  ".join(events))
    return deltas


def _process_pots(
    raw: bytes,
    offset: int,
    prev: list,
    verbose: bool,
    threshold: int,
):
    """Parse 9-byte pot frame and report raw values with deadband."""
    if raw[offset] != 0x02:
        return prev
    vals = list(struct.unpack_from("<4H", raw, offset + 1))

    if verbose:
        print("POTS     ", "  ".join(f"POT{i+1}={vals[i]}" for i in range(4)))
    else:
        changed = [f"POT{i+1}={vals[i]}" for i in range(4)
                   if abs(vals[i] - prev[i]) >= threshold]
        if changed:
            print("POTS     ", "  ".join(changed))
    return vals


def main():
    args = sys.argv[1:]
    verbose = "-v" in args or "--verbose" in args
    show_hold = "--no-hold" not in args
    poti_leds_mode = "--poti-leds" in args
    enc_leds_mode = "--enc-leds" in args
    pot_divider = DEFAULT_POT_DIVIDER
    pot_threshold = DEFAULT_POT_THRESHOLD

    i = 0
    cleaned_args = []
    while i < len(args):
        a = args[i]
        if a in ("-v", "--verbose", "--no-hold", "--poti-leds", "--enc-leds"):
            i += 1
            continue
        if a == "--poti-divider" and i + 1 < len(args):
            pot_divider = max(1, int(args[i + 1]))
            i += 2
            continue
        if a == "--poti-threshold" and i + 1 < len(args):
            pot_threshold = max(0, int(args[i + 1]))
            i += 2
            continue
        cleaned_args.append(a)
        i += 1

    args = cleaned_args
    port_name = args[0] if args else _auto_detect_port()
    print(f"Connecting to {port_name} …" + (" [verbose]" if verbose else ""))

    with serial.Serial(port_name, 115200, timeout=SERIAL_TIMEOUT) as port:
        time.sleep(0.1)
        port.reset_input_buffer()
        print("Ready. Press Ctrl-C to quit.\n")

        pot_vals = [0, 0, 0, 0]
        led_on = [False] * 44
        prev_pressed = [False] * 44
        prev_down_print = [False] * 44
        prev_enc_sw_down = [False] * 8
        prev_pot_led_level = None
        prev_enc_led_level = None
        enc_led_position = 0
        cycle = 0
        while True:
            try:
                if cycle % pot_divider == 0:
                    port.write(_CMD_BTN_ENC_POT)
                    raw = read_exact(port, 46)
                    btn_off, enc_off, pot_off = _validate_frame(raw, with_pots=True)
                    states = _decode_button_states(raw, btn_off)
                    _process_buttons(raw, btn_off, prev_down_print, show_hold)
                    enc_deltas = _process_encoders(raw, enc_off, verbose, prev_enc_sw_down, show_hold)
                    pot_vals = _process_pots(raw, pot_off, pot_vals, verbose, pot_threshold)
                    if poti_leds_mode:
                        prev_pot_led_level = _apply_pot_led_test(port, pot_vals, prev_pot_led_level)
                    if enc_leds_mode:
                        enc_led_position, prev_enc_led_level = _apply_encoder_led_test(
                            port, enc_deltas, enc_led_position, prev_enc_led_level
                        )
                else:
                    port.write(_CMD_BTN_ENC)
                    raw = read_exact(port, 37)
                    btn_off, enc_off, _ = _validate_frame(raw, with_pots=False)
                    states = _decode_button_states(raw, btn_off)
                    _process_buttons(raw, btn_off, prev_down_print, show_hold)
                    enc_deltas = _process_encoders(raw, enc_off, verbose, prev_enc_sw_down, show_hold)
                    if enc_leds_mode:
                        enc_led_position, prev_enc_led_level = _apply_encoder_led_test(
                            port, enc_deltas, enc_led_position, prev_enc_led_level
                        )

                if not poti_leds_mode and not enc_leds_mode:
                    _apply_button_led_toggle(port, states, led_on, prev_pressed)
                cycle += 1
            except TimeoutError as e:
                print(f"[timeout] {e}")
                port.reset_input_buffer()
                cycle = 0
            except ValueError as e:
                print(f"[desync] {e}")
                port.reset_input_buffer()
                cycle = 0


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\nBye.")
