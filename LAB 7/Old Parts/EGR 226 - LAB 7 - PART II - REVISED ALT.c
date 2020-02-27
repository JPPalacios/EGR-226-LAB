/*
 * Name: Andrew Mullen, Juan Paulo Palacios
 * Date: February 20, 2020
 *
 */
#include "msp.h"

#define RS 1    // define:
#define RW 2
#define EN 4

void LCD_Init(void);                                              // prototype: LCD Initialization Function
void LCD_nibble_write(unsigned char data, unsigned char control); // prototype: write to LCD
void LCD_Command(unsigned char command);                          // prototype: LCD commands
void LCD_Data(unsigned char data);                                // prototype: LCD data
void LCD_Print_strings(void);

void SysTick_Init(void);                // SysTick Initialization Function
void delay_Ms(uint16_t msec);          // SysTick delay in milliseconds
void delay_micros(uint32_t usec);      // SysTick delay in microseconds

//unsigned char data;                     // global variable
//unsigned char control;                  // global variable
//unsigned char command;                  // global variable

unsigned char first[] = "ANDREW";
unsigned char second[] = "JUAN";
unsigned char third[] = "EGR";
unsigned char fourth[] = "226";

uint8_t count = 0;

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    SysTick_Init();

    LCD_Init();

    for(;;){
        LCD_Command(1);
        LCD_Print_strings();
    }
}

void LCD_Print_strings(void){
    int i;

    delay_Ms(50);
    LCD_Command(0x85);
    while(count == 0){
        for(i = 0; first[i] != '\0'; i++){
            LCD_Data(first[i]);
        }
        count++;
    }
    delay_Ms(50);

    delay_Ms(50);
    LCD_Command(0xC6);
    while(count == 1){
        for(i = 0; second[i] != '\0'; i++){
            LCD_Data(second[i]);
        }
        count++;
    }
    delay_Ms(50);

    delay_Ms(50);
    LCD_Command(0x96);
    while(count == 2){
        for(i = 0; third[i] != '\0'; i++){
            LCD_Data(third[i]);
        }
        count++;
    }
    delay_Ms(50);

    delay_Ms(50);
    LCD_Command(0xD6);
    while(count == 3){
        for(i = 0; fourth[i] != '\0'; i++){
            LCD_Data(fourth[i]);
        }
        count = 0;
    }
    delay_Ms(50);
}

/*This function initializes the LCD and writes the commands*/
void LCD_Init(void){
    P4->DIR = 0xFF;

    //delay 30 ms
    delay_Ms(30);
    LCD_nibble_write(0x30, 0);
    //delay 10 ms
    delay_Ms(10);
    LCD_nibble_write(0x30, 0);
    //delay 1 ms
    delay_Ms(1);
    LCD_nibble_write(0x30, 0);
    //delay 1 ms
    delay_Ms(1);
    LCD_nibble_write(0x20, 0);
    //delay 1 ms
    delay_Ms(1);

    LCD_Command(0x28);
    LCD_Command(0x06);
    LCD_Command(0x01);
    LCD_Command(0x0F);
}

/*This function changes and pushes out nibbles*/
void LCD_nibble_write(unsigned char data, unsigned char control){
    data &= 0xF0;
    control &= 0x0F;
    P4->OUT = data | control;
    P4->OUT = data | control | EN;
    //delay 0 ms
    delay_Ms(0);
    P4->OUT = data;
    P4->OUT = 0;
}

/*This function writes commands to the LCD using the command variable
 * delays 4 ms if command is less than 4, otherwise only delays 1 ms*/
void LCD_Command(unsigned char command){
    LCD_nibble_write(command & 0xF0, 0);
    LCD_nibble_write(command << 4, 0);

    if(command < 4){
        delay_Ms(4);    //delay 4 ms
    }else{
        delay_Ms(1);    //delay 1 ms, 40 us
    }
}

/*This function sends the characters to get "nibbled"*/
void LCD_Data(unsigned char data){
    LCD_nibble_write(data & 0xF0, RS);
    LCD_nibble_write(data << 4, RS);

    //delay 1 ms
    delay_Ms(1);
}

/*This function initializes the SysTick Timer*/
void SysTick_Init(void){
    SysTick->CTRL = 0;
    SysTick->LOAD = 0x00FFFFFF;
    SysTick->VAL = 0;
    SysTick->CTRL = 0x00000005;
}

/*This function is used as a delay in milliseconds using SysTick*/
void delay_Ms(uint16_t msec){
    SysTick->LOAD = ((msec * 3000) - 1);
    SysTick->VAL = 0;

    while((SysTick->CTRL & 0x00010000) == 0)
    {}
    //SysTick -> CTRL = 0;
}

/*This function is used as a delay in microseconds using SysTick*/
void delay_micros(uint32_t usec){
    SysTick->LOAD = ((usec * 3) - 1);
    SysTick->VAL = 0;
    while((SysTick->CTRL & 0x10000) == 0)
    {}
    //SysTick -> CTRL = 0;
}
