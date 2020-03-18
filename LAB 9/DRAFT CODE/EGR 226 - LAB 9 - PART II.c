/* Name: Juan Paulo Palacios & Andrew Mullen
 * Date: March 19, 2020
 * Course: EGR 226-904
 * Instructor: Dr. Kandalaft
 * Description: This is part II of lab 9.
 * This lab exercise */

#include "msp.h"

void SysTick_Init(void);                // prototype: SysTick Initialization Function
void SysTick_Ms_delay(uint16_t msec);   // prototype: SysTick delay in milliseconds

void Segment_pinsetup(void);            // prototype: 7 Segment LCD display pin initialization
void Segment_display(void);             // prototype: 7 Segment LCD Display function

const unsigned char digitPattern[] =
    {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	 SysTick_Init();     // initialize: all functions
	 Segment_pinsetup();

	 while(1){
	     Segment_display();
	 }

}

void Segment_display(void){
    P4->OUT = digitPattern[8];
    P5->OUT |= 0x01;
    SysTick_Ms_delay(10);
}

void Segment_pinsetup(void){
    P4->SEL1 &= ~0xFF;      // P4.0-P4.7 configured as GPIO
    P4->SEL1 &= ~0xFF;
    P4->DIR |= 0xFF;        // P4.0-P4.7 set as an output

    P5->SEL1 &= ~0x01;      // P5.0 configured as GPIO
    P5->SEL1 &= ~0x01;
    P5->DIR |= 0x01;        // P5.0 set as an output
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
