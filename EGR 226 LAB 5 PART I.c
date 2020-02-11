/*LAB 5 PART I
 * Name: Juan Paulo Palacios, Andrew Mullen
 * Date: February 10, 2020
 * Course: EGR 226 904
 * Professor: Kandalaft
 * Description: This program toggles a green, yellow, and red LED
 * when the external button is pushed, also uses SysTick Timer
 */

#include "msp.h"
#include <stdint.h>
#define __SYSTEM_CLOCK 3000000

// Function prototypes
void SysTick_Init(void);                 // SysTick Initialization Function
void SysTick_delay(uint16_t delay);         // SysTick Function
uint8_t DebounceSwitch1(void);              // Debounce Switch Function

int main(void) {
    static uint8_t count = 0;               // count variable to cycle through colored LED functions

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;            // Stop WDT

    P4->SEL1 &= ~BIT1;   // set P4.1 as simple I/O
    P4->SEL0 &= ~BIT1;
    P4->DIR &= ~BIT1;    // set P4.1 as output pin
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
            P3->OUT &= ~BIT2;   // turn OFF GREEN LED
            P3->OUT &= ~BIT3;   // turn OFF YELLOW LED
            P4->OUT &= ~BIT0;   // turn OFF RED LED
            count++;            // begins the sequencing of lights
        }

        if(DebounceSwitch1() && count == 1){
            P3->OUT &= ~BIT3;
            P3->OUT ^= BIT2;   // turns ON the GREEN led
            P4->OUT &= ~BIT0;
            count++;           // increments to 2
        }

        if(DebounceSwitch1() && count == 2){
            P3->OUT &= ~BIT2;
            P3->OUT ^= BIT3;   // turns ON the YELLOW LED
            P4->OUT &= ~BIT0;
            count++;           // increments to 3
        }

        if(DebounceSwitch1() && count == 3){
            P3->OUT &= ~BIT2;
            P3->OUT &= ~BIT3;
            P4->OUT ^= BIT0;   // turns ON the RED LED
            count = 1;         // resets to 1
        }
    }
}

uint8_t DebounceSwitch1(void){
    static uint16_t State = 0;
    SysTick_Init();
    State = (State << 1) | ((P4->IN & BIT1) >> 1) | 0xF800;
    SysTick_delay(1);

    if(State == 0xFC00){
        return 1;                              // returns 1 if pressed
    }else{
        return 0;                              // returns 0 if pressed
    }
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
    while((SysTick -> CTRL & 0x00010000) == 0);
}

/*
 * uint8_t DebounceSwitch1(void){
    static uint16_t State = 0;
    SysTick_Init();
    State = (State << 1) | ((P4->IN & BIT1) >> 1) | 0xF800;
    SysTick_delay(10);

    if(State == 0xFC00){
        return 1;                              // returns 1 if pressed
    }else{
        return 0;                              // returns 0 if pressed
    }
}

//Other switch bounce
 * uint8_t DebounceSwitch1(void){
    int pin_value = 0;
    SysTick_Init();
       if((P4->IN & BIT1) == 0x00){
           SysTick_delay(5);
           if((P4->IN & BIT1) == 0x00){
               pin_value = 1;           // the button is pressed, changes value to one
           }
       }
       return pin_value;
    }

*/
