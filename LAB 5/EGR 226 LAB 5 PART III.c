/*LAB 5 PART III
 * Name: Juan Paulo Palacios, Andrew Mullen
 * Date: February 10, 2020
 * Course: EGR 226 904
 * Professor: Kandalaft
 * Description: This program toggles a green, yellow, and red LED
 * when the external button is pushed and held, can reverse
 * the order of the lights if the second button is pushed. This
 * also uses SysTick Timer
 */

#include "msp.h"
#include <stdint.h>

// Function prototypes
void SysTick_Init(void);                    // SysTick Initialization Function
void SysTick_delay(uint16_t delay);         // SysTick Function
uint8_t DebounceSwitch1(void);
uint8_t DebounceSwitch2(void);

void RED_LED(void);
void YELLOW_LED(void);
void GREEN_LED(void);

int main(void) {
    static uint8_t count = 0;               // count variable to cycle through colored LED functions

     WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;           // Stop WDT

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
        SysTick_Init();
        if(count == 0){
            P3->OUT &= ~BIT2;   // OFF GREEN
            P3->OUT &= ~BIT3;   // OFF YELLOW
            P4->OUT &= ~BIT0;   // OFF RED
            count++;
        }

        if(DebounceSwitch1()){
            if(DebounceSwitch1() && count == 1){
                GREEN_LED();
                count++;
            }
            SysTick_delay(1000);

            if(DebounceSwitch1() && count == 2){
                YELLOW_LED();
                count++;
            }
            SysTick_delay(1000);

            if(DebounceSwitch1() && count == 3){
                RED_LED();
                count = 1;
            }
            SysTick_delay(1000);
        }

        if(DebounceSwitch2()){
            if(DebounceSwitch2() && count == 1){
                RED_LED();
                count++;
            }
            SysTick_delay(1000);

            if(DebounceSwitch2() && count == 2){
                YELLOW_LED();
                count++;
            }
            SysTick_delay(1000);

            if(DebounceSwitch2() && count == 3){
                GREEN_LED();
                count = 1;
            }
            SysTick_delay(1000);
        }
    }
}

uint8_t DebounceSwitch1(void){
    int pin_value = 0;

       if((P4->IN & 0x02) == 0x00){
           SysTick_delay(5);
           if((P4->IN & 0x02) == 0x00){
               pin_value = 1;           // the button is pressed, changes value to one
           }
       }

       return pin_value;
}

uint8_t DebounceSwitch2(void){
    int pin_value = 0;

       if((P4->IN & 0x04) == 0x00){
           SysTick_delay(5);
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

void SysTick_Init(void){
    SysTick -> CTRL = 0;
    SysTick -> LOAD = 0x00FFFFFF;
    SysTick -> VAL = 0;
    SysTick -> CTRL = 0x00000005;
}

void SysTick_delay(uint16_t delay){
    SysTick -> LOAD = ((delay * 3000) - 1);
    SysTick -> VAL = 0;
    while((SysTick -> CTRL & 0x00010000) == 0)
    {}
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
