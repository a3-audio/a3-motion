/* 
 * File:   leds.h
 * Author: tiri
 *
 * Created on August 29, 2025, 2:06 AM
 */

#ifndef LEDS_H
#define	LEDS_H
#include "bitoperation.h"
#include <stdint.h>

#define LED_R_0 led_arr[0]
#define LED_00 led_arr[1]
#define LED_01 led_arr[2]
#define LED_02 led_arr[3]
#define LED_03 led_arr[4]
#define LED_R_1 led_arr[5]
#define LED_10 led_arr[6]
#define LED_11 led_arr[7]
#define LED_12 led_arr[8]
#define LED_13 led_arr[9]
#define LED_R_2 led_arr[10]
#define LED_20 led_arr[11]
#define LED_21 led_arr[12]
#define LED_22 led_arr[13]
#define LED_23 led_arr[14]
#define LED_30 led_arr[15]
#define LED_31 led_arr[16]
#define LED_32 led_arr[17]
#define LED_33 led_arr[18]
typedef struct Led {
    uint8_t offset_green;
    uint8_t offset_red;
    uint8_t offset_blue;
    volatile uint8_t *pLed;
    volatile uint8_t *pGrLed;

} Led;
extern Led led_arr[19];
void update_leds(Led led, uint8_t green, uint8_t red, uint8_t blue);
void init_leds(volatile uint8_t *spi_tx_buffer);

#endif	/* LEDS_H */

