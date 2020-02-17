#include "msp.h"
#include <stdio.h>

void keypad_Init(void);         // prototype: GPIO initialization.
uint8_t Read_Keypad(void);      // prototype: keypad scan
void Print_Keys(void);          // prototype: print key

void SysTick_Init(void);                    // prototype: SysTick initialization
void SysTick_MSdelay(uint16_t delay);       // prototype: SysTick delay

void LED_Init();                            // prototype: LED initialization

uint8_t numb, press;                        // global variable declaration

int main(void){

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	keypad_Init();                                  // call keypad initialize
	LED_Init();

	while(1){
	    SysTick_Init();
	    P2->OUT &= ~0x04;
	    press = Read_Keypad();
	    if(press){
	        P2->OUT |= 0x04;
	        Print_Keys();
	    }
	    SysTick_MSdelay(10);
	}
}

void keypad_Init(void){
    P4->SEL0 &= ~0x7F;
    P4->SEL1 &= ~0x7F;
    P4->DIR &= ~0x7F;
    P4->REN |= 0x7F;
    P4->OUT |= 0x7F; // enable pull resistor for column pins
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
        SysTick_MSdelay(10);            // delay for 10 ms
        row = P4->IN & 0x0F;            // read the rows

        while(!(P4->IN & BIT0) | !(P4->IN & BIT1) | !( P4->IN & BIT2) | !( P4->IN & BIT3) ){}

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

void Print_Keys(void){
    printf("Testing: ");
    if(numb < 10){
        printf("%d\n", numb);
    }else if(numb == 10){
        printf("*\n");
    }else if(numb == 11){
        printf("0\n");
    }else if(numb == 12){
        printf("#\n");
    }
}

/*The following function initializes the SysTick Timer
 * for use of delays within the code */
void SysTick_Init(void){
    SysTick -> CTRL = 0;
    SysTick -> LOAD = 0x00FFFFFF;
    SysTick -> VAL = 0;
    SysTick -> CTRL = 0x00000005;
}

/*The following function uses the SysTick Timer
 * for use of delays within the code at 1 millisecond*/
void SysTick_MSdelay(uint16_t delay){
    SysTick->LOAD = ((delay * 3000) - 1);
    SysTick->VAL = 0;
    while((SysTick->CTRL & 0x00010000) == 0){
    }
}

void LED_Init(){
    P2->SEL0 & ~0x04;
    P2->SEL1 & ~0x04;
    P2->DIR = 0x04;
    P2->OUT &= ~0x04;
}



