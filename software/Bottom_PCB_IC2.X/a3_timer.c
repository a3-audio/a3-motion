#include "a3_timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>

// Static variable to store the callback function
static timer_callback_t user_callback = NULL;

void a3_timer_init(timer_callback_t callback) {
    // Store the user-provided callback function
    user_callback = callback;
    
    // Timer configuration for TCA0 (adjust as needed for your requirements)
    TCA0.SINGLE.CTRLA = 0; // Stop timer first
    TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_NORMAL_gc; // Normal mode
    TCA0.SINGLE.CTRLD = 0; // No split mode
    
    // Set prescaler and enable overflow interrupt
    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV64_gc | TCA_SINGLE_ENABLE_bm;
    TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm; // Overflow interrupt
    
    // Set period (adjust based on your clock frequency)
    // Example: 1ms period with 20MHz clock and DIV64 prescaler
    TCA0.SINGLE.PER = (20000000UL / 64 / 1000) - 1; // 1ms period
}

void a3_timer_start(void) {
    TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
}

void timer_stop(void) {
    TCA0.SINGLE.CTRLA &= ~TCA_SINGLE_ENABLE_bm;
}

void timer_set_period(uint16_t period_ms) {
    // Stop timer, update period, restart
    uint8_t ctrla = TCA0.SINGLE.CTRLA;
    TCA0.SINGLE.CTRLA = 0;
    
    // Calculate period value based on clock and prescaler
    // Assuming 20MHz system clock and DIV64 prescaler
    uint32_t ticks = (20000000UL / 64 * period_ms) / 1000;
    if (ticks > 0xFFFF) ticks = 0xFFFF;
    TCA0.SINGLE.PER = (uint16_t)(ticks - 1);
    
    TCA0.SINGLE.CTRLA = ctrla;
}

// Timer interrupt service routine
ISR(TCA0_OVF_vect) {
    // Clear interrupt flag
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
    
    // Call user callback if it's set
    if (user_callback != NULL) {
        user_callback();
    }
}
