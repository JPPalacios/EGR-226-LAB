/* Name: Juan Paulo Palacios
 * Date: April 2, 2020
 * Course: EGR 226-904
 * Instructor: Dr. Kandalaft
 * Description: LAB 11 - PART I
 * This code is used to detect IR */

#include "msp.h"
#include <stdio.h>

#define IRPORT P2
#define IRPIN BIT4
#define LEDPIN BIT0

#define FREQUENCY 37501                        // 10 HZ
#define DUTY_CYCLE (FREQUENCY * 0.05)           // 50 % DUTY CYCLE

void PORT_INIT(void);
void TIMERA_INIT(void);
void TA0_N_IRQHandler(void);

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	PORT_INIT();
	TIMERA_INIT();

	NVIC_EnableIRQ(TA0_0_IRQn);
	__enable_irq();

	while(1);

}

void PORT_INIT(void){

    IRPORT->SEL0 |= IRPIN;      // CONFIG. GPIO -> PWM OUTPUT
    IRPORT->SEL1 &=~ (IRPIN);
    IRPORT->DIR |= IRPIN;       // P2.4 SET TA0.1

    IRPORT->SEL1 &= ~LEDPIN;    // CONFIG. GPIO
    IRPORT->SEL0 &= ~LEDPIN;
    IRPORT->DIR |= LEDPIN;      // P2.0 SET I/O

}

void TIMERA_INIT(void){

    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK |
                    TIMER_A_CTL_MC__UP |
                    TIMER_A_CTL_CLR |
                    TIMER_A_CTL_ID__8 |
                    TIMER_A_CTL_IE;         // SMCLK, UP MODE, CLEAR TIMER, DIV/8

    TIMER_A0->CCR[0] = FREQUENCY;           // 10 HZ
    TIMER_A0->CCR[1] = DUTY_CYCLE;          // 50% DUTY CYCLE*/
    TIMER_A0->CCTL[1] = 0x4914;             // RISING CAPTURE, SYNCH., OUTBIT VAL.,

    TIMER_A0->CCTL[1] &=~ BIT0;             // CLEAR INTERUPT FLAG
    TIMER_A0->CCTL[1] &=~ BIT1;             // CLEAR OVERFLOW FLAG
}

void TA0_N_IRQHandler(void){

    if(TIMER_A0->CCTL[1] & BIT0){
        TIMER_A0->CCTL[1] &=~ BIT0;        // CLEAR INTERUPT FLAG
        TIMER_A0->CCTL[1] &=~ BIT1;        // CLEAR OVERFLOW FLAG
        IRPORT->OUT ^= LEDPIN;
    }


}
