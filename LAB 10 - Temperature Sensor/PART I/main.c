/* Name: Juan Paulo Palacios
 * Date: March 26, 2020
 * Course: EGR 226-904
 * Instructor: Dr. Kandalaft
 * Description: Lab 10 - Part I
 * This program configures
 * the ADC and tests the input */

#include "msp.h"
#include <stdio.h>
#include "EGR_226-ADC_Library.h"

void main(void){
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	SysTick_INIT();
	PIN_INIT();
	ADC_INIT();

	while(1){
	    Convert_Vinput();
	}
}
