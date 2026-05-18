#include "potis.h"
#include "multiplexer_map.h"

void potis_read(uint16_t *vals, uint8_t count) {
    for (uint8_t i = 0; i < count && i < POT_MUX_COUNT; i++) {
        // All POTIS are DIRECT_INPUT (no MUX): read ADC pin directly.
        vals[i] = (uint16_t)analogRead(POTIS[i].gpio);
    }
}
