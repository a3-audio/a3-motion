#include "potis.h"
#include "multiplexer_map.h"
#include "pot_filter.h"

static PotBank potBank;

static uint16_t read_oversampled(uint8_t index) {
    // All POTIS are DIRECT_INPUT (no MUX): read ADC pin directly.
    uint32_t sum = 0;
    for (uint8_t s = 0; s < POT_OVERSAMPLE_COUNT; s++) {
        sum += (uint16_t)analogRead(POTIS[index].gpio);
    }
    return (uint16_t)(sum / POT_OVERSAMPLE_COUNT);
}

void potis_init() {
    potBank.prime(read_oversampled);
}

void potis_update() {
    potBank.scan(read_oversampled);
}

void potis_read(uint16_t *vals, uint8_t count) {
    potBank.read(vals, count);
}
