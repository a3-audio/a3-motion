/* 
 * File:   a3_timer.h
 * Author: flex
 *
 * Created on September 25, 2025, 11:02 PM
 */

#ifndef A3_TIMER_H
#define	A3_TIMER_H

#include <stdint.h>  // Yes, include it even if common
#include <stddef.h>  // Yes, include it here too
// Function pointer type for the callback
typedef void (*timer_callback_t)(void);

// Timer functions
void a3_timer_init(timer_callback_t callback);
void a3_timer_start(void);
void timer_stop(void);
void timer_set_period(uint16_t period_ms); // Optional: for adjustable period

#endif	/* A3_TIMER_H */

