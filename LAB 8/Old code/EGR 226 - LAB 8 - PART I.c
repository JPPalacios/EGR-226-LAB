#include "msp.h"

void pin_Init(void);
void SysTick_Init(void);                // SysTick Initialization Function
void delay_Ms(uint16_t msec);          // SysTick delay in milliseconds



void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	pin_Init();

	while(1){
	    P4->OUT = (BIT0 & 0x00);
	}
}

void pin_Init(void){
    P4->SEL1 &= ~BIT0;   // set P4.0 as simple I/O
    P4->SEL0 &= ~BIT0;
    P4->DIR &= ~BIT0;    // set P4.0 as output pin
    P4->REN |= BIT0;     // enable internal resistor
    P4->OUT &= ~BIT0;     // pull down resistor
}

/* SYSTICK FUNCTIONS*/
/*This function initializes the SysTick Timer*/
void SysTick_Init(void){
    SysTick -> CTRL = 0;
    SysTick -> LOAD = 0x00FFFFFF;
    SysTick -> VAL = 0;
    SysTick -> CTRL = 0x00000005;
}

/*This function is used as a delay in milliseconds using SysTick*/
void delay_Ms(uint16_t msec){
    SysTick -> LOAD = ((msec * 3000) - 1);
    SysTick -> VAL = 0;
    while((SysTick -> CTRL & 0x10000) == 0)
    {}
}
