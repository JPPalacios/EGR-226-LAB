/*
 * Name: Juan Paulo Palacios & Andrew Mullen
 * Date: February 4, 2020
 * Course: EGR 226
 * Description: This code will sequence through the red, green, and blue LEDs
 * on the MSP432 Development Board when switch 1 is pressed. This
 * uses debounce switch functions also
 */

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

    P1->SEL1 &= ~2;   // set P1.1 as simple I/O
    P1->SEL0 &= ~2;
    P1->DIR &= ~2;    // set P1.1 as output pin
    P1->REN |= 2;     // enable internal resistor
    P1->OUT |= 2;     // pull up resistor, negative logic

    P2->SEL1 &= ~7;   // set P2.0, P2.1, P2.2 as simple I/O
    P2->SEL0 &= ~7;
    P2->DIR |= 7;     // set P2.0, P2.1, P2.2 as output pins

    while (1){                               // continuous loop
        if(count == 0){
            P2->OUT &= ~0x07;
            count++;
        }else if(count == 1){     // add initial step for count == 0
            if(DebounceSwitch1()){           // if count is zero and button is pressed
                RED_LED();                   // turn on red LED
                count++;
             }
        }else if(count == 2){
            if(DebounceSwitch1()){           // if count is one and button is pressed
                GREEN_LED();                 // turn on green LED
                count++;
             }
        }else if(count == 3){
            if(DebounceSwitch1()){           // if count is two and button is pressed
                BLUE_LED();                  // turn on blue LED
                count = 1;
            }
        }
        }
}

// This debounce switch function is used to ensure the change of color of LED

uint8_t DebounceSwitch1(void){
    static uint16_t State = 0;

    State = (State << 1) | ((P1->IN & BIT1) >> 1) | 0xF800;
    __delay_cycles(10);

    if(State == 0xFC00){
        return 1;
    }else{
        return 0;
    }
}

/*
 * This function toggles BIT2 and BIT1 and clears BIT
 */

uint8_t RED_LED(void){    // RED
    P2->OUT ^= 0x05;      // XOR P1->IN
    P2->OUT &= ~0x06;
    return 0;
}

uint8_t GREEN_LED(void){      // GREEN
    P2->OUT ^= 0x06;
    P2->OUT &= ~0x05;
    return 0;
}

uint8_t BLUE_LED(void){  // BLUE
    P2->OUT ^= 0x06;
    P2->OUT &= ~0x03;
    return 0;
}

// Draft Code for reference

/* press toggle version
while(1){
    if(P1->IN & 2)
        P2->OUT &= ~1;
    else
        P2->OUT |= 1;
}

// red and green set up
    P2->SEL1 &= ~2;   // set P2.0 as simple I/O
    P2->SEL0 &= ~2;
    P2->DIR |= 2;     // set P2.0 as output pin

    P2->SEL1 &= ~1;   // set P2.1 as simple I/O
    P2->SEL0 &= ~1;
    P2->DIR |= 1;     // set P2.1 as output pin

// toggle six colors
 *
 *     while (1){                               // continuous loop
        if(count == 0){
            if(DebounceSwitch1()){
                P2->OUT ^= BIT0;
                count++;
             }
        }else if(count == 1){
            if(DebounceSwitch1()){
                P2->OUT ^= BIT1;
                count++;
             }
        }else if(count == 2){
            if(DebounceSwitch1()){
                P2->OUT ^= BIT2;
                count = 0;
            }
        }
        }
*/
