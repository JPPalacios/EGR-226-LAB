 /*Name:Juan Paulo Palacios, Andrew Mullen
  *Date: February 18, 2020
  *Course: EGR 226 904
  *Instructor: Dr. Kandalaft
  *Description: PART II
  */

#include "msp.h"
#include <stdio.h>
#include <string.h>

void keypad_Init(void);         // prototype: GPIO initialization.
uint8_t Read_Keypad(void);      // prototype: keypad scan
void Print_Keys(void);          // prototype: print key
void SysTick_Init(void);                    // prototype: SysTick initialization
void SysTick_MSdelay(uint16_t delay);       // prototype: SysTick delay

uint8_t Input_prompt(void);        // prototype: print prompt for user

uint8_t numb, press;            // global variable declaration
unsigned char key;

#define MAX 50                 // macro: array max size


int main(void){
    int i = 0;

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    SysTick_Init();
    keypad_Init();                                  // call keypad initialize

    while(1){
        while(i < 1){
            Input_prompt();
            i++;
        }
    }
}

//Part II functions:


uint8_t Input_prompt(void){
    char pin[MAX];
    int i, flag, count = 0;

    do{
        flag = 0;
        count = 0;
        printf("Please enter your 4 digit PIN: \n");

        for(i = 0; i < MAX; i++){
            press = Read_Keypad();
            Print_Keys();

            if(press){

                if(key != '#' || key != '*'){
                     pin[i] = key;
                     count++;
                     printf("%c \t count: %d \n", key, count);
                 }

                 while(key == '*' || key == '#'){
                    while(count > 4 || key == '#'){
                        flag = 1;
                        printf("Error 808: PIN must be 4 digits!\n");
                        break;
                    }

                    while(count < 4 && key == '*' || key == '#'){
                        flag = 1;
                        printf("Error 404: PIN cannot contain %c!\n", key);
                        break;
                    }
                 }
            }
        }

    }while(flag == 1);

    printf("PIN: %s | \t", pin);
    for(i = count - 4; i < count; i++ ){
        printf("%c ", pin[i]);
    }

                /*else{
                    pin[i] = key;
                    count++;
                    printf("%c \t count: %d \n", key, count);
                }

                if((count <= 4) && (key == '#')){
                    flag = 1;
                    printf("%c \n", key);
                    printf("Error 808: PIN must contain 4 digits!\n");
                    break;
                }

                if(count > 4 && key == '#'){
                    printf("string: %s\n", pin);
                }
            }
          }
    }while(flag == 1);

    printf("PIN: ");
        for(i = strlen(pin) - 4; i < strlen(pin) ; i++){
            printf("%c ", pin[i]);
        }
    printf("\n");*/


    return 0;
}

/* this function converts the numb pressed into char variable used later */
void Print_Keys(void){

    switch(numb){
        case 1:
            key = '1';
            break;
        case 2:
            key = '2';
            break;
        case 3:
            key = '3';
            break;
        case 4:
            key = '4';
            break;
        case 5:
            key = '5';
            break;
        case 6:
            key = '6';
            break;
        case 7:
            key = '7';
            break;
        case 8:
            key = '8';
            break;
        case 9:
            key = '9';
            break;
        case 10:
            key = '*';
            break;
        case 11:
            key = '0';
            break;
        case 12:
            key = '#';
            break;
        default:
            key = '0';
            break;
    }
    /*
    if(numb < 10){
        printf("%d\n", numb);
    }else if(numb == 10){
        printf("*\n");
    }else if(numb == 11){
        printf("0\n");
    }else if(numb == 12){
        printf("#\n");
    }*/

}

//Part I functions:

void keypad_Init(void){
    P4->SEL0 &= ~0x7F;
    P4->SEL1 &= ~0x7F;
    P4->DIR &= ~0x7F;
    P4->REN |= 0x7F;
    P4->OUT |= 0x7F; // enable pull resistor for column pins
}

/*This function reads input from the columns and rows by going through each column
 * and scanning all the inputs in the rows and determining which key is pressed,
 * returns the number if any key is pressed, otherwise, returns 0.*/
uint8_t Read_Keypad(void){
    uint8_t column, row;                // rows: 4 pins, 4.0, 4.1, 4.2, 4.3
                                        // columns: 3 pins, 4.4, 4.5, 4.6
    for(column = 0; column < 3; column++){
        P4->DIR = 0x00;                 // set columns -> inputs
        P4->DIR |= BIT(4 + column);     // set column 3 -> output
        P4->OUT &= ~BIT(4 + column);    // clear column 3 -> low
        SysTick_MSdelay(10);            // delay for 10 ms
        row = P4->IN & 0x0F;            // read the rows

        while(!(P4->IN & BIT0) | !(P4->IN & BIT1) | !( P4->IN & BIT2) | !( P4->IN & BIT3) ){}

        if(row != 0x0F)
            break;     // if one of the input is low, some key is pressed.
    }
    P4->DIR = 0x00;                   // Set Columns to input

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

    return 1;
}



/*The following function initializes the SysTick Timer
 * for use of delays within the code */
void SysTick_Init(void){
    SysTick -> CTRL = 0;
    SysTick -> LOAD = 0x00FFFFFF;
    SysTick -> VAL = 0;
    SysTick -> CTRL = 0x00000005;
}

/*The following function uses the SysTick Timer
 * for use of delays within the code at 1 millisecond*/
void SysTick_MSdelay(uint16_t delay){
    SysTick->LOAD = ((delay * 3000) - 1);
    SysTick->VAL = 0;
    while((SysTick->CTRL & 0x00010000) == 0){
    }
}


/*
 * void Input_prompt(void){
    int pin[MAX];
    int count = 0, flag = 0;

    do{
        for(count = 0; count < MAX; count++){

            press = Read_Keypad();

            if(press && ((numb == 10) || (numb == 12))){
                printf("Error: Please avoid using # and *!\n");
                flag = 1;
            }

            if(press){
                pin[count] = numb;
                SysTick_MSdelay(10);
            }
        }

        if(count < 3){
            printf("Error: Please enter a 4 digit PIN!\n");
            flag = 1;
        }

        printf("pin: %d\n", pin);

    }while(flag == 1);
}
 *
 */
