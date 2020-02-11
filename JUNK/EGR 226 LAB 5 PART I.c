//LAB 5 PART I

#include "msp.h"
#include <stdint.h>

// Function prototypes

uint8_t DebounceSwitch1(void);
uint8_t RED_LED(void);
uint8_t GREEN_LED(void);
uint8_t BLUE_LED(void);

int main(void) {
    static uint8_t count = 0;               // count variable to cycle through colored LED functions

     WDT_A->CTL = WDT_A_CTL_PW |             // Stop WDT
                 WDT_A_CTL_HOLD;

    P4->SEL1 &= ~BIT1;   // set P1.1 as simple I/O
    P4->SEL0 &= ~BIT1;
    P4->DIR &= ~BIT1;    // set P1.1 as output pin
    P4->REN |= BIT1;     // enable internal resistor
    P4->OUT |= BIT1;     // pull up resistor, negative logic

    P4->SEL1 &= ~BIT0;   // set P4.0 as simple I/O  RED
    P4->SEL0 &= ~BIT0;
    P4->DIR |= BIT0;     // set P4.0 as output pin

    P3->SEL1 &= ~BIT3;   // set P3.3 as simple I/O YELLOW
    P3->SEL0 &= ~BIT3;
    P3->DIR |= BIT3;     // set P3.3 as output pin

    P3->SEL1 &= ~BIT2;   // set P3.2 as simple I/O GREEN
    P3->SEL0 &= ~BIT2;
    P3->DIR |= BIT2;     // set P3.2 as output pin


    while(1){
        if(count == 0){
            P3->OUT &= ~BIT2;
            P3->OUT &= ~BIT3;    // turns off the
            P4->OUT &= ~BIT0;   // turn off red
            count++;
        }

        if(DebounceSwitch1() && count == 1){
            P3->OUT &= ~BIT3;   // turns on the YELLOW LED
            P3->OUT ^= BIT2;   // turns on the Green led
            P4->OUT &= ~BIT0;
            count++;
        }

        if(DebounceSwitch1() && count == 2){
            P3->OUT &= ~BIT2;
            P3->OUT ^= BIT3;   // turns on the YELLOW LED
            P4->OUT &= ~BIT0;
            count++;
        }

        if(DebounceSwitch1() && count == 3){
            P3->OUT &= ~BIT2;
            P3->OUT &= ~BIT3;   // turns on the YELLOW LED
            P4->OUT ^= BIT0;
            count = 1;
        }



        /*
        if((P4->IN & 0x02 && count == 1)){  //pressed turns off
            P3->OUT ^= BIT3;   // turns on the YELLOQ LED
            P4->OUT ^= BIT0;  //
            if(P4->IN & 0x02){
                count = 0;
            }
        }else
            P3->OUT &= ~BIT3;    // turns off the RED LED
            P4->OUT &= ~BIT0;   // turn off red
*/
    }
}



uint8_t DebounceSwitch1(void){
    static uint16_t State = 0;

    State = (State << 1) | ((P4->IN & BIT1) >> 1) | 0xF800;
    __delay_cycles(75);

    if(State == 0xFC00){
        return 1;                              // returns 1 if pressed
    }else{
        return 0;                              // returns 0 if pressed
    }
}
