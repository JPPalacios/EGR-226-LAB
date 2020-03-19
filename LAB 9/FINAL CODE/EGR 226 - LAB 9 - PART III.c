/* Name: Juan Paulo Palacios & Andrew Mullen
 * Date: March 19, 2020
 * Course: EGR 226-904
 * Instructor: Dr. Kandalaft
 * Description: This is part III of lab 9.
 * This lab exercise uses two buttons to increase
 * or decrease the number on the LCD*/

#include "msp.h"

void SysTick_Init_Interupt(void);       // prototype: SysTick initialization with Interupts enabled function
void SysTick_Handler1(void);             // prototype: SysTick Handler function
void SysTick_Ms_delay(uint16_t msec);   // prototype: SysTick delay in milliseconds

uint8_t DebounceSwitch1(void);          // prototype: Debounce Switch function 1
uint8_t DebounceSwitch2(void);          // prototype: Debounce Switch function 2

void button_Init(void);                 // prototype: push button initialization
void Segment_Init(void);                // prototype: 7 Segment LCD display pin initialization
void Segment_display(void);             // prototype: 7 Segment LCD Display function

volatile uint32_t ms_timeout;
volatile uint32_t count;
uint8_t value = 0;

const unsigned char digitPattern[] =
    {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x80};

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

     SysTick_Init_Interupt();     // initialize: all functions
     __enable_irq();              // enable global interrupts
     Segment_Init();
     button_Init();

     while(1){
         Segment_display();
     }

}

void Segment_display(void){

    if(DebounceSwitch1() == 0 && DebounceSwitch2() == 0){
        while(DebounceSwitch1() == 0 && DebounceSwitch2() == 0){
            P4->OUT = digitPattern[value];
        }
    }

    if(DebounceSwitch1() == 1 && DebounceSwitch2() == 0){
        value++;
        if(value > 9){
            value = 0;
        }
    }

    if(DebounceSwitch1() == 0 && DebounceSwitch2() == 1){
        value--;
        if(value == 0){
            value = 9;
        }
    }

}

void Segment_Init(void){
    P4->SEL1 &= ~0xFF;      // P4.0-P4.7 configured as GPIO
    P4->SEL1 &= ~0xFF;
    P4->DIR |= 0xFF;        // P4.0-P4.7 set as an output
}

void button_Init(){
    // BUTTON ONE: INCREAMENT
    P3->SEL1 &= ~BIT2;   // set P3.2 as simple I/O
    P3->SEL0 &= ~BIT2;
    P3->DIR &= ~BIT2;    // set P3.2 as output pin
    P3->REN |= BIT2;     // enable internal resistor
    P3->OUT |= BIT2;     // pull up resistor, negative logic

    // BUTTON TWO: DECREAMENT
    P3->SEL1 &= ~BIT3;   // set P3.3 as simple I/O
    P3->SEL0 &= ~BIT3;
    P3->DIR &= ~BIT3;    // set P3.3 as output pin
    P3->REN |= BIT3;     // enable internal resistor
    P3->OUT |= BIT3;     // pull up resistor, negative logic

}

uint8_t DebounceSwitch1(void){
    int pin_value = 0;

    if((P3->IN & BIT2) == 0x00){
        SysTick_Ms_delay(50);
        if((P3->IN & BIT2) == 0x00){
            pin_value = 1;           // the button is pressed, changes value to one
        }
    }

    return pin_value;
}

uint8_t DebounceSwitch2(void){
    int pin_value = 0;

    if((P3->IN & BIT3) == 0x00){
        SysTick_Ms_delay(50);
        if((P3->IN & BIT3) == 0x00){
            pin_value = 1;           // the button is pressed, changes value to one
        }
    }

    return pin_value;
}

/* SYSTICK FUNCTIONS*/

/*This function initializes the SysTick Timer*/
void SysTick_Init_Interupt(void){
SysTick -> CTRL = 0;
SysTick -> LOAD = 150000;
SysTick -> VAL = 0;
SysTick -> CTRL = 0x00000007;
}

void SysTick_Handler(void){
    ms_timeout = 1;
    count++;
}

/*This function is used as a delay in milliseconds using SysTick. modified */
void SysTick_Ms_delay(uint16_t msec){
SysTick->LOAD = ((msec * 3000)- 1);
SysTick->VAL = 0;
while((SysTick->CTRL & 0x00010000) == 0);
}

