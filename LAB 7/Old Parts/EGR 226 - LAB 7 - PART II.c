/*
 * Name: Andrew Mullen, Juan Paulo Palacios
 * Date: February 20, 2020
 * PART II:
 */
#include "msp.h"

void LCD_Init(void);                                        // prototype: LCD Initialization Function
void LCD_Pulse_Enable(void);                                // prototype: Enable pulse pin
void LCD_Push_nibble(uint8_t nibble);                       // prototype: LCD pushes nibbles
void LCD_Push_Byte(uint8_t byte);                           // prototype: pushes bytes
void LCD_Command(uint8_t command);                          // prototype: LCD commands
void LCD_Data(uint8_t data);                                // prototype: LCD data
void LCD_Print_strings(void);                               // prototype: prints out strings

void SysTick_Init(void);                // SysTick Initialization Function
void delay_Ms(uint16_t msec);          // SysTick delay in milliseconds
void delay_micros(uint32_t usec);      // SysTick delay in microseconds

uint8_t nibble;
uint8_t byte;
uint8_t command;                  // global variable
uint8_t data;                     // global variable
uint8_t count = 0;

unsigned char first[] = "ANDREW";
unsigned char second[] = "JUAN";
unsigned char third[] = "EGR";
unsigned char fourth[] = "226";

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    SysTick_Init();
    LCD_Init();

   for(;;){
        LCD_Command(1);
        LCD_Print_strings();    // calls the print strings function
    }
}

/*This function prints out the four strings: Andrew, Juan, EGR, and 226
 * using a for loop and count variable to parse through string*/
void LCD_Print_strings(void){
    int i;

    delay_Ms(500);
    LCD_Command(0x80);
    while(count == 0){
        for(i = 0; first[i] != '\0'; i++){
            LCD_Data(first[i]);
        }
        count++;
    }
    delay_Ms(500);

    delay_Ms(500);
    LCD_Command(0xC0);
    while(count == 1){
        for(i = 0; second[i] != '\0'; i++){
            LCD_Data(second[i]);
        }
        count++;
    }
    delay_Ms(500);

    delay_Ms(500);
    LCD_Command(0x90);
    while(count == 2){
        for(i = 0; third[i] != '\0'; i++){
            LCD_Data(third[i]);
        }
        count++;
    }
    delay_Ms(500);

    delay_Ms(500);
    LCD_Command(0xD0);
    while(count == 3){
        for(i = 0; fourth[i] != '\0'; i++){
            LCD_Data(fourth[i]);
        }
        count = 0;
    }
    delay_Ms(500);
}

/*This function initializes the LCD and writes the commands*/
void LCD_Init(void){
     // Pin setup: LCD Display
    P4->SEL0 = 0x00;
    P4->SEL1 = 0x00;

    P4->DIR |= 0xF5;        // RS OUTPUT, SET TO LOW 0B 1111 0101
    P4->OUT &= ~0XF5;       // RS OUTPUT, SET TO LOW 0B 0000 1010

    delay_Ms(20);
    LCD_Command(3);
    delay_Ms(10);
    LCD_Command(3);
    delay_micros(200);
    LCD_Command(3);
    delay_Ms(100);

    LCD_Command(2);
    delay_micros(100);
    LCD_Command(2);
    delay_micros(100);

    LCD_Command(8);
    delay_micros(100);
    LCD_Command(0x0F);
    delay_micros(100);
    LCD_Command(1);
    delay_micros(100);
    LCD_Command(6);
    delay_micros(10);

}

void LCD_Pulse_Enable(void){
    P4->OUT &= ~BIT2;
    delay_micros(10);
    P4->OUT |= BIT2;
    delay_micros(10);
    P4->OUT &= ~BIT2;
    delay_micros(10);

}

void LCD_Push_nibble(uint8_t nibble){
    P4->OUT &= ~0xF0;
    P4->OUT |= (nibble & 0x0F) << 4;
    LCD_Pulse_Enable();
}

void LCD_Push_Byte(uint8_t byte){
    nibble = (byte & 0xF0) >> 4;
    LCD_Push_nibble(nibble);
    nibble = byte & 0x0F;
    LCD_Push_nibble(nibble);
    delay_micros(100);
}

void LCD_Command(uint8_t command){
    LCD_Push_Byte(0x00);
    delay_micros(100);
}

void LCD_Data(uint8_t data){
    LCD_Push_Byte(0x01);
}


/* DELAY FUNCTIONS BELOW */

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
    { }
}

/*This function is used as a delay in microseconds using SysTick*/
void delay_micros(uint32_t usec){
    SysTick -> LOAD = ((usec * 3) - 1);
    SysTick -> VAL = 0;
    while((SysTick -> CTRL & 0x00010000) == 0)
    { }
}

