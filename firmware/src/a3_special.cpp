#include "a3_special.h"

namespace {
constexpr unsigned long A3_FRAME_DURATION_MS = 500;
constexpr unsigned long A3_TOTAL_DURATION_MS = 5000;

constexpr bool A_FRAME[LED_MATRIX_A3_ROW_COUNT][LED_MATRIX_A3_COL_COUNT] = {
  { false, false, true,  true,  true,  true,  true,  false, false },
  { false, true,  false, false, false, false, false, true,  false },
  { false, true,  true,  true,  true,  true,  true,  true,  false },
  { false, true,  false, false, false, false, false, true,  false },
  { false, true,  false, false, false, false, false, true,  false }
};

constexpr bool III_FRAME[LED_MATRIX_A3_ROW_COUNT][LED_MATRIX_A3_COL_COUNT] = {
  { false, true,  false, false, true,  false, false, true,  false },
  { false, true,  false, false, true,  false, false, true,  false },
  { false, true,  false, false, true,  false, false, true,  false },
  { false, true,  false, false, true,  false, false, true,  false },
  { false, true,  false, false, true,  false, false, true,  false }
};

void renderFrame(Adafruit_NeoPixel &strip, const bool (&frame)[LED_MATRIX_A3_ROW_COUNT][LED_MATRIX_A3_COL_COUNT]) {
  for (uint8_t row = 0; row < LED_MATRIX_A3_ROW_COUNT; row++) {
    const uint8_t matrixRow = LED_MATRIX_A3_START_ROW + row;
    for (uint8_t col = 0; col < LED_MATRIX_A3_COL_COUNT; col++) {
      const uint8_t matrixCol = LED_MATRIX_A3_START_COL + col;
      const int8_t pixelIndex = LED_MATRIX[matrixRow][matrixCol];
      if (pixelIndex < 0) {
        continue;
      }

      strip.setPixelColor(pixelIndex, frame[row][col] ? strip.Color(0, 0, 255) : 0);
    }
  }

  strip.show();
}

void restorePixels(Adafruit_NeoPixel &strip, const uint32_t (&savedColors)[NUMPIXELS]) {
  for (uint8_t pixel = 0; pixel < NUMPIXELS; pixel++) {
    strip.setPixelColor(pixel, savedColors[pixel]);
  }

  strip.show();
}
}  // namespace

void runA3Special(Adafruit_NeoPixel &strip, const ButtonConfig &button00, const ButtonConfig &button09) {
  uint32_t savedColors[NUMPIXELS];
  for (uint8_t pixel = 0; pixel < NUMPIXELS; pixel++) {
    savedColors[pixel] = strip.getPixelColor(pixel);
  }

  unsigned long startMillis = millis();
  bool showA = true;
  while (millis() - startMillis < A3_TOTAL_DURATION_MS) {
    renderFrame(strip, showA ? A_FRAME : III_FRAME);
    showA = !showA;

    unsigned long frameStart = millis();
    while (millis() - frameStart < A3_FRAME_DURATION_MS) {
      if (readMuxDigital(button00) != LOW && readMuxDigital(button09) != LOW) {
        break;
      }
      delay(BUTTON_RELEASE_DELAY_MS);
    }
  }

  while (readMuxDigital(button00) == LOW || readMuxDigital(button09) == LOW) {
    delay(BUTTON_RELEASE_DELAY_MS);
  }

  restorePixels(strip, savedColors);
}