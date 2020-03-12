/* Name: Juan Paulo Palacios & Andrew Mullen
 * Date: March 12, 2020
 * Course: EGR 226-904
 * Instructor: Dr. Kandalaft
 * Description: This is part I of lab 8.
 * This lab exercise includes setting the duty cycle
 * using global variables and a transistor*/

#include "msp.h"

void Pin_Init(void);
void SysTick_Init(void);                // SysTick Initialization Function
void SysTick_Ms_delay(uint16_t msec);   // SysTick delay in milliseconds
void Motor_On(void);                    // prototype: turns on motor

double Duty_cycle = 0.10;               // variable: sets the percentage of duty cycle in decimal form
double T_period = 50;                   // variable: sets the period of the PWM

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	SysTick_Init();
	Pin_Init();

	while(1){
	    Motor_On();
	}
}

void Motor_On(void){
    P4->OUT |= BIT0;
    SysTick_Ms_delay(T_period * Duty_cycle);               // Set to time on
    P4->OUT &= ~BIT0;
    SysTick_Ms_delay(T_period - (T_period * Duty_cycle));    // Cleared to time off
}

void Pin_Init(){
    P4->SEL0 &= ~BIT0;               // set P4.0 as GPIO for PWM
    P4->SEL1 &= ~BIT0;
    P4->DIR |= BIT0;
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
