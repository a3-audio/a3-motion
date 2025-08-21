
#include "main.hpp"
ledEvent event;
volatile uint8_t led_cnt1 = 0;
uint8_t led_cnt2 = 0;
//75us

ISR(TCA0_OVF_vect) {
    TCA0.SINGLE.INTFLAGS |= TCA_SINGLE_OVF_bm; //clear interrupt flag
    led_cnt1++;
    if (led_cnt1 == LED_MAX)
        led_cnt1 = 0;

}

volatile uint16_t test = 0;

//250us

ISR(TCB0_INT_vect) {

    TCB0.INTFLAGS |= TCB_CAPT_bm;
    if (test < 5001)
        test++;
}
int t2=0;
int t3=0;
int main() {
    ccp_write_io((uint8_t *) & (CLKCTRL.MCLKCTRLB), 0); //protected write to disable prescaler of CPU clock
    led_init();
    time_t0Init();
    time_t1Init();
    led_updateRGB(0, 50, 0, 0);
uint16_t dat[4];
            dat[0] = 0b0000000000000000;
            dat[1] = 0b0000000000000000;
            dat[2] = 0b0000000000000000;
            dat[3] = 0b0000000000000000;
    sei();
    while (1) {
        if (test == 5001) {
            cli();
            for(int i=0;i<4;i++)
                dat[i]=0;
            dat[t2/16]=1<<t2%16;
            if(t2>64){
                t3=-1;
            }else if(t2==0){
                t3=1;
            }
                t2+=t3;
                      
            led_shiftSend(dat);
            for (int i = 0; i < 400; i++);
            BIT_SET(LED_SHU_PT.OUT, LED_SHU_BP);
            for (int i = 0; i < 400; i++);
            BIT_CLEAR(LED_SHU_PT.OUT, LED_SHU_BP);
            test=0;
            sei();
        }
    }
    return 0;
}
