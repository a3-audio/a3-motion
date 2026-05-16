# Team Setup (Firmware)

Dieses Verzeichnis ist auf PlatformIO umgestellt.

## Zielstruktur

- `platformio.ini`
- `src/main.cpp`
- `include/config.h`
- `include/multiplexer_map.h`

## Status

- Migration von Arduino-Sketch zu PlatformIO-Struktur ist erfolgt.
- In `GuideMill` wurden keine Custom-Board-Definitionen (`boards/*.json`) gefunden.
- Deshalb wird aktuell ein Standard-Board verwendet: `esp32-s3-devkitc-1-n16r8`.
- `include/multiplexer_map.h` ist jetzt angelegt und enthaelt Button-, Encoder- und Poti-Zuordnungen fuer den ESP32-S3.
- `include/config.h` verwendet jetzt Structs fuer I/O-Mapping (`ButtonConfig`, `EncoderConfig`, `PotiConfig`) mit GPIO-basierter Benennung.
- Encoder sind zusammenhaengend definiert: `gpioA`, `gpioB` und `switchInput`.
- Toolchain ist lokal lauffaehig: `pio run` baut erfolgreich im Ordner `firmware/`.

## Git-Hinweis

- KiCad-Lockdateien und `.history`-Ordner sind ueber die Root-`.gitignore` vom Commit ausgeschlossen.
- Fuer neue Dateien immer erst `git add -A` verwenden, danach `git commit -m "..."`.
- Beispiel: `git add -A && git commit -m "neue encoder definition und buttons"`
- Falls `pio` in neuen Shells nicht gefunden wird: Terminal neu starten oder `source ~/.bashrc` ausfuehren.

## To-Do fürs Team

1. Falls das Zielboard ein anderes ist, in `platformio.ini` den `board`-Eintrag anpassen.
2. Build pruefen mit `pio run`.
3. Upload/Monitor pruefen mit `pio run -t upload` und `pio device monitor`.
4. Falls weitere Button- oder Encoder-Zuordnungen geaendert werden, immer `include/multiplexer_map.h` und die Ausgabelogik in `src/main.cpp` zusammen pruefen.
