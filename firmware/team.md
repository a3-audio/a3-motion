# Team Setup (Firmware)

Dieses Verzeichnis ist auf PlatformIO umgestellt.

## Zielstruktur

- `platformio.ini`
- `src/main.cpp`
- `include/config.h`
- `include/multiplexer_map.h`

## Status

- Migration von Arduino-Sketch zu PlatformIO-Struktur ist erfolgt.
- Deshalb wird aktuell ein Standard-Board verwendet: `esp32-s3-devkitc-1-n16r8`.
- Die Boarddefinition fuer `esp32-s3-devkitc-1-n16r8` liegt im Projekt unter `firmware/boards/esp32-s3-devkitc-1-n16r8.json`.
- `platformio.ini` zeigt mit `board_dir = boards` auf die lokale Boardkopie.
- `include/config.h` enthaelt nur noch die physischen GPIO-Pins und Pin-Arrays.
- `include/multiplexer_map.h` enthaelt die logischen Mappings fuer Button-, Encoder- und Poti-Eingaenge.
- `include/multiplexer_map.h` nutzt direkte `BUTTON_XX`-Defines fuer Matrix und LED-Map; die frueheren `EXTRA_BUTTON_*`-Namen sind aufgelost.
- Encoder sind zusammenhaengend definiert: `gpioA`, `gpioB` und `switchInput`.
- Easter Egg ist ausgelagert: `src/main.cpp` erkennt nur noch `BUTTON_00 + BUTTON_09`, die Anzeige laeuft in `src/a3_special.cpp`.
- Das Easter Egg zeigt auf der 5x9-LED-Matrix fuer 5 Sekunden abwechselnd `A` und `III`.
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
5. Wenn das Easter Egg veraendert wird, nur den Trigger in `src/main.cpp` und die Animation in `src/a3_special.cpp` anfassen.
