// PART II
/*
 * Name: Juan Paulo Palacios & Andrew Mullen
 * Date: February 4, 2020
 * Course: EGR 226
 * Description: This code will sequence through the red, green, and blue LEDs
 * on the MSP432 Development Board when switch 1 is pressed and held. This
 * uses a debounce switch function also.
 */

#include "msp.h"
#include <stdint.h>

// Function prototypes

uint8_t DebounceSwitch1(void);
uint8_t RED_LED(void);
uint8_t GREEN_LED(void);
uint8_t BLUE_LED(void);

int main(void) {
    static uint8_t count = 0;                          // count variable to cycle through colored LED functions

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

    while(1){

        if(count == 0){
            P2->OUT &= ~0x07;
            count = 1;
        }

        if(DebounceSwitch1()){          //if button is pressed
                if(DebounceSwitch1() && (count == 1)){  //when the button is pressed AND count is 1
                    RED_LED();
                    count = 2;
                }
                __delay_cycles(3000000);    // delay parameter set to one second
                if(DebounceSwitch1() && (count == 2)){
                    GREEN_LED();
                    count = 3;
                }
                __delay_cycles(3000000);
                if(DebounceSwitch1() && (count == 3)){  // if button is pressed and count is pressed
                    BLUE_LED();
                    count = 1;
                }
                __delay_cycles(3000000);
    }
}
}

    // This debounce switch function is used to ensure the change of color of LED
uint8_t DebounceSwitch1(void){
    int pin_value = 0;

       if((P1->IN & 0x02) == 0x00){
           __delay_cycles(30000);
           if((P1->IN & 0x02) == 0x00){
               pin_value = 1;           // the button is pressed, changes value to one
           }
       }

       return pin_value;
    }


uint8_t RED_LED(void){                          // RED LED ON
    P2->OUT ^= 0x05;                            // TOGGLE
    P2->OUT &= ~0x06;                           // CLEAR
    return 0;
}

uint8_t GREEN_LED(void){                        // GREEN LED ON
    P2->OUT ^= 0x06;                            // TOGGLE
    P2->OUT &= ~0x05;                           // CLEAR
    return 0;
}

uint8_t BLUE_LED(void){                         // BLUE LED ON
    P2->OUT ^= 0x06;                            // TOGGLE
    P2->OUT &= ~0x03;                           // CLEAR
    return 0;
}

/*
while (1){                               // continuous loop
    if(count == 0){
        P2->OUT &= ~0x07;
        count++;
    }else if(count == 1){     // add initial step for count == 0
        if(DebounceSwitch1()){           // if count is zero and button is pressed
            RED_LED();                   // turn on red LED
            count++;                     // count increases
         }
    }else if(count == 2){
        if(DebounceSwitch1()){           // if count is one and button is pressed
            GREEN_LED();                 // turn on green LED
            count++;
         }
    }else if(count == 3){
        if(DebounceSwitch1()){           // if count is two and button is pressed
            BLUE_LED();                  // turn on blue LED
            count = 1;                   // count resets to RED LED
        }
    }
    }

uint8_t DebounceSwitch1(void){
   if((P1->IN & 0x02) == 0x00){
       __delay_cycles(30000);
       if((P1->IN & 0x02) == 0x00){
           return 1;
       }else{
           return 0;
       }
   }
}
    */
