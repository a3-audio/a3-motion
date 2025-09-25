#include "leds.h"
Led led_arr[19];

void update_leds(Led led, uint8_t green, uint8_t red, uint8_t blue) {
    (green > 0) ? BIT_SET(*led.pGrLed, led.offset_green) : BIT_CLEAR(*led.pGrLed, led.offset_green);
    (red > 0) ? BIT_SET(*led.pLed, led.offset_red) : BIT_CLEAR(*led.pLed, led.offset_red);
    (blue > 0) ? BIT_SET(*led.pLed, led.offset_blue) : BIT_CLEAR(*led.pLed, led.offset_blue);
}

void init_leds(volatile uint8_t *spi_tx_buffer) {
    LED_R_0.pLed = &spi_tx_buffer[0];
    LED_R_0.pGrLed = &spi_tx_buffer[0];
    LED_R_0.offset_green = 2;
    LED_R_0.offset_red = 1;
    LED_R_0.offset_blue = 0;

    LED_00.pLed = &spi_tx_buffer[0];
    LED_00.pGrLed = &spi_tx_buffer[0];
    LED_00.offset_green = 5;
    LED_00.offset_red = 4;
    LED_00.offset_blue = 3;

    LED_01.pLed = &spi_tx_buffer[0];
    LED_01.pGrLed = &spi_tx_buffer[1];
    LED_01.offset_green = 0;
    LED_01.offset_red = 7;
    LED_01.offset_blue = 6;

    LED_02.pLed = &spi_tx_buffer[1];
    LED_02.pGrLed = &spi_tx_buffer[1];
    LED_02.offset_green = 3;
    LED_02.offset_red = 2;
    LED_02.offset_blue = 1;

    LED_03.pLed = &spi_tx_buffer[1];
    LED_03.pGrLed = &spi_tx_buffer[1];
    LED_03.offset_green = 6;
    LED_03.offset_red = 5;
    LED_03.offset_blue = 4;

    LED_R_1.pLed = &spi_tx_buffer[2];
    LED_R_1.pGrLed = &spi_tx_buffer[2];
    LED_R_1.offset_green = 2;
    LED_R_1.offset_red = 1;
    LED_R_1.offset_blue = 0;

    LED_10.pLed = &spi_tx_buffer[2];
    LED_10.pGrLed = &spi_tx_buffer[2];
    LED_10.offset_green = 5;
    LED_10.offset_red = 4;
    LED_10.offset_blue = 3;

    LED_11.pLed = &spi_tx_buffer[2];
    LED_11.pGrLed = &spi_tx_buffer[3];
    LED_11.offset_green = 0;
    LED_11.offset_red = 7;
    LED_11.offset_blue = 6;

    LED_12.pLed = &spi_tx_buffer[3];
    LED_12.pGrLed = &spi_tx_buffer[3];
    LED_12.offset_green = 3;
    LED_12.offset_red = 2;
    LED_12.offset_blue = 1;

    LED_13.pLed = &spi_tx_buffer[3];
    LED_13.pGrLed = &spi_tx_buffer[3];
    LED_13.offset_green = 6;
    LED_13.offset_red = 5;
    LED_13.offset_blue = 4;

    LED_R_2.pLed = &spi_tx_buffer[4];
    LED_R_2.pGrLed = &spi_tx_buffer[4];
    LED_R_2.offset_green = 2;
    LED_R_2.offset_red = 1;
    LED_R_2.offset_blue = 0;

    LED_20.pLed = &spi_tx_buffer[4];
    LED_20.pGrLed = &spi_tx_buffer[4];
    LED_20.offset_green = 5;
    LED_20.offset_red = 4;
    LED_20.offset_blue = 3;

    LED_21.pLed = &spi_tx_buffer[4];
    LED_21.pGrLed = &spi_tx_buffer[5];
    LED_21.offset_green = 0;
    LED_21.offset_red = 7;
    LED_21.offset_blue = 6;

    LED_22.pLed = &spi_tx_buffer[5];
    LED_22.pGrLed = &spi_tx_buffer[5];
    LED_22.offset_green = 3;
    LED_22.offset_red = 2;
    LED_22.offset_blue = 1;

    LED_23.pLed = &spi_tx_buffer[5];
    LED_23.pGrLed = &spi_tx_buffer[5];
    LED_23.offset_green = 6;
    LED_23.offset_red = 5;
    LED_23.offset_blue = 4;

    LED_30.pLed = &spi_tx_buffer[6];
    LED_30.pGrLed = &spi_tx_buffer[6];
    LED_30.offset_green = 2;
    LED_30.offset_red = 1;
    LED_30.offset_blue = 0;

    LED_31.pLed = &spi_tx_buffer[6];
    LED_31.pGrLed = &spi_tx_buffer[6];
    LED_31.offset_green = 5;
    LED_31.offset_red = 4;
    LED_31.offset_blue = 3;

    LED_32.pLed = &spi_tx_buffer[6];
    LED_32.pGrLed = &spi_tx_buffer[7];
    LED_32.offset_green = 0;
    LED_32.offset_red = 7;
    LED_32.offset_blue = 6;

    LED_33.pLed = &spi_tx_buffer[7];
    LED_33.pGrLed = &spi_tx_buffer[7];
    LED_33.offset_green = 3;
    LED_33.offset_red = 2;
    LED_33.offset_blue = 1;


}