/*
 * Name: Andrew Mullen, Juan Paulo Palacios
 * Date: February 20, 2020
 *
 */


#include "msp.h"
#define RS 1
#define RW 2
#define EN 4

void LCD_Init(void);    // set
void LCD_nibble_write(unsigned char data, unsigned char control); // set
void LCD_Command(unsigned char command);
void LCD_Data(unsigned char data);


//void pushNibble(uint_t nibble);
//void pushByte(uint8_t byte);

void SysTick_Init(void);                 // SysTick Initialization Function
void delay_Ms(uint16_t delay);         // SysTick Function
void delay_micros(uint32_t delay);

unsigned char data;
unsigned char control;
unsigned char command;

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	SysTick_Init();
	LCD_Init();

	for(;;){
	    LCD_Command(1);
	    delay_Ms(500);
	    LCD_Command(0x80);
        LCD_Data('A');
        LCD_Data('N');
        LCD_Data('D');
        LCD_Data('R');
        LCD_Data('E');
        LCD_Data('W');
	    delay_Ms(500);
	}

}

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

void LCD_Command(unsigned char command){
    LCD_nibble_write(command & 0xF0, 0);
    LCD_nibble_write(command << 4, 0);

    if(command < 4)
        //delay 4 ms
        delay_Ms(4);
    else
        //delay 1 ms
        delay_Ms(1);
}

void LCD_Data(unsigned char data){
    LCD_nibble_write(data & 0xF0, RS);
    LCD_nibble_write(data << 4, RS);

    //delay 1 ms
    delay_Ms(1);
}


void SysTick_Init(void){
    SysTick -> CTRL = 0;
    SysTick -> LOAD = 0x00FFFFFF;
    SysTick -> VAL = 0;
    SysTick -> CTRL = 0x00000005;
}



void delay_Ms(uint16_t delay){
    SysTick -> LOAD = ((delay * 3) - 1);
    SysTick -> VAL = 0;
    while((SysTick -> CTRL & 0x00010000) == 0);
}

void delay_micros(uint32_t delay){
    SysTick -> LOAD = ((delay * 3) - 1);
    SysTick -> VAL = 0;
    while((SysTick -> CTRL & 0x00010000) == 0);
}
