#include "msp.h"

#ifndef EGR_226_FINAL_PROJECT_LIBRARIES_H_
#define EGR_226_FINAL_PROJECT_LIBRARIES_H_


/* ----------------LCD FUNCTIONS----------------------*/


void LCD_INIT(void);
void LCD_WRITE(unsigned char data, unsigned char control);
void LCD_COMMAND(unsigned char command);
void LCD_DATA(unsigned char data);
void LCD_DELAY(uint8_t loop_delay);

unsigned char data;                     // global variable
unsigned char control;                  // global variable
unsigned char command;
uint8_t count;
uint8_t loop_delay;

#define RS 1
#define RW 2
#define EN 4
#define LCD_PORT P4


/* ----------------KEYPAD FUNCTIONS----------------------*/


void KEYPAD_INIT(void);         // prototype: GPIO initialization.
void KEYPAD_PRINT(void);        // prototype: print key
uint8_t KEYPAD_READ(void);      // prototype: keypad scan

uint8_t numb, pressed;          // global variable declaration


/* ----------------BUTTON FUNCTIONS----------------------*/


void PORT6_IRQHandler(void);


/* ----------------DOOR FUNCTIONS----------------------*/


void DOOR_LED_BUTTONS_INIT(void);
uint8_t DOOR_DEBOUNCE_BUTTONS(void);

uint8_t door_button;


/* ----------------MOTOR FUNCTIONS----------------------*/


void MOTOR_TIMERA_INIT(void);
void MOTOR_DUTY_CYCLE(void);
uint8_t MOTOR_DEBOUNCE_BUTTON(void);

char motor_speed[10];
uint8_t motor_button;
double Duty_cycle;

#define PWM_value (50000.0 * Duty_cycle);      // global variable: sets the PWM value that is read in Timer_A0


/* ----------------LIGHTS FUNCTIONS----------------------*/


void LIGHT_LED_BUTTONS_INIT(void);
uint8_t LIGHTS_DEBOUNCE_BUTTONS(void);

uint8_t light_button;


/* ----------------SYSTICK FUNCTIONS----------------------*/


void SYSTICK_INIT(void);                    // prototype: SysTick initialization
void SYSTICK_DELAY_MS(uint16_t systick_delay);       // prototype: SysTick delay
uint8_t systick_delay;


#endif /* EGR_226_FINAL_PROJECT_LIBRARIES_H_ */
