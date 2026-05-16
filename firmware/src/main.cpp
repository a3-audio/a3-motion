#include <Adafruit_NeoPixel.h>

#include "a3_special.h"
#include "config.h"
#include "multiplexer_map.h"

Adafruit_NeoPixel strip(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
bool ledState[NUMPIXELS] = { false };

void selectMuxChannel(uint8_t channel) {
  digitalWrite(SELECT_PINS[0], (channel >> 0) & 1);
  digitalWrite(SELECT_PINS[1], (channel >> 1) & 1);
  digitalWrite(SELECT_PINS[2], (channel >> 2) & 1);
  delayMicroseconds(MUX_SETTLE_DELAY_US);
}

bool isAssigned(const ButtonConfig &button) {
  return button.muxIndex != DIRECT_INPUT && button.gpio != UNASSIGNED_CHANNEL;
}

int readMuxDigital(const ButtonConfig &button) {
  selectMuxChannel(button.muxIndex);
  return digitalRead(button.gpio);
}

int readEncoderA(uint8_t encoderIndex) {
  return digitalRead(ENCODERS[encoderIndex].gpioA);
}

int readEncoderB(uint8_t encoderIndex) {
  return readMuxDigital(ENCODERS[encoderIndex].gpioB);
}

int readEncoderSwitch(uint8_t encoderIndex) {
  return readMuxDigital(ENCODERS[encoderIndex].switchInput);
}

int readPoti(uint8_t potiIndex) {
  const PotiConfig &poti = POTIS[potiIndex];
  if (poti.muxIndex != DIRECT_INPUT) {
    selectMuxChannel(poti.muxIndex);
  }
  return analogRead(poti.gpio);
}

void printLedMapArrays() {
  Serial.println("\n=== LED_TO_MUX ===");
  Serial.print("{ ");
  for (int i = 0; i < NUMPIXELS; i++) {
    if (isAssigned(LED_MAP[i])) {
      Serial.print(LED_MAP[i].muxIndex);
    } else {
      Serial.print(-1);
    }
    if (i < NUMPIXELS - 1) {
      Serial.print(", ");
    }
  }
  Serial.println(" }");

  Serial.println("=== LED_TO_CHANNEL ===");
  Serial.print("{ ");
  for (int i = 0; i < NUMPIXELS; i++) {
    if (isAssigned(LED_MAP[i])) {
      Serial.print(LED_MAP[i].gpio);
    } else {
      Serial.print(-1);
    }
    if (i < NUMPIXELS - 1) {
      Serial.print(", ");
    }
  }
  Serial.println(" }");
}

void scanAllMuxInputs() {
  for (uint8_t button = 0; button < MATRIX_BUTTON_COUNT; button++) {
    if (readMuxDigital(MATRIX_BUTTONS[button]) == LOW) {
      Serial.print("Detected: BUTTON ");
      Serial.println(button);
    }
  }

  for (uint8_t encoder = 0; encoder < ENCODER_COUNT; encoder++) {
    int a = readEncoderA(encoder);
    int b = readEncoderB(encoder);
    int sw = readEncoderSwitch(encoder);

    if (a == LOW || b == LOW || sw == LOW) {
      Serial.print("Detected: ENCODER ");
      Serial.print(encoder + 1);
      Serial.print(" A=");
      Serial.print(a == LOW ? "1" : "0");
      Serial.print(" B=");
      Serial.print(b == LOW ? "1" : "0");
      Serial.print(" SW=");
      Serial.println(sw == LOW ? "1" : "0");
    }
  }

  for (uint8_t poti = 0; poti < POT_MUX_COUNT; poti++) {
    int value = readPoti(poti);
    if (value < POT_ACTIVE_THRESHOLD) {
      Serial.print("Detected: POTI ");
      Serial.print(poti);
      Serial.print(" ADC=");
      Serial.println(value);
    }
  }
}

void printButtonMatrix() {
  Serial.println("\n=== BUTTON MATRIX ===");
  for (uint8_t mux = 0; mux < BUTTON_MUX_COUNT; mux++) {
    Serial.print("BTN OUT ");
    Serial.print(MATRIX_BUTTONS[mux * MUX_CHANNEL_COUNT].gpio);
    Serial.print(": [");
    for (uint8_t idx = 0; idx < MUX_CHANNEL_COUNT; idx++) {
      int value = readMuxDigital(MATRIX_BUTTONS[mux * MUX_CHANNEL_COUNT + idx]);
      Serial.print(value == LOW ? "1" : "0");
      if (idx < MUX_CHANNEL_COUNT - 1) {
        Serial.print(" ");
      }
    }
    Serial.println("]");
  }
}

void printAllValues() {
  printButtonMatrix();

  Serial.println("\n=== ENCODERS ===");
  for (uint8_t encoder = 0; encoder < ENCODER_COUNT; encoder++) {
    int a = readEncoderA(encoder);
    int b = readEncoderB(encoder);
    int sw = readEncoderSwitch(encoder);

    Serial.print("ENC ");
    Serial.print(encoder + 1);
    Serial.print(": A=");
    Serial.print(a == LOW ? "1" : "0");
    Serial.print(" B=");
    Serial.print(b == LOW ? "1" : "0");
    Serial.print(" SW=");
    Serial.println(sw == LOW ? "1" : "0");
  }

  Serial.println("\n=== POTIS ===");
  for (uint8_t poti = 0; poti < POT_MUX_COUNT; poti++) {
    Serial.print("POT ");
    Serial.print(poti);
    Serial.print(": ");
    Serial.println(readPoti(poti));
  }
}

void setup() {
  Serial.begin(115200);

  for (uint8_t i = 0; i < SELECT_PIN_COUNT; i++) {
    pinMode(SELECT_PINS[i], OUTPUT);
    digitalWrite(SELECT_PINS[i], LOW);
  }

  for (uint8_t i = 0; i < BUTTON_MUX_COUNT; i++) {
    pinMode(MUX_PINS[i], INPUT_PULLUP);
  }

  for (uint8_t i = 0; i < ENCODER_COUNT; i++) {
    pinMode(ENCODER_A_PINS[i], INPUT_PULLUP);
  }

  for (uint8_t i = BUTTON_MUX_COUNT; i < 9; i++) {
    pinMode(MUX_PINS[i], INPUT_PULLUP);
  }

  for (uint8_t i = 0; i < POT_MUX_COUNT; i++) {
    pinMode(POT_PINS[i], INPUT);
  }

  strip.begin();
  strip.show();
}

void loop() {
  static unsigned long lastPrint = 0;

  if (millis() - lastPrint > DEBUG_PRINT_INTERVAL_MS) {
    lastPrint = millis();
    printLedMapArrays();
    printAllValues();
  }

  if (readMuxDigital(BUTTON_00) == LOW && readMuxDigital(BUTTON_09) == LOW) {
    runA3Special(strip, BUTTON_00, BUTTON_09);
    delay(MAIN_LOOP_DELAY_MS);
    return;
  }

  scanAllMuxInputs();

  for (int led = 0; led < NUMPIXELS; led++) {
    if (!isAssigned(LED_MAP[led])) {
      continue;
    }

    int pressed = readMuxDigital(LED_MAP[led]);
    if (pressed != LOW) {
      continue;
    }

    ledState[led] = !ledState[led];

    if (ledState[led]) {
      strip.setPixelColor(led, strip.Color(0, 0, 255));
    } else {
      strip.setPixelColor(led, 0);
    }

    strip.show();

    while (readMuxDigital(LED_MAP[led]) == LOW) {
      delay(BUTTON_RELEASE_DELAY_MS);
    }
  }

  delay(MAIN_LOOP_DELAY_MS);
}
