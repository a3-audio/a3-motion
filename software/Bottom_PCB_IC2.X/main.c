#include <avr/io.h>
#include <avr/interrupt.h>
#include "a3_timer.h"
#include "a3_bitoperation.h"

// Your callback function that will be called from the timer interrupt
void my_timer_callback(void) {
    // Toggle LED or perform other time-critical tasks
    PORTA.OUTTGL = PIN0_bm; // Example: toggle PA0
}

int main(void) {
    // Setup I/O
    PORTA.DIRSET = PIN0_bm; // Set PA0 as output
    
    // Initialize timer with your callback function
    a3_timer_init(my_timer_callback);
    
    // Start the timer
    a3_timer_start();
    
    // Enable global interrupts
    sei();
    
    while (1) {
        // Main loop - timer interrupts will call my_timer_callback automatically
        // You can add other non-time-critical code here
    }
}