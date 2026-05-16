# Team Setup (Firmware)

Dieses Verzeichnis ist auf PlatformIO umgestellt.

## Zielstruktur

- `platformio.ini`
- `src/main.cpp`
- `include/config.h`
- `include/multiplexer_map.h` (noch anzulegen)

## Status

- Migration von Arduino-Sketch zu PlatformIO-Struktur ist erfolgt.
- In `GuideMill` wurden keine Custom-Board-Definitionen (`boards/*.json`) gefunden.
- Deshalb wird aktuell ein Standard-Board verwendet: `esp32-s3-devkitc-1-n16r8`.

## To-Do fürs Team

1. `include/multiplexer_map.h` mit allen Pin-Makros anlegen:
   - `MUX_BUTTON_0_OUT` ... `MUX_BUTTON_4_OUT`
   - `ENCODER_1_PIN` ... `ENCODER_8_PIN`
   - `POT_1_PIN` ... `POT_4_PIN`
   - `MUX_S0`, `MUX_S1`, `MUX_S2`
2. Falls das Zielboard ein anderes ist, in `platformio.ini` den `board`-Eintrag anpassen.
3. Build prüfen mit `pio run`.
4. Upload/Monitor prüfen mit `pio run -t upload` und `pio device monitor`.
