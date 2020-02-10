//LAB 5 PART III

#include "msp.h"
#include <stdint.h>

// Function prototypes

uint8_t button_press(void);
uint8_t DebounceSwitch1(void);
uint8_t DebounceSwitch2(void);

void RED_LED(void);
void YELLOW_LED(void);
void GREEN_LED(void);

int main(void) {
    static uint8_t count = 0;               // count variable to cycle through colored LED functions

     WDT_A->CTL = WDT_A_CTL_PW |             // Stop WDT
                 WDT_A_CTL_HOLD;

    // BUTTON ONE: GREEN -> YELLOW -> RED
    P4->SEL1 &= ~BIT1;   // set P4.1 as simple I/O
    P4->SEL0 &= ~BIT1;
    P4->DIR &= ~BIT1;    // set P4.1 as output pin
    P4->REN |= BIT1;     // enable internal resistor
    P4->OUT |= BIT1;     // pull up resistor, negative logic

    // BUTTON TWO: RED -> YELLOW -> GREEN
    P4->SEL1 &= ~BIT2;   // set P4.2 as simple I/O
    P4->SEL0 &= ~BIT2;
    P4->DIR &= ~BIT2;    // set P4.2 as output pin
    P4->REN |= BIT2;     // enable internal resistor
    P4->OUT |= BIT2;     // pull up resistor, negative logic

    P3->SEL1 &= ~BIT2;   // set P3.2 as simple I/O GREEN
    P3->SEL0 &= ~BIT2;
    P3->DIR |= BIT2;     // set P3.2 as output pin

    P3->SEL1 &= ~BIT3;   // set P3.3 as simple I/O YELLOW
    P3->SEL0 &= ~BIT3;
    P3->DIR |= BIT3;     // set P3.3 as output pin

    P4->SEL1 &= ~BIT0;   // set P4.0 as simple I/O  RED
    P4->SEL0 &= ~BIT0;
    P4->DIR |= BIT0;     // set P4.0 as output pin

    while(1){
        if(count == 0){
            P3->OUT &= ~BIT2;   // OFF GREEN
            P3->OUT &= ~BIT3;   // OFF YELLOW
            P4->OUT &= ~BIT0;   // OFF RED
            count = 1;
        }

        if(DebounceSwitch1()){
            if(DebounceSwitch1() && count == 1){
                GREEN_LED();
                count++;
            }
            __delay_cycles(3000000);

            if(DebounceSwitch1() && count == 2){
                YELLOW_LED();
                count++;
            }
            __delay_cycles(3000000);

            if(DebounceSwitch1() && count == 3){
                RED_LED();
                count = 1;
            }
            __delay_cycles(3000000);
        }

        if(DebounceSwitch2()){
            if(DebounceSwitch2() && count == 1){
                RED_LED();
                count++;
            }
            __delay_cycles(3000000);

            if(DebounceSwitch2() && count == 2){
                YELLOW_LED();
                count++;
            }
            __delay_cycles(3000000);

            if(DebounceSwitch2() && count == 3){
                GREEN_LED();
                count = 1;
            }
            __delay_cycles(3000000);
        }


    }
}

//uint8_t button_press(void);


uint8_t DebounceSwitch1(void){
    int pin_value = 0;

       if((P4->IN & 0x02) == 0x00){
           __delay_cycles(30000);
           if((P4->IN & 0x02) == 0x00){
               pin_value = 1;           // the button is pressed, changes value to one
           }
       }

       return pin_value;
}

uint8_t DebounceSwitch2(void){
    int pin_value = 0;

       if((P4->IN & 0x04) == 0x00){
           __delay_cycles(30000);
           if((P4->IN & 0x04) == 0x00){
               pin_value = 1;           // the button is pressed, changes value to one
           }
       }

       return pin_value;
}

void GREEN_LED(void){
    P3->OUT ^= BIT2;    // TOGGLES GREEN
    P3->OUT &= ~BIT3;
    P4->OUT &= ~BIT0;
}

void YELLOW_LED(void){
    P3->OUT &= ~BIT2;
    P3->OUT ^= BIT3;   // TOGGLES YELLOW
    P4->OUT &= ~BIT0;
}
void RED_LED(void){
    P3->OUT &= ~BIT2;
    P3->OUT &= ~BIT3;
    P4->OUT ^= BIT0;    // TOGGLES RED
}




/*

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
*/
