/* Name: Juan Paulo Palacios & Andrew Mullen
 * Date: March 12, 2020
 * Course: EGR 226-904
 * Instructor: Dr. Kandalaft
 * Description: This is part II of lab 8.
 * This lab exercise includes setting the duty cycle
 * using global variables and the use of an optocoupler*/

#include "msp.h"

void SysTick_Init(void);                // prototype: SysTick Initialization Function
void SysTick_Ms_delay(uint16_t msec);   // prototype: SysTick delay in milliseconds

void TimerA0_Init(void);        // prototype: TIMER_A0 initialization
void Keypad_Init(void);         // prototype: GPIO initialization

void PWM_function(void);        // prototype: PWM function
uint8_t Read_Keypad(void);      // prototype: keypad scan
void set_Duty_Cycle(void);      // prototype: modify the duty cycle percent

void Motor_On(void);                    // prototype: turns on motor

double Duty_cycle = 0.10;               // variable: sets the percentage of duty cycle in decimal form
double T_period = 50000;                   // variable: sets the period of the PWM
#define PWM_value (T_period * Duty_cycle);  // variable: sets the PWM value that is read in Timer_A0

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    //SysTick_Init();
    Pin_Init();

    PWM_function();

    while(1){
    }
}

void Motor_On(void){
    SysTick_Ms_delay(1);    // T_on set to 1 msec
    P4->OUT |= BIT0;
    SysTick_Ms_delay(39);   // T cleared
    P4->OUT &= ~BIT0;
}

void PWM_function(void){
    TIMER_A0->CCR[0] = T_period - 1;        //
    TIMER_A0->CCTL[1] = 0xE0;
    TIMER_A0->CCR[1] = PWM_value;
    TIMER_A0->CTL = 0x0214;         // SMCLK, UP Mode, CLEAR TAR
}


void Pin_Init(void){
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
    while((SysTick->CTRL & 0x10000) == 0)
            {}
    SysTick->CTRL = 0;
}
