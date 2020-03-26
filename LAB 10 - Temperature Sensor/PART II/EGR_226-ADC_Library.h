/* EGR_226-ADC_Library.h
 This is the ADC Library header file,
 modify as needed */

#ifndef EGR_226_ADC_LIBRARY_H_
#define EGR_226_ADC_LIBRARY_H_

#include "msp.h"
#include <stdio.h>

void PIN_INIT(void);
void ADC_INIT(void);
void Convert_Vinput(void);
void SysTick_INIT(void);       // prototype: SysTick initialization with Interupts enabled function
void SysTick_Ms_delay(uint16_t msec);   // prototype: SysTick delay in milliseconds


#endif /* EGR_226_ADC_LIBRARY_H_ */
