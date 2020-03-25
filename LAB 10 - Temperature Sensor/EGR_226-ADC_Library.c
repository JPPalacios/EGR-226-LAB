/* EGR_226-ADC_Library.c
 This is the ADC Library source file,
 modify as needed */

#include <stdio.h>
#include <stdint.h>
#include "EGR_226-ADC_Library.h"

int ADC_value = 0;

void ADC_setup(void){
    ADC14->CTL0 = 0x00000010;       // power on, disabled during configuration
    ADC14->CTL0 |= 0x04D80300;      // S/H pulse mode, MCLCK, 32 sample clocks,
                                    // SW trigger, /4 clock divider
    ADC14->CTL1 = 0x00000030;       // 14 bit resolution
    ADC14->MCTL[5] = 0;             // A0 input, single ended, Vref = AVCC

    P5->SEL1 |= 0x20;               // P5.5 configured for AO
    P5->SEL0 |= 0x20;

    ADC14->CTL1 |= 0x00050000;      // convert at memory register 5
    ADC14->CTL0 |= 2;               // Enable ADC after configuration
}

void Convert_Vinput(void){
    float voltage = ADC_value * 0.0002;

    ADC14->CTL0 |= 1;               // start conversion
    while(!ADC14->IFGR0){
        ADC_value = ADC14->MEM[5];
        printf("ADC value:\t%d Voltage:\t%f\n", ADC_value, voltage);
        SysTick_Ms_delay(1000);
    }
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

