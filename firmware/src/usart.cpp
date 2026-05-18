#include "usart.h"

void usart_init(unsigned long baud) {
    Serial.begin(baud);
}

bool usart_readByte(uint8_t *b) {
    if (!Serial.available()) return false;
    *b = (uint8_t)Serial.read();
    return true;
}

void usart_write(const uint8_t *data, size_t len) {
    Serial.write(data, len);
}
