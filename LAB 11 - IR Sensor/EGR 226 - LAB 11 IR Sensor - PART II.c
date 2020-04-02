/* Name: Juan Paulo Palacios
 * Date: April 2, 2020
 * Course: EGR 226-904
 * Instructor: Dr. Kandalaft
 * Description: LAB 11 - PART II
 * This code is used to detect IR */

#include "msp.h"
#include <stdio.h>

#define IRPORT           P2
#define IR_LED           BIT4
#define PHOTO_LED        BIT5
//#define PHOTO_LEDPORT    P5

#define REDPORT          P1
#define RED_LED          BIT0

#define FREQUENCY        37500                     // 10 HZ
#define DUTY_CYCLE       (FREQUENCY / 2)           // 50 % DUTY CYCLE

volatile uint16_t PERIOD;
volatile uint16_t EDGE_N;
volatile uint16_t EDGE_0;
uint8_t count = 0;

void TIMERA_INIT(void);
void IR_LED_INIT(void);
void PHOTO_LED_INIT(void);
void RED_LED_INIT(void);

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    TIMERA_INIT();
    RED_LED_INIT();
    IR_LED_INIT();
    PHOTO_LED_INIT();

    NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31);
    __enable_irq();

    while(1){
        if(count == 1){
            REDPORT->OUT ^= RED_LED;
            printf("Detection success!\nCycles detected: %d\n", PERIOD);
        }else{
            printf("No Detection!\n");
        }
    }


}

void RED_LED_INIT(void){

    REDPORT->SEL1 &=~ RED_LED;       // CONFIG. GPIO -> ONBOARD LED
    REDPORT->SEL0 &=~ RED_LED;
    REDPORT->DIR |= RED_LED;         // P1.0 SET GPIO
    REDPORT->OUT |= RED_LED;         // P1.0 SET GPIO

}

void IR_LED_INIT(void){

    IRPORT->SEL0 |= IR_LED;          // CONFIG. GPIO -> PWM OUTPUT
    IRPORT->SEL1 &=~ IR_LED;
    IRPORT->DIR |= IR_LED;           // P2.4 SET TA0.1

}

void PHOTO_LED_INIT(void){

    IRPORT->SEL0 |= PHOTO_LED;       // CONFIG. GPIO -> PWM OUTPUT
    IRPORT->SEL1 &=~ PHOTO_LED;
    IRPORT->DIR &=~ PHOTO_LED;       // P2.5 SET TA2.1

}

void TIMERA_INIT(void){

    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK |
            TIMER_A_CTL_MC__UP |
            TIMER_A_CTL_CLR |
            TIMER_A_CTL_ID__8;              // SMCLK, UP MODE, CLEAR TIMER, DIV/8

    TIMER_A0->CCR[0] = FREQUENCY;           // 10 HZ
    TIMER_A0->CCR[1] = DUTY_CYCLE;          // 50% DUTY CYCLE*/
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;

    //TIMER_A0->CCTL[2] = TIMER_A_CCTLN_CM_1 |
                        //TIMER_A_CCTLN_CCIS_0 |
                        //TIMER_A_CCTLN_CCIE|
                        //TIMER_A_CCTLN_CAP |
                        //TIMER_A_CCTLN_SCS;

    //TIMER_A2->CTL |= 0x0214 | TIMER_A_CTL_ID__8; //Use SMCLK, Up mode, clear TA0R
    TIMER_A0->CCTL[2] = 0x4910;             // RISING CAPTURE, SYNCH., OUTBIT VAL.,
    //TIMER_A0->EX0 = 0;

}

void TA0_N_IRQHandler(void){

    TIMER_A0->CCTL[2] &=~ BIT0;        // CLEAR INTERUPT FLAG
    EDGE_N = TIMER_A0->CCR[2];
    PERIOD = EDGE_N - EDGE_0;
    EDGE_0 = EDGE_N;

    if((35000 < PERIOD) && (PERIOD < 40000)){
        count = 1;
        REDPORT->OUT ^= RED_LED;
    }

    TIMER_A0->CCTL[2] &=~ TIMER_A_CCTLN_CCIFG;

}
