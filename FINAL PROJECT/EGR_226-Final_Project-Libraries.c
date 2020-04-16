#include "msp.h"
#include "EGR_226-Final_Project-Libraries.h"
#include <stdio.h>

/* ----------------LCD FUNCTIONS----------------------*/


void LCD_PRINT(void){
}

/*This function initializes the LCD and writes the commands*/
void LCD_INIT(void){

    LCD_PORT->DIR = 0xFF;

    LCD_DELAY(30);
    LCD_WRITE(0x30, 0);
    LCD_DELAY(10);
    LCD_WRITE(0x30, 0);
    LCD_DELAY(1);
    LCD_WRITE(0x30, 0);
    LCD_DELAY(1);
    LCD_WRITE(0x20, 0);
    LCD_DELAY(1);

    LCD_COMMAND(0x28);
    LCD_COMMAND(0x0F);
    LCD_COMMAND(0x01);
    LCD_COMMAND(0x06);

}

/*This function changes and pushes out nibbles*/
void LCD_WRITE(unsigned char data, unsigned char control){

    data &= 0xF0;
    control &= 0x0F;

    LCD_PORT->OUT = data | control;
    LCD_PORT->OUT = data | control | EN;

    LCD_DELAY(0);
    LCD_PORT->OUT = data;
    LCD_PORT->OUT = 0;

}

/*This function writes commands to the LCD using the command variable
 * delays 4 ms if command is less than 4, otherwise only delays 1 ms*/
void LCD_COMMAND(unsigned char command){

    LCD_WRITE(command & 0xF0, 0);
    LCD_WRITE(command << 4, 0);

    if(command < 4){
        LCD_DELAY(4);    //delay 4 ms
    }else{
        LCD_DELAY(1);    //delay 1 ms, 40 us
    }

}

/*This function sends the characters to get "nibbled"*/
void LCD_DATA(unsigned char data){

    LCD_WRITE(data & 0xF0, RS);
    LCD_WRITE(data << 4, RS);
    LCD_DELAY(1);

}

void LCD_DELAY(uint8_t loop_delay){

    int i = 0, j = 0;

    for(i, j; j < loop_delay; j++);
        for(i = 250; i > 0; i--);

}


/* ----------------KEYPAD FUNCTIONS----------------------*/


void KEYPAD_INIT(void){

    P2->SEL0 &= ~0x0F;
    P2->SEL1 &= ~0x0F;
    P2->DIR &= ~0x0F;
    P2->REN |= 0x0F;
    P2->OUT |= 0x0F; // enable pull resistor for column pins

}

void KEYPAD_PRINT(void){

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

uint8_t KEYPAD_READ(void){

    uint8_t column, row;                // rows: 4 pins, 4.0, 4.1, 4.2, 4.3
                                        // columns: 3 pins, 4.4, 4.5, 4.6
    for(column = 0; column < 3; column++){
        P2->DIR = 0x00;                 // set columns -> inputs
        P2->DIR |= BIT(4 + column);     // set column 3 -> output
        P2->OUT &= ~BIT(4 + column);    // clear column 3 -> low
        SYSTICK_DELAY_MS(10);            // delay for 10 ms
        row = P2->IN & 0x0F;            // read the rows

        while(!(P2->IN & BIT0) | !(P2->IN & BIT1) | !( P2->IN & BIT2) | !( P2->IN & BIT3) ){}

        if(row != 0x0F)
            break;     // if one of the input is low, some key is pressed.
    }

    P2->DIR = 0x00;                   // Set Columns to input

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

    return numb;

}


/* ----------------BUTTON FUNCTIONS----------------------*/

void BUTTON_INIT(void){

    P6->SEL0 &=~ BIT0; //GREEN
    P6->SEL1 &=~ BIT0;
    P6->DIR &=~ BIT0;
    P6->REN |= BIT0;
    P6->OUT |= BIT0;
    P6->IES |= BIT0;
    P6->IE  |= BIT0;

    P6->SEL0 &=~ BIT1; //RED
    P6->SEL1 &=~ BIT1;
    P6->DIR &=~ BIT1;
    P6->REN |= BIT1;
    P6->OUT |= BIT1;
    P6->IES |= BIT1;
    P6->IE  |= BIT1;

    P6->IFG = 0;

}

void PORT6_IRQHandler(void){

    uint8_t status = P6->IFG;

    if(DEBOUNCE_BUTTON1()){
        while((P6->IFG & BIT0) && DEBOUNCE_BUTTON1()){
            P6->OUT ^= BIT4;
            P6->OUT &=~ BIT5;
            P6->IFG = 0;
        }
    }

    if(DEBOUNCE_BUTTON2()){
        while((P6->IFG & BIT1) && DEBOUNCE_BUTTON2()){
            P6->OUT ^= BIT5;
            P6->OUT &=~ BIT4;
            P6->IFG = 0;
        }
    }


}

void DOOR_LED_INIT(void){

    P6->SEL0 &=~ BIT4;  //RED
    P6->SEL1 &=~ BIT4;
    P6->DIR |= BIT4;
    P6->OUT &=~ BIT4;

    P6->SEL0 &=~ BIT5;  //RED
    P6->SEL1 &=~ BIT5;
    P6->DIR |= BIT5;
    P6->OUT &=~ BIT5;
}


uint8_t DEBOUNCE_BUTTON1(void){

       if((P6->IN & BIT4) == 0x00){
           SYSTICK_DELAY_MS(7);
           if((P6->IN & BIT4) == 0x00);
               return 1;
       }

       return 0;
}

uint8_t DEBOUNCE_BUTTON2(void){

       if((P6->IN & BIT5) == 0x00){
           SYSTICK_DELAY_MS(3);
           if((P6->IN & BIT5) == 0x00);
               return 1;
       }

       return 0;
}


/* ----------------SYSTICK FUNCTIONS----------------------*/


/*The following function initializes the SysTick Timer
 * for use of delays within the code */
void SYSTICK_INIT(void){

    SysTick->CTRL = 0;
    SysTick->LOAD = 0x00FFFFFF;
    SysTick->VAL = 0;
    SysTick->CTRL = 0x00000005;

}

/*The following function uses the SysTick Timer
 * for use of delays within the code at 1 millisecond*/
void SYSTICK_DELAY_MS(uint16_t systick_delay){

    SysTick->LOAD = ((systick_delay * 3000) - 1);
    SysTick->VAL = 0;
    while((SysTick->CTRL & 0x00010000) == 0);

}


/* ----------------TIMERA FUNCTIONS----------------------*/


void TIMERA_INIT(void){

    //IRPORT->SEL0 |= IRPIN;      // CONFIG. GPIO -> PWM OUTPUT
    //IRPORT->SEL1 &=~ (IRPIN);
    //IRPORT->DIR |= IRPIN;       // P2.4 SET TA0.1

    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK |
                    TIMER_A_CTL_MC__UP |
                    TIMER_A_CTL_CLR |
                    TIMER_A_CTL_ID__8 |
                    TIMER_A_CTL_IE;         // SMCLK, UP MODE, CLEAR TIMER, DIV/8

    TIMER_A0->CCR[0] = FREQUENCY;           // 10 HZ
    TIMER_A0->CCR[1] = DUTY_CYCLE;          // 50% DUTY CYCLE*/
    TIMER_A0->CCTL[1] = 0x4914;             // RISING CAPTURE, SYNCH., OUTBIT VAL.,

}

void TA0_N_IRQHandler(void){

    if(TIMER_A0->CCTL[1] & BIT0){
        TIMER_A0->CCTL[1] &=~ BIT0;        // CLEAR INTERUPT FLAG
        TIMER_A0->CCTL[1] &=~ BIT1;        // CLEAR OVERFLOW FLAG
        //IRPORT->OUT ^= LEDPIN;
    }

}


/* ----------------ADC14 FUNCTIONS----------------------*/


void ADC_INIT(void){

    //P5->SEL1 |= 0x20;                   // P5.5 configured for AO
    //P5->SEL0 |= 0x20;

    ADC14->CTL0 &=~ ADC14_CTL0_ENC;     // power on, disabled during configuration
    ADC14->CTL0 |= 0x04200210;          // S/H pulse mode, SMCLCK, 16 sample clocks,
    ADC14->CTL1 = 0x00000030;           // 14 bit resolution
    ADC14->CTL1 |= 0x00000000;          // convert at memory register 5
    ADC14->MCTL[0] = 0x00000000;        // A0 input, single ended, Vref = AVCC
    ADC14->CTL0 |= ADC14_CTL0_ENC;      // Enable ADC after configuration

}

void ADC_CONVERT(void){

    ADC14->CTL0 |= 1;               // start conversion
    while (!ADC14->IFGR0);
    ADC_value = ADC14->MEM[0];
    voltage = (ADC_value * 3.3) / 16384;

    //Temp_Celcius = (ADC_value * 0.02) - 50;
    //printf("Voltage: %g\tTemperature: %g\n",voltage, Temp_Celcius);
    SYSTICK_DELAY_MS(500);
}



