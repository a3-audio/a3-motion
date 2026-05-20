# A3 Motion – Firmware Team Reference

## Überblick

Die Firmware läuft auf einem **ESP32-S3-DevKitC-1-N16R8** (16 MB Flash, 8 MB OPI PSRAM) unter dem Arduino-Framework via PlatformIO.

Die Kommunikation mit dem Host (z. B. Raspberry Pi oder PC) findet über **USB-CDC** statt – der ESP erscheint dabei als `/dev/ttyACM0` oder `/dev/ttyACM1`. Es werden keine separaten Hardware-UART-Pins belegt. Das Protokoll ist **binär** (Byte-Frames) und ermöglicht Auslesen von Buttons, Encodern, Potis sowie LED-Steuerung.

---

## Projektstruktur

```
firmware/
├── boards/
│   └── esp32-s3-devkitc-1-n16r8.json   Lokale Board-Definition (16 MB/PSRAM + USB-CDC aktiv)
├── include/
│   ├── config.h             GPIO-Pins, Konstanten, LED-Matrix-Definition
│   ├── multiplexer_map.h    Button-/Encoder-/Poti-Mappings (MATRIX_BUTTONS, ENCODERS, POTIS)
│   ├── mux_io.h             MUX-Kanal-Selektion und digitale Lesefunktionen
│   ├── buttons.h            Bounce2-Debounce + 2-Bit-Zustandsverfolgung für alle Buttons
│   ├── encoder.h            Quadratur-Dekodierung, Delta-Puffer, Encoder-Schalter-Debounce
│   ├── potis.h              ADC-Lesefunktionen für alle Potis
│   ├── usart.h              Thin Wrapper um USB-CDC Serial
│   ├── protocol.h           Protokoll-Dispatcher (PING, GET_POTS, GET_ENCODERS, GET_BUTTONS)
│   └── a3_special.h         Easter-Egg-Deklaration
└── src/
    ├── main.cpp             Setup, Loop, Protokoll-Dispatch, Easter-Egg-Trigger
    ├── mux_io.cpp
    ├── buttons.cpp
    ├── encoder.cpp
    ├── potis.cpp
    ├── usart.cpp
    ├── protocol.cpp
    └── a3_special.cpp       A/III LED-Matrix-Animation
```

---

## Abhängigkeiten / Libraries

| Library                | Zweck                                    | PlatformIO-Quelle              |
|------------------------|------------------------------------------|--------------------------------|
| Adafruit NeoPixel      | LED-Strip-Ansteuerung                    | `adafruit/Adafruit NeoPixel`   |
| **Bounce2**            | Button-Debounce (alle Matrix-Buttons und Encoder-Schalter) | `thomasfredericks/Bounce2` |

PlatformIO lädt beide Libraries automatisch beim ersten `pio run`. Kein manuelles Installieren nötig.

---

## Kommunikationsprotokoll

### Verbindungsparameter

| Parameter     | Wert             |
|---------------|------------------|
| Port          | `/dev/ttyACM0`   |
| Format        | **Binär**, byteweise |

Der Port erscheint nach dem Einschalten des Geräts. Mit `ARDUINO_USB_MODE=1` (HWCDC) läuft die Übertragung über den nativen USB-Peripheral des ESP32-S3 (USB Full-Speed, ~12 Mbit/s). Die Baudrate-Einstellung im Host-Treiber ist für HWCDC irrelevant. 450 KB/s+ sind problemlos erreichbar.

### Protokoll-Struktur

- **Request**: immer **1 Byte** (Opcode)
- **Response**: bei Read-Kommandos **1 Byte Opcode-Echo** (Header) + Nutzdaten
- `SET_LED` und `SET_ALL_LEDS` haben **keine** Antwort

### Befehle und Antworten

#### `0x01` PING
```
→ [0x01]
← [0x01]                          (1 Byte)
```

#### `0x02` GET_POTS
Liest alle 4 Potis (direktverdrahtet am ADC, kein MUX).
```
→ [0x02]
← [0x02] [u16 LE] [u16 LE] [u16 LE] [u16 LE]    (9 Byte)
```
- 4 × ADC-Wert 0–4095 als uint16, Little-Endian

#### `0x03` GET_ENCODERS
Liest akkumulierte Rotationsdeltas und Schalter-Zustände. **Puffer wird nach dem Lesen auf 0 zurückgesetzt.**
```
→ [0x03]
← [0x03] (8 × [i16 LE delta] [u8 switchState])   (25 Byte)
```
- `delta`: signed int16 LE, akkumuliert seit letztem Aufruf (positiv = CW)
- `switchState`: 2-Bit-Zustand (s. u.)

#### `0x04` GET_BUTTONS
Liest den 2-Bit-Zustand aller 44 Matrix-Buttons. **Transition-Flags werden nach dem Lesen zurückgesetzt.**
```
→ [0x04]
← [0x04] [11 Byte gepackt]                        (12 Byte)
```
- 44 Buttons × 2 Bit = 88 Bit → 11 Bytes, LSB-first, 4 Buttons pro Byte
- Byte 0: Bits[1:0]=btn0, [3:2]=btn1, [5:4]=btn2, [7:6]=btn3, …
- Reihenfolge entspricht `MATRIX_BUTTONS[]` in `multiplexer_map.h`

#### `0x05` SET_LED
Setzt eine einzelne LED per RGB.
```
→ [0x05] [u8 led_id] [u8 r] [u8 g] [u8 b]         (5 Byte)
```
- keine Response
- `led_id` Bereich: `0..NUMPIXELS-1`

#### `0x06` SET_ALL_LEDS
Setzt alle LEDs auf dieselbe Farbe.
```
→ [0x06] [u8 r] [u8 g] [u8 b]                      (4 Byte)
```
- keine Response

#### `0xFF` ERR — unbekannter Opcode
```
← [0xFF] [echo des empfangenen Bytes]             (2 Byte)
```

### 2-Bit-Zustandscodierung (Buttons und Encoder-Schalter)

| Wert | Bedeutung | Details |
|------|-----------|---------|
| `0`  | Steady pressed  | Button war die ganze Zeit LOW (gedrückt), keine Flanke |
| `1`  | Steady released | Button war HIGH (losgelassen) oder gerade erst HIGH geworden |
| `2`  | Brief release   | Button war LOW → kurz HIGH → wieder LOW (kurz losgelassen) |
| `3`  | **CLICK**       | Button war HIGH → LOW (Drücken) → HIGH (Loslassen) |

- **bit0** = aktueller Debounce-Pegel (0 = LOW/gedrückt, 1 = HIGH/losgelassen)
- **bit1** = beide Flanken (Drücken und Loslassen) wurden seit letztem Aufruf erkannt

---

## Hardware-Anmerkungen

### MUX-Schema
- **5 Button-MUXe** (MUX 0–4): je 8 Kanäle → 40 Matrix-Buttons
- **4 Extra-Buttons** (BUTTON_00, 10, 09, 19) auf MUX 5 und MUX 8
- **Encoder-B + Schalter** auf MUX 5, 6, 7 und 8
- **Encoder-A** direkt verkabelt (GPIO 5–7, 10–13, 18)
- **Potis** (4×) direkt am ADC (GPIO 3, 4, 16, 17) – kein MUX

### Bounce2 mit MUX
Da Bounce2 intern `digitalRead(pin)` aufruft, wird vor jedem `update()`-Aufruf manuell der korrekte MUX-Kanal gesetzt (`selectMuxChannel()`). Mehrere Bounce2-Objekte können denselben GPIO-Pin teilen, solange der MUX-Kanal unmittelbar vor dem `update()` gesetzt wird.

### USB-CDC-Konfiguration
Die lokale Board-Datei `boards/esp32-s3-devkitc-1-n16r8.json` setzt `-DARDUINO_USB_MODE=1` (Hardware-USB-CDC). Damit mappt `Serial` auf den nativen USB-Peripheral des ESP32-S3 und erscheint als `/dev/ttyACM0` – ohne GPIO-Pins zu belegen.

### LED-Steuerung
Die frühere LED-Toggle-Logik in der Firmware-Loop ist deaktiviert. LEDs werden ausschließlich per Host-Protokoll (`0x05`/`0x06`) gesteuert.

### Easter Egg
BUTTON_00 und BUTTON_09 gleichzeitig halten → 5 Sekunden lang wechselt die LED-Matrix zwischen `A` und `III` (je 500 ms). Implementiert in `src/a3_special.cpp`.

---

## Build und Upload

```bash
# Firmware bauen
pio run

# Firmware hochladen
pio run -t upload

# Seriellen Monitor öffnen
pio device monitor
# oder direkt:
python3 -m serial.tools.miniterm /dev/ttyACM0 115200

# Hinweis: Nicht parallel zu ui/host.py auf demselben Port öffnen.
```

### Schnelltest des Protokolls
```python
import serial, struct

ser = serial.Serial('/dev/ttyACM0', 2000000, timeout=1)

# PING
ser.write(b'\x01')
print(ser.read(1))          # b'\x01'

# GET_POTS
ser.write(b'\x02')
data = ser.read(9)          # 1 header + 4 × uint16
opcode, v0, v1, v2, v3 = struct.unpack('<BHHHH', data)
print(v0, v1, v2, v3)       # ADC 0..4095

# GET_BUTTONS
ser.write(b'\x04')
data = ser.read(12)         # 1 header + 11 packed bytes
packed = data[1:]
states = [(packed[i // 4] >> ((i % 4) * 2)) & 0x03 for i in range(44)]
print(states)               # list of 0/1/2/3 per button
```

---

## Python-Client (Master-Seite — TODO für das Team)

> **Aufgabe:** Auf dem Master (Raspberry Pi o. ä.) muss ein Python-Skript erstellt werden, das dieselben Protokoll-Opcodes kennt und darüber mit dem ESP kommuniziert. Die unten stehende Beispiel-Klasse kann als Ausgangspunkt dienen — sie muss noch in die Master-Software integriert werden (z. B. in `rpi/`).

Der ESP sendet ausschließlich Antworten auf eingehende Opcodes (kein automatisches Push). Das Protokoll ist **binär** – kein Text, kein ASCII-Parsing.

Benötigte Library: `pyserial` (`pip install pyserial`).

Beispiel-Klasse:
```python
import serial, struct

class A3MotionClient:
    PING         = 0x01
    GET_POTS     = 0x02
    GET_ENCODERS = 0x03
    GET_BUTTONS  = 0x04
    SET_LED      = 0x05
    SET_ALL_LEDS = 0x06

    def __init__(self, port='/dev/ttyACM0'):
        self.ser = serial.Serial(port, 2000000, timeout=0.1)

    def ping(self):
        self.ser.write(bytes([self.PING]))
        return self.ser.read(1) == b'\x01'

    def get_pots(self):
        self.ser.write(bytes([self.GET_POTS]))
        data = self.ser.read(9)
        return struct.unpack_from('<4H', data, 1)  # (v0, v1, v2, v3)

    def get_encoders(self):
        self.ser.write(bytes([self.GET_ENCODERS]))
        data = self.ser.read(25)
        result = []
        for i in range(8):
            delta, = struct.unpack_from('<h', data, 1 + i * 3)
            sw = data[3 + i * 3]
            result.append((delta, sw))
        return result  # [(delta, switchState), ...]

    def get_buttons(self):
        self.ser.write(bytes([self.GET_BUTTONS]))
        data = self.ser.read(12)
        packed = data[1:]
        return [(packed[i // 4] >> ((i % 4) * 2)) & 0x03 for i in range(44)]

    def set_led(self, led_id, r, g, b):
        self.ser.write(bytes([self.SET_LED, led_id & 0xFF, r & 0xFF, g & 0xFF, b & 0xFF]))

    def set_all_leds(self, r, g, b):
        self.ser.write(bytes([self.SET_ALL_LEDS, r & 0xFF, g & 0xFF, b & 0xFF]))
```

---

## Dateiänderungen kurz erklärt

| Datei | Änderung |
|-------|----------|
| `boards/esp32-s3-devkitc-1-n16r8.json` | `ARDUINO_USB_MODE=1` gesetzt (USB-CDC aktiv); `ARDUINO_USB_CDC_ON_BOOT` entfernt |
| `platformio.ini` | `thomasfredericks/Bounce2` zu `lib_deps` hinzugefügt |
| `src/main.cpp` | Komplett refactored: nur noch Setup, Loop, LED-Toggle und Easter-Egg-Trigger |
| `include/a3_special.h` | Nutzt jetzt `mux_io.h` statt eigenem `extern`-Deklaration |
| `include/mux_io.h` + `src/mux_io.cpp` | Neu: MUX-Hilfsfunktionen ausgelagert |
| `include/buttons.h` + `src/buttons.cpp` | Neu: Bounce2 + 2-Bit-Zustand für alle 44 Matrix-Buttons |
| `include/encoder.h` + `src/encoder.cpp` | Neu: Quadratur-Dekodierung, Delta-Puffer, Schalter-Debounce |
| `include/potis.h` + `src/potis.cpp` | Neu: ADC-Lesefunktionen |
| `src/usart.cpp` / `include/usart.h` | Neu binär: `usart_readByte()` + `usart_write(data, len)`; kein readline/writeln mehr |
| `src/protocol.cpp` / `include/protocol.h` | Neu binär: 1-Byte-Opcodes, packed binary responses; kein snprintf/strcmp |

