#include "usart.h"

void usart_init(unsigned long baud) {
    Serial0.begin(baud);
}

bool usart_readByte(uint8_t *b) {
    if (!Serial0.available()) return false;
    *b = (uint8_t)Serial0.read();
    return true;
}

bool usart_readByteWait(uint8_t *b, uint32_t timeoutMs) {
    const uint32_t start = millis();
    while ((millis() - start) < timeoutMs) {
        if (usart_readByte(b)) return true;
        delayMicroseconds(100);
    }
    return false;
}

void usart_write(const uint8_t *data, size_t len) {
    Serial0.write(data, len);
}
