/* EGR_226-ADC_Library.c
 This is the ADC Library source file,
 modify as needed */

#include <stdio.h>
#include "EGR_226-ADC_Library.h"

static volatile uint16_t ADC_value;
float voltage = 0;

void PIN_INIT(void){
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
    ADC14->CTL0 |= 1;               // start conversion
    while (!ADC14->IFGR0);
    ADC_value = ADC14->MEM[0];
    voltage = (ADC_value * 3.3) / 16384;
    printf("ADC Value: %d\tVoltage: %g\n", ADC_value, voltage);
    SysTick_Ms_delay(500);
}

/*This function initializes the SysTick Timer*/
void SysTick_INIT(void){
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

