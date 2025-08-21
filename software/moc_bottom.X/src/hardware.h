/*
 * hardware.h
 *
 * Created: 17/03/2022 13:06:41
 *  Author: tirii
 */ 


#ifndef HARDWARE_H_
#define HARDWARE_H_
#include <avr/io.h>
#define F_CPU 20000000UL

#define BUR1_PT     PORTA	//PA4
#define BUR1__BP	PIN4_bp
#define BUR1__BM	PIN4_bm

#define BUR2_PT     PORTB	//PB2
#define BUR2__BP	PIN2_bp
#define BUR2__BM	PIN2_bm

#define ENC1_A_PT	PORTA	//PA5
#define ENC1_A_BP	PIN5_bp
#define ENC1_A_BM	PIN5_bm

#define ENC1_B_PT	PORTA	//PA6
#define ENC1_B_BP	PIN6_bp
#define ENC1_B_BM	PIN6_bm

#define ENC1_SW_PT	PORTA	//PA7
#define ENC1_SW_BP	PIN7_bp
#define ENC1_SW_BM	PIN7_bm

#define ENC2_A_PT	PORTB	//PB5
#define ENC2_A_BP	PIN5_bp
#define ENC2_A_BM	PIN5_bm

#define ENC2_B_PT	PORTB	//PB4
#define ENC2_B_BP	PIN4_bp
#define ENC2_B_BM	PIN4_bm

#define ENC2_SW_PT	PORTB	//PB3
#define ENC2_SW_BP	PIN3_bp
#define ENC2_SW_BM	PIN3_bm

#define ENC3_SW_PT	PORTA	//PA2
#define ENC3_SW_BP	PIN2_bp
#define ENC3_SW_BM	PIN2_bm

#define ENC4_SW_PT	PORTC	//PC2
#define ENC4_SW_BP	PIN2_bp
#define ENC4_SW_BM	PIN2_bm

#define TWI_SDA_PT	PORTB	//PB1
#define TWI_SDA_BP	PIN1_bp
#define TWI_SDA_BM	PIN1_bm

#define TWI_SCL_PT	PORTB	//PB0
#define TWI_SCL_BP	PIN0_bp
#define TWI_SCL_BM	PIN0_bm

#define LED_SHU_PT	PORTC	//PC3
#define LED_SHU_BP	PIN3_bp
#define LED_SHU_BM	PIN3_bm

#define LED_SHD_PT	PORTA	//PA1
#define LED_SHD_BP	PIN1_bp
#define LED_SHD_BM	PIN1_bm

#define LED_SHC_PT	PORTA	//PA3
#define LED_SHC_BP	PIN3_bp
#define LED_SHC_BM	PIN3_bm

#define POS_LEDS1B	0    // 1
#define POS_LEDS1R	1    // 2
#define POS_LEDS1G	2    // 3
#define POS_LEDS2B	3    // 4
#define POS_LEDS2R	4    // 5
#define POS_LEDS2G	5    // 6
#define POS_LEDS3B	6    // 7
#define POS_LEDS3R	7    // 8
#define POS_LEDS3G	8    // 9
#define POS_LEDS4B	9    // 10
#define POS_LEDS4R	10   // 11
#define POS_LEDS4G	11   // 12
#define POS_LEDS5B	12   // 13
#define POS_LEDS5R	13   // 14
#define POS_LEDS5G	14   // 15
#define POS_LEDS6B	16   // 16
#define POS_LEDS6R	17   // 17
#define POS_LEDS6G	18   // 18
#define POS_LEDS7B	19   // 19
#define POS_LEDS7R	20   // 20
#define POS_LEDS7G	21   // 21
#define POS_LEDS8B	22   // 22
#define POS_LEDS8R	23   // 23
#define POS_LEDS8G	24   // 24
#define POS_LEDS9B	25   // 25
#define POS_LEDS9R	26   // 26
#define POS_LEDS9G	27   // 27
#define POS_LEDS10B	28   // 28
#define POS_LEDS10R	29   // 29
#define POS_LEDS10G 30   // 30
#define POS_LEDS11B	32   // 31
#define POS_LEDS11R	33   // 32
#define POS_LEDS11G	34   // 33
#define POS_LEDS12B	35   // 34
#define POS_LEDS12R	36   // 35
#define POS_LEDS12G	37   // 36
#define POS_LEDS13B	38   // 37
#define POS_LEDS13R	39   // 38
#define POS_LEDS13G	40   // 39
#define POS_LEDS14B	41   // 40
#define POS_LEDS14R	42   // 41
#define POS_LEDS14G	43   // 42
#define POS_LEDS15B	44   // 43
#define POS_LEDS15R	45   // 44
#define POS_LEDS15G	46   // 45
#define POS_LEDS16B	48   // 46
#define POS_LEDS16R	49   // 47
#define POS_LEDS16G	50   // 48
#define POS_LEDS17B	51   // 49
#define POS_LEDS17R	52   // 50
#define POS_LEDS17G	53   // 51
#define POS_LEDS18B	54   // 52
#define POS_LEDS18R	55   // 53
#define POS_LEDS18G	56   // 54
#define POS_LEDS19B	57   // 55
#define POS_LEDS19R	58   // 56
#define POS_LEDS19G	59   // 57







#endif /* HARDWARE_H_ */
