#include "msp.h"
#include "EGR_226-Final_Project-Libraries.h"
#include <stdio.h>

/* ----------------LCD FUNCTIONS----------------------*/


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


/* ----------------DOOR FUNCTIONS----------------------*/


void DOOR_LED_BUTTONS_INIT(void){

    P#->SEL0 |= BIT#;       // RED LIGHT KY-009
    P#->SEL1 |= BIT#;
    P#->DIR |= BIT#;
    P#->OUT &=~ BIT#;

    P6->SEL0 &=~ BIT0;  // RED BUTTON
    P6->SEL1 &=~ BIT0;
    P6->DIR &=~ BIT0;
    P6->REN |= BIT0;
    P#->OUT |= BIT#;

    P#->SEL0 |= BIT#;       // GREEN LIGHT KY-009
    P#->SEL1 |= BIT#;
    P#->DIR |= BIT#;
    P#->OUT &=~ BIT#;

    P6->SEL0 &=~ BIT1;   // GREEN BUTTON
    P6->SEL1 &=~ BIT1;
    P6->DIR &=~ BIT1;
    P6->REN |= BIT1;
    P6->OUT |= BIT1;

    // NOTE FOR TOMORROW:
    // SOLDER PINS, FIGURE BUTTON PINS, BUILD LIGHT CIRCUIT, BUILD SERVO CIRCUIT,

}

uint8_t DOOR_DEBOUNCE_BUTTONS(void){

       if((P#->IN & BIT#) == 0x00){     // GREEN PUSH BUTTON PRESSED, OPEN DOOR
           SYSTICK_DELAY_MS(7);
           if((P#->IN & BIT#) == 0x00);
               door_button = 1;
       }

       if((P#->IN & BIT#) == 0x00){     // RED PUSH BUTTON PRESSED, CLOSE DOOR
           SYSTICK_DELAY_MS(7);
           if((P#->IN & BIT#) == 0x00);
               door_button = 2;
       }

       return door_button;
}


/* ----------------MOTOR FUNCTIONS----------------------*/


void MOTOR_TIMERA_INIT(void){

    P5->SEL0 |= BIT6;               // set P5.6 as GPIO for PWM
    P5->SEL1 &= ~BIT6;
    P5->DIR |= BIT6;                // set P5.6 as TA2.1

    TIMER_A2->CCR[0] = 50000 - 1;
    TIMER_A2->CCTL[1] = 0x00E0;
    TIMER_A2->CCR[1] = PWM_value;
    TIMER_A2->CTL = 0x0214;         // SMCLK, UP Mode, CLEAR TAR

}

void MOTOR_DUTY_CYCLE(void){

    int i;

    if(numb == 4){
        Duty_cycle = 0.20;       // sets the duty cycle from 10% to 100%
        MOTOR_TIMERA_INIT();
        sprintf(motor_speed,"%g %%", Duty_cycle * 100);

        LCD_COMMAND(0x96);
        for(i = 0; motor_speed[i] != '\0'; i++){
            LCD_DATA(motor_speed[i]);
            LCD_DELAY(50);
        }
    }
    if(numb == 5){
        Duty_cycle = 0.40;       // sets the duty cycle from 10% to 100%
        MOTOR_TIMERA_INIT();
        sprintf(motor_speed,"%g %%", Duty_cycle * 100);
        LCD_COMMAND(0x96);
        for(i = 0; motor_speed[i] != '\0'; i++){
            LCD_DATA(motor_speed[i]);
            LCD_DELAY(50);
        }
    }
    if(numb == 6){
        Duty_cycle = numb * 0.60;       // sets the duty cycle from 10% to 100%
        MOTOR_TIMERA_INIT();
        sprintf(motor_speed,"%g %%", Duty_cycle * 100);
        LCD_COMMAND(0x97);
        for(i = 0; motor_speed[i] != '\0'; i++){
            LCD_DATA(motor_speed[i]);
            LCD_DELAY(50);
        }
    }
    if(numb == 11){
        Duty_cycle = 0.80;               // sets the duty cycle to 0%
        MOTOR_TIMERA_INIT();
        sprintf(motor_speed,"%g %%", Duty_cycle * 100);
        LCD_COMMAND(0x96);
        for(i = 0; motor_speed[i] != '\0'; i++){
            LCD_DATA(motor_speed[i]);
            LCD_DELAY(50);
        }
    }
    if(numb == 12){
        Duty_cycle = 1.00;               // sets the duty cycle to 0%
        MOTOR_TIMERA_INIT();
        sprintf(motor_speed,"%g %%", Duty_cycle * 100);
        LCD_COMMAND(0x96);
        for(i = 0; motor_speed[i] != '\0'; i++){
            LCD_DATA(motor_speed[i]);
            LCD_DELAY(50);
        }
    }

}

uint8_t MOTOR_DEBOUNCE_BUTTON(void){

    if((P6->IN & BIT6) == 0x00){     // MOTOR PUSH BUTTON PRESSED, STOP MOTOR
        SYSTICK_DELAY_MS(7);
        if((P6->IN & BIT6) == 0x00);
            motor_button = 1;
    }else{
        motor_button = 0;
    }

    return motor_button;
}


/* ----------------LIGHTS FUNCTIONS----------------------*/


void LIGHT_LED_BUTTONS_INIT(void){

    P#->SEL0 |= BIT#;       // RED LIGHT KY-009
    P#->SEL1 |= BIT#;
    P#->DIR |= BIT#;
    P#->OUT |= BIT#;

    P#->SEL0 |= BIT#;       // GREEN LIGHT KY-009
    P#->SEL1 |= BIT#;
    P#->DIR |= BIT#;
    P#->OUT |= BIT#;

    P#->SEL0 |= BIT#;       // BLUE LIGHT KY-009
    P#->SEL1 |= BIT#;
    P#->DIR |= BIT#;
    P#->OUT |= BIT#;

    P6->SEL0 &=~ BIT0;      // RED BUTTON
    P6->SEL1 &=~ BIT0;
    P6->DIR &=~ BIT0;
    P6->REN |= BIT0;
    P6->OUT |= BIT0;

    P6->SEL0 &=~ BIT1;      // GREEN BUTTON
    P6->SEL1 &=~ BIT1;
    P6->DIR &=~ BIT1;
    P6->REN |= BIT1;
    P6->OUT |= BIT1;

    P6->SEL0 &=~ BIT2;      // BLUE BUTTON * RE-PIN
    P6->SEL1 &=~ BIT2;
    P6->DIR &=~ BIT2;
    P6->REN |= BIT2;
    P6->OUT |= BIT2;

}

uint8_t LIGHTS_DEBOUNCE_BUTTONS(void){

    if((P#->IN & BIT#) == 0x00){     // MOTOR PUSH BUTTON PRESSED, STOP MOTOR
        SYSTICK_DELAY_MS(7);
        if((P#->IN & BIT#) == 0x00);
        light_button = 1;
    }else if((P#->IN & BIT#) == 0x00){     // MOTOR PUSH BUTTON PRESSED, STOP MOTOR
        SYSTICK_DELAY_MS(7);
        if((P#->IN & BIT#) == 0x00);
        light_button = 2;
    }else if((P#->IN & BIT#) == 0x00){     // MOTOR PUSH BUTTON PRESSED, STOP MOTOR
        SYSTICK_DELAY_MS(7);
        if((P#->IN & BIT#) == 0x00);
        light_button = 3;
    }else{
        light_button = 0;
    }

    return light_button;

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

