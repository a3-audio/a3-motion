#include <Adafruit_NeoPixel.h>

#include "config.h"

Adafruit_NeoPixel strip(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
bool ledState[NUMPIXELS] = { false };

void selectMuxChannel(uint8_t channel) {
  digitalWrite(SELECT_PINS[0], (channel >> 0) & 1);
  digitalWrite(SELECT_PINS[1], (channel >> 1) & 1);
  digitalWrite(SELECT_PINS[2], (channel >> 2) & 1);
  delayMicroseconds(MUX_SETTLE_DELAY_US);
}

int readButtonMux(uint8_t muxIndex, uint8_t channel) {
  selectMuxChannel(channel);
  return digitalRead(MUX_BUTTON_OUT[muxIndex]);
}

int readAuxMux(uint8_t muxIndex, uint8_t channel) {
  selectMuxChannel(channel);
  return digitalRead(AUX_MUX_OUT[muxIndex]);
}

int readEncoderA(uint8_t encoderIndex) {
  return digitalRead(ENCODER_A_PINS[encoderIndex]);
}

int readEncoderB(uint8_t encoderIndex) {
  const EncoderMuxInput &mapping = ENCODER_MUX_INPUTS[encoderIndex];
  return readAuxMux(mapping.bMux, mapping.bChannel);
}

int readEncoderSwitch(uint8_t encoderIndex) {
  const EncoderMuxInput &mapping = ENCODER_MUX_INPUTS[encoderIndex];
  return readAuxMux(mapping.swMux, mapping.swChannel);
}

int readPotiMux(uint8_t muxIndex, uint8_t channel) {
  selectMuxChannel(channel);
  return analogRead(POT_MUX_OUT[muxIndex]);
}

void printLedMapArrays() {
  Serial.println("\n=== LED_TO_MUX ===");
  Serial.print("{ ");
  for (int i = 0; i < NUMPIXELS; i++) {
    Serial.print(LED_MAP[i].mux);
    if (i < NUMPIXELS - 1) {
      Serial.print(", ");
    }
  }
  Serial.println(" }");

  Serial.println("=== LED_TO_CHANNEL ===");
  Serial.print("{ ");
  for (int i = 0; i < NUMPIXELS; i++) {
    Serial.print(LED_MAP[i].ch);
    if (i < NUMPIXELS - 1) {
      Serial.print(", ");
    }
  }
  Serial.println(" }");
}

void scanAllMuxInputs() {
  for (uint8_t mux = 0; mux < BUTTON_MUX_COUNT; mux++) {
    for (uint8_t ch = 0; ch < MUX_CHANNEL_COUNT; ch++) {
      if (readButtonMux(mux, ch) == LOW) {
        Serial.print("Detected: BUTTON MUX ");
        Serial.print(mux);
        Serial.print(" CH ");
        Serial.println(ch);
      }
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

  for (uint8_t i = 0; i < EXTRA_BUTTON_COUNT; i++) {
    if (readAuxMux(EXTRA_BUTTONS[i].mux, EXTRA_BUTTONS[i].channel) == LOW) {
      Serial.print("Detected: EXTRA BUTTON IDX ");
      Serial.println(i);
    }
  }

  for (uint8_t mux = 0; mux < POT_MUX_COUNT; mux++) {
    for (uint8_t ch = 0; ch < MUX_CHANNEL_COUNT; ch++) {
      int value = readPotiMux(mux, ch);
      if (value < POT_ACTIVE_THRESHOLD) {
        Serial.print("Detected: POTI MUX ");
        Serial.print(mux);
        Serial.print(" CH ");
        Serial.print(ch);
        Serial.print(" ADC=");
        Serial.println(value);
      }
    }
  }
}

void printAllValues() {
  Serial.println("\n=== BUTTON MULTIPLEXER ===");
  for (uint8_t mux = 0; mux < BUTTON_MUX_COUNT; mux++) {
    Serial.print("BTN MUX ");
    Serial.print(mux);
    Serial.print(": [");
    for (uint8_t ch = 0; ch < MUX_CHANNEL_COUNT; ch++) {
      int value = readButtonMux(mux, ch);
      Serial.print(value == LOW ? "1" : "0");
      if (ch < MUX_CHANNEL_COUNT - 1) {
        Serial.print(" ");
      }
    }
    Serial.println("]");
  }

  Serial.println("\n=== ENCODER MULTIPLEXER ===");
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

  Serial.println("\n=== EXTRA BUTTONS ===");
  for (uint8_t i = 0; i < EXTRA_BUTTON_COUNT; i++) {
    int value = readAuxMux(EXTRA_BUTTONS[i].mux, EXTRA_BUTTONS[i].channel);
    Serial.print("EXTRA ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(value == LOW ? "1" : "0");
  }

  Serial.println("\n=== POTI MULTIPLEXER ===");
  for (uint8_t mux = 0; mux < POT_MUX_COUNT; mux++) {
    Serial.print("POT MUX ");
    Serial.print(mux);
    Serial.print(": [");
    for (uint8_t ch = 0; ch < MUX_CHANNEL_COUNT; ch++) {
      int value = readPotiMux(mux, ch);
      Serial.print(value);
      if (ch < MUX_CHANNEL_COUNT - 1) {
        Serial.print(", ");
      }
    }
    Serial.println("]");
  }
}

void setup() {
  Serial.begin(115200);

  for (uint8_t i = 0; i < SELECT_PIN_COUNT; i++) {
    pinMode(SELECT_PINS[i], OUTPUT);
    digitalWrite(SELECT_PINS[i], LOW);
  }

  for (uint8_t i = 0; i < BUTTON_MUX_COUNT; i++) {
    pinMode(MUX_BUTTON_OUT[i], INPUT_PULLUP);
  }

  for (uint8_t i = 0; i < ENCODER_COUNT; i++) {
    pinMode(ENCODER_A_PINS[i], INPUT_PULLUP);
  }

  for (uint8_t i = 0; i < AUX_MUX_COUNT; i++) {
    pinMode(AUX_MUX_OUT[i], INPUT_PULLUP);
  }

  for (uint8_t i = 0; i < POT_MUX_COUNT; i++) {
    pinMode(POT_MUX_OUT[i], INPUT);
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

  scanAllMuxInputs();

  for (int led = 0; led < NUMPIXELS; led++) {
    int8_t mux = LED_MAP[led].mux;
    int8_t channel = LED_MAP[led].ch;

    if (mux < 0 || channel < 0) {
      continue;
    }

    int pressed = readButtonMux(mux, channel);
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

    while (readButtonMux(mux, channel) == LOW) {
      delay(BUTTON_RELEASE_DELAY_MS);
    }
  }

  delay(MAIN_LOOP_DELAY_MS);
}
