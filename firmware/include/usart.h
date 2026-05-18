#ifndef USART_H
#define USART_H

#include <Arduino.h>
#include <stddef.h>
#include <stdint.h>

// Initialise the USB-CDC Serial port.
// With ARDUINO_USB_MODE=1 the hardware USB peripheral (HWCDC) provides the
// CDC interface at /dev/ttyACM0. For HWCDC the baud parameter is symbolic —
// actual transfer speed is USB Full-Speed (~12 Mbit/s); 450 KB/s+ is fine.
void usart_init(unsigned long baud);

// Non-blocking single-byte reader.
// Returns true and sets *b if a byte is available, false otherwise.
bool usart_readByte(uint8_t *b);

// Write exactly len bytes to the CDC serial port.
void usart_write(const uint8_t *data, size_t len);

#endif // USART_H
