#include "msp.h"
#include "EGR_226-Final_Project-Libraries.h"
#include <stdio.h>

void MAIN_MENU(void);
void MAIN_SELECTION(void);

void DOOR_MENU(void);
void DOOR_SELECTION(void);

//void MOTOR_FUNCTION(void);
//void LIGHT_FUNCTION(void);


unsigned char main_menu0[] = "MENU";
unsigned char main_menu1[] = "1 DOOR";
unsigned char main_menu2[] = "2 MOTOR";
unsigned char main_menu3[] = "3 LIGHTS";

unsigned char door_menu1[] = "1 OPEN DOOR";
unsigned char door_menu2[] = "2 CLOSE DOOR";

uint8_t option;

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	SYSTICK_INIT();

    LCD_INIT();
    KEYPAD_INIT();

	BUTTON_INIT();
	DOOR_LED_INIT();

	NVIC_EnableIRQ(PORT6_IRQn);
	__enable_interrupt();



	while(1){

	    pressed = KEYPAD_READ();
	    if(pressed){
	        LCD_COMMAND(1);
	        MAIN_MENU();
	        LCD_DELAY(10);
	    }

	}
}

void MAIN_MENU(void){

    int i;

    LCD_COMMAND(0x80);
    for(i = 0; main_menu0[i] != '\0'; i++){
        LCD_DATA(main_menu0[i]);
        LCD_DELAY(50);
    }

    LCD_COMMAND(0xC0);
    for(i = 0; main_menu1[i] != '\0'; i++){
        LCD_DATA(main_menu1[i]);
        LCD_DELAY(50);
    }
/*
    LCD_COMMAND(0x90);
    for(i = 0; main_menu2[i] != '\0'; i++){
        LCD_DATA(main_menu2[i]);
        LCD_DELAY(50);
    }

    LCD_COMMAND(0xC0);
    for(i = 0; main_menu3[i] != '\0'; i++){
        LCD_DATA(main_menu3[i]);
        LCD_DELAY(50);
    }
*/
}

void MAIN_SELECTION(void){

    switch(numb){
    case 1:
        LCD_COMMAND(1);
        DOOR_MENU();
        option = 1;
        while(option == 1){
            //DOOR_SELECTION();
        }
        break;

    case 10:
        LCD_COMMAND(1);
        MAIN_MENU();
        break;

    default:
        LCD_COMMAND(1);
    }
}


void DOOR_MENU(void){

    int i;

    LCD_COMMAND(0x80);
    for(i = 0; door_menu1[i] != '\0'; i++){
        LCD_DATA(door_menu1[i]);
        LCD_DELAY(50);
    }

    LCD_COMMAND(0xC0);
    for(i = 0; door_menu2[i] != '\0'; i++){
        LCD_DATA(door_menu2[i]);
        LCD_DELAY(50);
    }

}

void DOOR_SELECTION(void){

    switch(numb){
    case 1:
        LCD_COMMAND(1);
        printf("OPEN\n");
        option = 1;
        while(option == 1);
        // OPEN DOOR -> SERVO
        // GREEN LED ON
        break;
    case 2:
        LCD_COMMAND(1);
        printf("CLOSE\n");
        option = 2;
        while(option == 2);
        // CLOSE DOOR -> SERVO
        // RED LED ON
        break;
    case 10:
        LCD_COMMAND(1);
        MAIN_MENU();
    default:
        LCD_COMMAND(1);
    }

}

