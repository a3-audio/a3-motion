#include "usart.h"

void usart_init(unsigned long baud) {
    Serial0.begin(baud);
}

bool usart_readByte(uint8_t *b) {
    if (!Serial0.available()) return false;
    *b = (uint8_t)Serial0.read();
    return true;
}

void usart_write(const uint8_t *data, size_t len) {
    Serial0.write(data, len);
}
