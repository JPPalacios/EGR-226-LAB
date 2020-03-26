/* Name: Juan Paulo Palacios
 * Date: March 26, 2020
 * Course: EGR 226-904
 * Instructor: Dr. Kandalaft
 * Description: Lab 10 - Part III
 * This program reads and prints
 * the value from the temperature */

#include "msp.h"
#include <stdio.h>
//#include "EGR_226-ADC_Library.h"
//#include "EGR_226-LCD_Library.h"

void PORT_INIT(void);
void ADC_INIT(void);
void Convert_Vinput(void);
void SysTick_Init(void);       // prototype: SysTick initialization with Interupts enabled function
void SysTick_Ms_delay(uint16_t msec);   // prototype: SysTick delay in milliseconds

#define RS 1
#define RW 2
#define EN 4

void LCD_Init(void);                                              // prototype: LCD Initialization Function
void LCD_nibble_write(unsigned char data, unsigned char control); // prototype: write to LCD
void LCD_Command(unsigned char command);                          // prototype: LCD commands
void LCD_Data(unsigned char data);                                // prototype: LCD data
void LCD_Print_strings(void);
void delay_Ms(int delay);          // SysTick delay in milliseconds

unsigned char data;                     // global variable
unsigned char control;                  // global variable
unsigned char command;                  // global variable

uint8_t count = 0;
int delay = 0;

unsigned char first[] = "TEMPERATURE: ";
char temperature[50];

void main(void){
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    SysTick_Init();
    PORT_INIT();
    ADC_INIT();
    LCD_Init();

    while(1){
        Convert_Vinput();
        LCD_Command(1);
        LCD_Print_strings();
    }
}

/*---------ADC FUNCTIONS----------------*/

void PORT_INIT(void){
    P5->SEL1 |= 0x20;                   // P5.5 configured for AO
    P5->SEL0 |= 0x20;
}

void ADC_INIT(void){

    ADC14->CTL0 &=~ ADC14_CTL0_ENC;     // power on, disabled during configuration
    ADC14->CTL0 |= 0x04200210;          // S/H pulse mode, SMCLCK, 16 sample clocks,
    ADC14->CTL1 = 0x00000030;           // 14 bit resolution
    ADC14->CTL1 |= 0x00000000;          // convert at memory register 5
    ADC14->MCTL[0] = 0x00000000;        // A0 input, single ended, Vref = AVCC

    ADC14->CTL0 |= ADC14_CTL0_ENC;      // Enable ADC after configuration
}

void Convert_Vinput(void){
    static volatile uint16_t ADC_value;
    float voltage = 0;
    float Temp_Celcius = 0;

    ADC14->CTL0 |= 1;               // start conversion
    while (!ADC14->IFGR0);
    ADC_value = ADC14->MEM[0];
    voltage = (ADC_value * 3.3) / 16384;
    Temp_Celcius = (ADC_value * 0.02) - 50;
    printf("Voltage:\t%g\tTemperature: \n",voltage);
    sprintf(temperature,"%g",Temp_Celcius);

    printf("%s\n", temperature);
    SysTick_Ms_delay(100);
}

/*This function initializes the SysTick Timer*/
void SysTick_Init(void){
    SysTick -> CTRL = 0;
    SysTick -> LOAD = 0x00FFFFFF;
    SysTick -> VAL = 0;
    SysTick -> CTRL = 0x00000005;
}

/*This function is used as a delay in milliseconds using SysTick. modified */
void SysTick_Ms_delay(uint16_t msec){
SysTick->LOAD = ((msec * 3000)- 1);
SysTick->VAL = 0;
while((SysTick->CTRL & 0x00010000) == 0);
}

/* ----------------LCD FUNCTIONS----------------------*/
void LCD_Print_strings(void){
    int i;

    LCD_Command(0x80);
    for(i = 0; first[i] != '\0'; i++){
        LCD_Data(first[i]);
        delay_Ms(50);
    }

    LCD_Command(0xC6);
    for(i = 0; temperature[i] != '\0'; i++){
        LCD_Data(temperature[i]);
        delay_Ms(50);
    }
    LCD_Command(0xCA);
    LCD_Data(0xDF);
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
    LCD_Command(0x0F);
    LCD_Command(0x01);
    LCD_Command(0x06);
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

void delay_Ms(int delay){
    int i, j;

    for(i = 0, j = 0; j < delay ; j++){
        for(i = 250; i > 0; i--);
    }
}
/*----------------------------------------*/

