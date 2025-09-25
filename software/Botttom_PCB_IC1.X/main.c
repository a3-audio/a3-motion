/**
 * @file main.c
 * @author tiri
 * @date 2025-08-19
 * @brief Main function
 */
#include "bitoperation.h"
#include <avr/io.h> // Change to your specific device, e.g., iotn1624.h, iotn1627.h
#include <avr/interrupt.h>
#include <stdbool.h>
#include <avr/fuse.h>
#include <avr/cpufunc.h> /* Required header file */
#include <stdint.h>
#include "leds.h"
#define F_CPU 20000000UL
#define TIMER_FREQ 4 // 1 second in milliseconds
#define PRESCALER 1024


volatile uint8_t spi_tx_buffer[8] = {0};
volatile int8_t spi_tx_index = 7;
volatile bool spi_interrupt_called = false;
volatile bool timer_interrupt_called = false;





void timer_init() {
    // Set Timer/Counter 0 to CTC mode
    TCA0.SINGLE.CTRLA = 0; // Disable the timer
    TCA0.SINGLE.CTRLB = 0; // Disable compare match interrupt
    TCA0.SINGLE.CTRLB |= TCA_SINGLE_CMP0EN_bm; // Enable compare match for channel 0
    TCA0.SINGLE.CTRLA |= TCA_SINGLE_CLKSEL_DIV1024_gc; // Set prescaler to 1024

    // Calculate the value to load into the compare register for 1 second
    // Formula: CMP0 = (F_CPU / (prescaler * frequency)) - 1
    // Here, frequency = 1Hz (1 second)
    TCA0.SINGLE.CMP0 = (F_CPU / (PRESCALER * TIMER_FREQ)) - 1; // Set compare value for 1 second

    // Enable Timer/Counter 0 Compare Match interrupt
    TCA0.SINGLE.INTCTRL |= TCA_SINGLE_CMP0_bm;
}

void timer_init2() {

    // Set the TOP value for the timer
    TCA0.SINGLE.PER = (F_CPU / (PRESCALER * TIMER_FREQ)) - 1; // Set the period register

    // Configure the timer
    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1024_gc; // Set prescaler to 1024
    TCA0.SINGLE.CTRLB = 0; // Disable compare match interrupt
    TCA0.SINGLE.INTCTRL |= TCA_SINGLE_OVF_bm; // Enable overflow interrupt
}

void timer_start() {
    // Start the timer by enabling it
    TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm; // Enable the timer
    timer_interrupt_called = false;
}

void timer_stop() {
    // Stop the timer by disabling it
    TCA0.SINGLE.CTRLA &= ~(TCA_SINGLE_ENABLE_bm); // Disable the timer
    TCA0.SINGLE.CNT = 0; // Reset the counter to zero
}

void spi_init_master(void) {
    // Set PA1 (MOSI) and PA3 (SCK) as outputs
    PORTA.DIRSET = PIN1_bm | PIN3_bm;
    // Set PA5 (pin 15) as output
    PORTC.DIRSET = PIN3_bm;

    // Set SPI0 to master, mode 0, MSB first, prescaler 64
    SPI0.CTRLA = SPI_ENABLE_bm | SPI_MASTER_bm | SPI_PRESC_DIV4_gc;
    SPI0.CTRLB = SPI_BUFEN_bm | SPI_SSD_bm; // SS driven by hardware
}

void togglePin15() {
    // Toggle PA5 high for 1ms
    PORTC.OUTSET = PIN3_bm;
    // Simple delay loop for ~1ms at 20MHz (not precise, for demo)
    for (volatile uint32_t i = 0; i < 400; i++) {
        __asm__ __volatile__("nop");
    }
    PORTC.OUTCLR = PIN3_bm;
}

void spi_transmit_8bytes(uint8_t *data) {
    for (uint8_t i = 0; i < 8; i++) {
        spi_tx_buffer[i] = data[i];
    }
    spi_tx_index = 7;
    // Reset the interrupt flag
    spi_interrupt_called = false;

    SPI0.DATA = spi_tx_buffer[spi_tx_index--];

    // Enable SPI interrupt
    SPI0.INTCTRL = SPI_DREIE_bm;

}

ISR(SPI0_INT_vect) {
    if (SPI0.INTFLAGS & SPI_DREIF_bm) { // Check if Data Register is empty
        if (spi_tx_index >= 0) {
            // Transmit the next byte
            SPI0.DATA = spi_tx_buffer[spi_tx_index--];

        } else {
            // All bytes sent, disable interrupt
            spi_interrupt_called = true;
            SPI0.INTCTRL &= ~SPI_DREIE_bm; // Disable SPI interrupt
        }
    }
}

ISR(TCA0_CMP0_vect) {
    // Timer interrupt service routine
    // Stop the timer
    timer_stop();
    TCA0.SINGLE.CNT = 0; // Reset the timer counter to zero
    timer_interrupt_called = true;
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_CMP0_bm; // Clear the interrupt flag
}

ISR(TCA0_OVF_vect) {
    // Timer interrupt service routine
    // Stop the timer
    timer_stop();
    timer_interrupt_called = true;
    // Clear the OVF interrupt flag
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm; // Write 1 to clear the OVF flag
}


static uint8_t byte_index = 0;
static uint8_t bit_index = 0;
uint8_t demo_i = 0;

int main() {
    ccp_write_io((void *) & (CLKCTRL.MCLKCTRLB), 0); // Disable prescaler of CPU clock
    init_leds(spi_tx_buffer);
    //update_leds(led_arr[0], 1, 0, 0);
    // Initialize SPI in master mode
    spi_init_master();
    // Initialize the timer
    timer_init2();
    PORTC.OUTSET = PIN3_bm; // Clear the output pin

    sei(); // Enable global interrupts
    spi_transmit_8bytes((uint8_t*) spi_tx_buffer); // Start SPI transmission
    timer_start(); // Start the timer
    for (int i = 0; i < 19; i++) {
        update_leds(led_arr[i], 0, 1, 0);
    }
    // Main loop
    while (1) {
        if (spi_interrupt_called) {
            togglePin15();
            spi_interrupt_called = false;
            timer_start();
        }
        if (timer_interrupt_called) {
            timer_interrupt_called = false;
            // Clear the SPI transmit buffer
            //for (uint8_t i = 0; i < 8; i++) spi_tx_buffer[i] = 0;
            // Set the shifting bit
            //spi_tx_buffer[byte_index] = 1 << (7 - bit_index);

            //for(int i=0;i<19;i++){
            //    update_leds(led_arr[i], 0, 0, 0);
            //}
            switch (demo_i) {
                case 0:
                    update_leds(LED_03, 0, 0, 1);
                    update_leds(LED_13, 0, 0, 1);
                    update_leds(LED_23, 0, 0, 1);
                    update_leds(LED_33, 0, 0, 1);
                    break;
                case 1:
                    update_leds(LED_02, 0, 0, 1);
                    update_leds(LED_12, 0, 0, 1);
                    update_leds(LED_22, 0, 0, 1);
                    update_leds(LED_32, 0, 0, 1);
                    break;
                case 2:
                    update_leds(LED_01, 1, 0, 0);
                    update_leds(LED_11, 1, 0, 0);
                    update_leds(LED_21, 1, 0, 0);
                    update_leds(LED_31, 1, 0, 0);
                    break;
                case 3:
                    update_leds(LED_00, 1, 0, 0);
                    update_leds(LED_10, 1, 0, 0);
                    update_leds(LED_20, 1, 0, 0);
                    update_leds(LED_30, 1, 0, 0);
                    break;
                case 4:
                    update_leds(LED_R_0, 0, 1, 0);
                    update_leds(LED_R_1, 0, 1, 0);
                    update_leds(LED_R_2, 0, 1, 0);
                    break;
                case 5:
                    update_leds(LED_R_0, 0, 0, 0);
                    update_leds(LED_R_1, 0, 0, 0);
                    update_leds(LED_R_2, 0, 0, 0);
                    break;
                case 6:
                    update_leds(LED_00, 0, 0, 0);
                    update_leds(LED_10, 0, 0, 0);
                    update_leds(LED_20, 0, 0, 0);
                    update_leds(LED_30, 0, 0, 0);
                    break;
                case 7:
                    update_leds(LED_01, 0, 0, 0);
                    update_leds(LED_11, 0, 0, 0);
                    update_leds(LED_21, 0, 0, 0);
                    update_leds(LED_31, 0, 0, 0);
                    break;
                case 8:
                    update_leds(LED_02, 0, 0, 0);
                    update_leds(LED_12, 0, 0, 0);
                    update_leds(LED_22, 0, 0, 0);
                    update_leds(LED_32, 0, 0, 0);
                    break;
                case 9:
                    update_leds(LED_03, 0, 0, 0);
                    update_leds(LED_13, 0, 0, 0);
                    update_leds(LED_23, 0, 0, 0);
                    update_leds(LED_33, 0, 0, 0);
                    break;


            }
            demo_i++;
            if (demo_i > 9)
                demo_i = 0;
            // Transmit the buffer
            spi_transmit_8bytes((uint8_t*) spi_tx_buffer);
            // Advance the bit index
            bit_index++;
            if (bit_index > 7) {
                bit_index = 0;
                byte_index++;
                if (byte_index > 7) byte_index = 0;
            }

        }
    }
    return 0;
}
