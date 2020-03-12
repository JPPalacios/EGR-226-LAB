/* Name: Juan Paulo Palacios & Andrew Mullen
 * Date: March 12, 2020
 * Course: EGR 226-904
 * Instructor: Dr. Kandalaft
 * Description: This is part III of lab 8.
 * This lab exercise includes setting the duty cycle
 * using keypad input from 0% to 100%*/

#include "msp.h"

void SysTick_Init(void);                // prototype: SysTick Initialization Function
void SysTick_Ms_delay(uint16_t msec);   // prototype: SysTick delay in milliseconds

void TimerA0_Init(void);        // prototype: TIMER_A0 initialization
void Keypad_Init(void);         // prototype: GPIO initialization

void PWM_function(void);        // prototype: PWM function
uint8_t Read_Keypad(void);      // prototype: keypad scan
void set_Duty_Cycle(void);      // prototype: modify the duty cycle percent

uint8_t numb, press;                            // global variable: number entered and state of keypad press
double Duty_cycle = 0.50;                       // global variable: sets the percentage of duty cycle in decimal form
double T_period = 50000.0;                      // global variable: sets the period of the PWM
#define PWM_value (T_period * Duty_cycle);      // global variable: sets the PWM value that is read in Timer_A0

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    SysTick_Init();     // initialize: all functions
    TimerA0_Init();
    Keypad_Init();

    PWM_function();     // sets initial duty cycle

    while(1){
        Read_Keypad();          // input from keypad
        set_Duty_Cycle();       // can vary the duty cycle from keypad input
        SysTick_Ms_delay(10);   // delay 10 ms
    }
}

/*This function controls the duty cycle with the keyboard*/
void set_Duty_Cycle(void){
    if(numb < 11){
        Duty_cycle = numb * 0.10;       // sets the duty cycle from 10% to 100%
        PWM_function();
    }else{
        Duty_cycle = 0.0;               // sets the duty cycle to 0%
        PWM_function();
    }
}

/*This function reads input from the columns and rows by going through each column
 * and scanning all the inputs in the rows and determining which key is pressed,
 * returns the number if any key is pressed, otherwise, returns 0.*/
uint8_t Read_Keypad(void){
    uint8_t column, row;                // rows: 4 pins, 4.0, 4.1, 4.2, 4.3
                                        // columns: 3 pins, 4.4, 4.5, 4.6

    for(column = 0; column < 3; column++){
        P4->DIR = 0x00;                 // set columns -> inputs
        P4->DIR |= BIT(4 + column);     // set column 3 -> output
        P4->OUT &= ~BIT(4 + column);    // clear column 3 -> low

        SysTick_Ms_delay(10);           // delay for 10 ms
        row = P4->IN & 0x0F;            // read the rows

        while(!(P4->IN & BIT0) | !(P4->IN & BIT1) | !( P4->IN & BIT2) | !( P4->IN & BIT3));

        if(row != 0x0F)
            break;     // if one of the input is low, some key is pressed.
    }

    P4->DIR = 0x00;                   // Set Columns to input

    if(column == 3)
        return 0;
    if(row == 0x0E)
        numb = column + 1;                // key in row 0
    if(row == 0x0D)
        numb = 3 + column + 1;            // key in row 1
    if(row == 0x0B)
        numb = 6 + column + 1;            // key in row 2
    if(row == 0x07)
        numb = 9 + column + 1;            // key in row 3

    return 1;
}

/*This function sets CCR1 value for Timer A*/
void PWM_function(void){
    TIMER_A0->CCR[0] = T_period - 1;
    TIMER_A0->CCTL[1] = 0x00E0;
    TIMER_A0->CCR[1] = PWM_value;
    TIMER_A0->CTL = 0x0214;         // SMCLK, UP Mode, CLEAR TAR
                                    //0b 0000 - 00 10 - 00 01 - 0 1 0 0
}

/*This function sets up the pins for the keypad*/
void Keypad_Init(void){
    P4->SEL0 &= ~0x7F;
    P4->SEL1 &= ~0x7F;
    P4->DIR &= ~0x7F;
    P4->REN |= 0x7F;
    P4->OUT |= 0x7F; // enable pull resistor for column pins
}

/*This function sets up the pins for Timer A*/
void TimerA0_Init(void){
    P2->SEL0 |= BIT4;               // set P2.4 as GPIO for PWM
    P2->SEL1 &= ~BIT4;
    P2->DIR |= BIT4;                // set P2.4 as TA0.1
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
