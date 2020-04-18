#include "msp.h"
#include "EGR_226-Final_Project-Libraries.h"
#include <stdio.h>

void MAIN_MENU(void);
void MAIN_SELECTION(void);

void DOOR_MENU(void);
void DOOR_SELECTION(void);



void MOTOR_MENU(void);
void MOTOR_SELECTION(void);



void LIGHTS_MENU(void);
void LIGHTS_SELECTION(void);


unsigned char main_menu0[] = "MAIN MENU";
unsigned char main_menu1[] = "1 - DOOR";
unsigned char main_menu2[] = "2 - MOTOR";
unsigned char main_menu3[] = "3 - LIGHTS";

unsigned char door_menu0[] = "DOOR MENU";
unsigned char door_menu1[] = "B1 - CLOSE DOOR";
unsigned char door_menu2[] = "B2 - OPEN DOOR";
unsigned char door_menu3[] = "* KEY TO RETURN";

unsigned char motor_menu0[] = "MOTOR MENU";
unsigned char motor_menu1[] = "TYPE MOTOR SPEED";
unsigned char motor_menu2[] = " ";
unsigned char motor_menu3[] = "* KEY TO RETURN";

unsigned char light_menu0[] = "LIGHTS MENU";
unsigned char light_menu1[] = "B1 - RED";
unsigned char light_menu2[] = "B2 - GREEN";
unsigned char light_menu3[] = "B3 - BLUE";

uint8_t option;

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	SYSTICK_INIT();

    LCD_INIT();
    KEYPAD_INIT();

    DOOR_LED_BUTTONS_INIT();
    NVIC->ISER[1] = 1 << ((PORT1_IRQn) & 31);
    __enable_interrupt();

    MOTOR_BUTTON_INIT();
	MOTOR_TIMERA_INIT();

	MAIN_MENU();
    KEYPAD_PRINT();

	while(1){

	    pressed = KEYPAD_READ();

	    if(pressed){
	        LCD_COMMAND(1);
	        MAIN_SELECTION();
	        KEYPAD_PRINT();
	        while((option == 1) || (option == 1) || (option == 1) || (option == 1));        // OPTION VAR FOR MENU SELECTION
	            pressed = 0;                                                                // while

	        LCD_DELAY(10);
	    }

	}
}

void MAIN_MENU(void){

    int i;

    LCD_COMMAND(0x83);
    for(i = 0; main_menu0[i] != '\0'; i++){
        LCD_DATA(main_menu0[i]);
        LCD_DELAY(50);
    }

    LCD_COMMAND(0xC0);
    for(i = 0; main_menu1[i] != '\0'; i++){
        LCD_DATA(main_menu1[i]);
        LCD_DELAY(50);
    }

    LCD_COMMAND(0x90);
    for(i = 0; main_menu2[i] != '\0'; i++){
        LCD_DATA(main_menu2[i]);
        LCD_DELAY(50);
    }

    LCD_COMMAND(0xD0);
    for(i = 0; main_menu3[i] != '\0'; i++){
        LCD_DATA(main_menu3[i]);
        LCD_DELAY(50);
    }

}

void MAIN_SELECTION(void){

    switch(numb){
    case 1:
        LCD_COMMAND(1);
        DOOR_MENU();
        LCD_DELAY(10);

        KEYPAD_PRINT();

        option = 1;
        while(option == 1){
            pressed = KEYPAD_READ();

            if(pressed){
                DOOR_SELECTION();
            }

        }
        break;

    case 2:
        LCD_COMMAND(1);
        MOTOR_MENU();

        KEYPAD_PRINT();

        option = 2;
        while(option == 2){
            pressed = KEYPAD_READ();

            if(pressed){
                MOTOR_SELECTION();
            }

        }
        break;

    case 3:
        LCD_COMMAND(1);
        LIGHTS_MENU();
        LCD_DELAY(10);

        KEYPAD_PRINT();

        option = 3;
        while(option == 3){
            pressed = KEYPAD_READ();

            if(pressed){
                LIGHTS_SELECTION();
            }

        }
        break;


    case 10:
        LCD_COMMAND(1);
        MAIN_MENU();
        break;

    default:
        LCD_COMMAND(1);
        MAIN_MENU();

    }
}


void DOOR_MENU(void){

    int i;

    LCD_COMMAND(0x83);
    for(i = 0; door_menu0[i] != '\0'; i++){
        LCD_DATA(door_menu0[i]);
        LCD_DELAY(50);
    }

    LCD_COMMAND(0xC0);
    for(i = 0; door_menu1[i] != '\0'; i++){
        LCD_DATA(door_menu1[i]);
        LCD_DELAY(50);
    }

    LCD_COMMAND(0x90);
    for(i = 0; door_menu2[i] != '\0'; i++){
        LCD_DATA(door_menu2[i]);
        LCD_DELAY(50);
    }

    LCD_COMMAND(0xD0);
    for(i = 0; door_menu3[i] != '\0'; i++){
        LCD_DATA(door_menu3[i]);
        LCD_DELAY(50);
    }


}

void DOOR_SELECTION(void){
// DOES NOT CLOSE DOOR, RED LIGHT REMAINS - DOORF BUTTON CLEAR ?
    if(DOOR_DEBOUNCE_BUTTONS() == 1){
        //P1->OUT |= BIT5;
        //P1->OUT &=~ BIT0;
        //door_button = 0;
        printf("CLOSE\n");
    }

    if(DOOR_DEBOUNCE_BUTTONS() == 2){
        //P1->OUT |= BIT0;
        //P1->OUT &=~ BIT5;
        //door_button = 0;
        printf("OPEN\n");
    }

    if(numb == 10){
        option = 0;
        P1->OUT &=~ BIT0;
        P1->OUT &=~ BIT5;
        // SERVO CLOSES DOOR
        LCD_COMMAND(1);
        MAIN_MENU();
    }

}


void PORT1_IRQHandler(void){

    int status = P1->IFG;

    if((status & BIT6) && (option == 1)){
        P3->OUT |= BIT0;
        P1->OUT &=~ BIT0;
        door_Duty_cycle = .15;
        DOOR_TIMERA_INIT();
    }

    if(status & BIT7 && (option == 1)){
        P1->OUT |= BIT0;
        P3->OUT &=~ BIT0;
        door_Duty_cycle = .05;
        DOOR_TIMERA_INIT();
    }

    if(status & BIT5 && (option == 2)){
        Duty_cycle = 0.00;               // sets the duty cycle to 0%
        MOTOR_TIMERA_INIT();
    }

    P1->IFG = 0;

}


void MOTOR_MENU(void){

    int i;

    LCD_COMMAND(0x83);
    for(i = 0; motor_menu0[i] != '\0'; i++){
        LCD_DATA(motor_menu0[i]);
        LCD_DELAY(50);
    }

    LCD_COMMAND(0xC0);
    for(i = 0; motor_menu1[i] != '\0'; i++){
        LCD_DATA(motor_menu1[i]);
        LCD_DELAY(50);
    }
/*
    LCD_COMMAND(0x96);
    for(i = 0; motor_speed[i] != '\0'; i++){
        LCD_DATA(motor_speed[i]);
        LCD_DELAY(50);
    }
*/
    LCD_COMMAND(0xD0);
    for(i = 0; motor_menu3[i] != '\0'; i++){
        LCD_DATA(motor_menu3[i]);
        LCD_DELAY(50);
    }

}

void MOTOR_SELECTION(void){

    pressed = KEYPAD_READ();
    KEYPAD_PRINT();
    MOTOR_DUTY_CYCLE();
    SYSTICK_DELAY_MS(10);

    if(MOTOR_DEBOUNCE_BUTTON() == 1){
        Duty_cycle = 0.0;
        MOTOR_DUTY_CYCLE();
    }

    if(numb == 10){
        Duty_cycle = 0.0;
        MOTOR_DUTY_CYCLE();
        option = 0;
        LCD_COMMAND(1);
        MAIN_MENU();
    }
}


void LIGHTS_MENU(void){

    int i;

    LCD_COMMAND(0x82);
    for(i = 0; light_menu0[i] != '\0'; i++){
        LCD_DATA(light_menu0[i]);
        LCD_DELAY(50);
    }

    LCD_COMMAND(0xC0);
    for(i = 0; light_menu1[i] != '\0'; i++){
        LCD_DATA(light_menu1[i]);
        LCD_DELAY(50);
    }

    LCD_COMMAND(0x90);
    for(i = 0; light_menu2[i] != '\0'; i++){
        LCD_DATA(light_menu2[i]);
        LCD_DELAY(50);
    }

    LCD_COMMAND(0xD0);
    for(i = 0; light_menu3[i] != '\0'; i++){
        LCD_DATA(light_menu3[i]);
        LCD_DELAY(50);
    }

}

void LIGHTS_SELECTION(void){
 /*   if(LIGHTS_DEBOUNCE_BUTTONS() == 1){
        // RED LIGHT TURNS ON
        // PWM FUNCTION
    }

    if(LIGHTS_DEBOUNCE_BUTTONS() == 2){
        // GREEN LIGHT TURNS ON
        // PWM FUNCTION
    }

    if(LIGHTS_DEBOUNCE_BUTTONS() == 3){
        // BLUE LIGHT TURNS ON
        // PWM FUNCTION
    }

    if(numb == 10){
        option = 0;
        LCD_COMMAND(1);
        MAIN_MENU();
    }
    */
}



