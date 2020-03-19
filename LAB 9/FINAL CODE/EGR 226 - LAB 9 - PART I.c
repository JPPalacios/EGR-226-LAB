/* Name: Juan Paulo Palacios & Andrew Mullen
 * Date: March 19, 2020
 * Course: EGR 226-904
 * Instructor: Dr. Kandalaft
 * Description: This is part I of lab 9.
 * This lab exercise includes setting the duty cycle
 * using keypad input from 0% to 100%*/

#include "msp.h"

void SysTick_Init(void);                // prototype: SysTick Initialization Function
void SysTick_Ms_delay(uint16_t msec);   // prototype: SysTick delay in milliseconds

void TimerA0_Init(void);        // prototype: TIMER_A0 initialization
void button_Init(void);                 // prototype: push button initialization
uint8_t DebounceSwitch1(void);          // prototype: Debounce Switch function 1
uint8_t DebounceSwitch2(void);          // prototype: Debounce Switch function 2

void PWM_function(void);        // prototype: PWM function
void set_Duty_Cycle(void);      // prototype: modify the duty cycle percent

uint8_t value = 0;                            // global variable: number entered and state of keypad press
double Duty_cycle = 0.50;                       // global variable: sets the percentage of duty cycle in decimal form
double T_period = 50000.0;                      // global variable: sets the period of the PWM
#define PWM_value (T_period * Duty_cycle);      // global variable: sets the PWM value that is read in Timer_A0



void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    SysTick_Init();     // initialize: all functions
    TimerA0_Init();
    button_Init();

    PWM_function();     // sets initial duty cycle

    while(1){
        set_Duty_Cycle();       // can vary the duty cycle from keypad input
        SysTick_Ms_delay(10);   // delay 10 ms
    }
}

/*This function controls the duty cycle with the buttons*/
void set_Duty_Cycle(void){

    if(DebounceSwitch1() == 0 && DebounceSwitch2() == 0 && DebounceSwitch3() == 0){
        while(DebounceSwitch1() == 0 && DebounceSwitch2() == 0 && DebounceSwitch3() == 0){
            PWM_function();
        }
    }

    if(DebounceSwitch1() == 1 && DebounceSwitch2() == 0 && DebounceSwitch3() == 0){
        value++;
        Duty_cycle = value * 0.10;       // sets the duty cycle from 10% to 100%
        PWM_function();
        if(value > 10){
            value = 0;
        }
    }

    if(DebounceSwitch1() == 0 && DebounceSwitch2() == 1 && DebounceSwitch3() == 0){
        value--;
        Duty_cycle = value * 0.10;       // sets the duty cycle from 10% to 100%
        if(value == 0){
            value = 9;
        }
    }

    if(DebounceSwitch1() == 0 && DebounceSwitch2() == 0 && DebounceSwitch3() == 1){
        Duty_cycle = 0.0;       // sets the duty cycle from 10% to 100%
    }
}

/*This function sets CCR1 value for Timer A*/
void PWM_function(void){
    TIMER_A0->CCR[0] = T_period - 1;
    TIMER_A0->CCTL[1] = 0x00E0;
    TIMER_A0->CCR[1] = PWM_value;
    TIMER_A0->CTL = 0x0214;         // SMCLK, UP Mode, CLEAR TAR
                                    //0b 0000 - 00 10 - 00 01 - 0 1 0 0
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

    // BUTTON TWO: STOP
    P3->SEL1 &= ~BIT0;   // set P4.0 as simple I/O
    P3->SEL0 &= ~BIT0;
    P3->DIR &= ~BIT0;    // set P4.0 as output pin
    P3->REN |= BIT0;     // enable internal resistor
    P3->OUT |= BIT0;     // pull up resistor, negative logic
}

/*This function sets up the pins for Timer A*/
void TimerA0_Init(void){
    P2->SEL0 |= BIT4;               // set P2.4 as GPIO for PWM
    P2->SEL1 &= ~BIT4;
    P2->DIR |= BIT4;                // set P2.4 as TA0.1
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

uint8_t DebounceSwitch3(void){
    int pin_value = 0;

    if((P4->IN & BIT0) == 0x00){
        SysTick_Ms_delay(50);
        if((P4->IN & BIT0) == 0x00){
            pin_value = 1;           // the button is pressed, changes value to one
        }
    }

    return pin_value;
}
    /* SYSTICK FUNCTIONS*/

/*This function initializes the SysTick Timer*/
void SysTick_Init(void){
    SysTick -> CTRL = 0;
    SysTick -> LOAD = 0x00FFFFFF;
    SysTick -> VAL = 0;
    SysTick -> CTRL = 0x00000005;
}

/*This function is used as a delay in milliseconds using SysTick.
 * modified */
void SysTick_Ms_delay(uint16_t msec){
    SysTick->LOAD = ((msec * 3000)- 1);
    SysTick->VAL = 0;
    while((SysTick->CTRL & 0x00010000) == 0);
}
