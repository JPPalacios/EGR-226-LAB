#include "msp.h"

#ifndef EGR_226_FINAL_PROJECT_LIBRARIES_H_
#define EGR_226_FINAL_PROJECT_LIBRARIES_H_



/* ----------------LCD FUNCTIONS----------------------*/

void LCD_INIT(void);
void LCD_PRINT(void);
void LCD_WRITE(unsigned char data, unsigned char control);
void LCD_COMMAND(unsigned char command);
void LCD_DATA(unsigned char data);
void LCD_DELAY(uint8_t loop_delay);

#define RS 1
#define RW 2
#define EN 4
#define LCD_PORT P4

unsigned char data;                     // global variable
unsigned char control;                  // global variable
unsigned char command;

uint8_t count;
uint8_t loop_delay;
uint8_t door_button;


/* ----------------KEYPAD FUNCTIONS----------------------*/


void KEYPAD_INIT(void);         // prototype: GPIO initialization.
void KEYPAD_PRINT(void);          // prototype: print key
uint8_t KEYPAD_READ(void);      // prototype: keypad scan
uint8_t numb, pressed;                        // global variable declaration


/* ----------------BUTTON FUNCTIONS----------------------*/

void BUTTON_INIT(void);
void DOOR_LED_INIT(void);
void PORT6_IRQHandler(void);

uint8_t DEBOUNCE_BUTTON1(void);
uint8_t DEBOUNCE_BUTTON2(void);
//uint8_t DEBOUNCE_BUTTON3(void);

//uint8_t DEBOUNCE_BUTTON4(void);
//uint8_t DEBOUNCE_BUTTON5(void);


/* ----------------SYSTICK FUNCTIONS----------------------*/


void SYSTICK_INIT(void);                    // prototype: SysTick initialization
void SYSTICK_DELAY_MS(uint16_t systick_delay);       // prototype: SysTick delay
uint8_t systick_delay;


/* ----------------TIMERA FUNCTIONS----------------------*/


void PORT_INIT(void);
void TIMERA_INIT(void);
void TA0_N_IRQHandler(void);
#define FREQUENCY 37501                        // 10 HZ
#define DUTY_CYCLE (FREQUENCY * 0.05)           // 50 % DUTY CYCLE


/* ----------------ADC14 FUNCTIONS----------------------*/


void ADC_INIT(void);
void ADC_CONVERT(void);

static volatile uint16_t ADC_value;
float voltage;



#endif /* EGR_226_FINAL_PROJECT_LIBRARIES_H_ */
