/*
 * adc_interrupts.c
 *
 *  Created on: Nov 21, 2024
 *      Author: Kayla Radu
 */


#include  "msp430.h"
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"
#include "strings.h"

unsigned int ADC_Channel;
unsigned int ADC_Left_Detect;
unsigned int ADC_Right_Detect;
unsigned int ADC_thumb;
volatile unsigned char update_display;
volatile unsigned char display_changed;

#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void){
    switch(__even_in_range(ADCIV,ADCIV_ADCIFG)){
    case ADCIV_NONE:
        break;
    case ADCIV_ADCOVIFG: // When a conversion result is written to the ADCMEM0
        // before its previous conversion result was read.
        break;
    case ADCIV_ADCTOVIFG: // ADC conversion-time overflow
        break;
    case ADCIV_ADCHIIFG: // Window comparator interrupt flags
        break;
    case ADCIV_ADCLOIFG: // Window comparator interrupt flag
        break;
    case ADCIV_ADCINIFG: // Window comparator interrupt flag
        break;
    case ADCIV_ADCIFG: // ADCMEM0 memory register with the conversion result

        // replace the two lines of code

/*
        ADC_Thumb = ADCMEM0; // Channel A5
        ADCCTL0 |= ADCSC; // Start next sample
*/

        ADCCTL0 &= ~ADCENC; // Disable ENC bit.
        switch (ADC_Channel++){
        case 0x00: // Channel A2 Interrupt
            ADC_Left_Detect = ADCMEM0; // Move result into Global
            ADC_Left_Detect = (ADC_Left_Detect >> 2) + 110; // Divide the result by 4
            HEXtoBCD(ADC_Left_Detect);
            /*adc_line(4,6);
            display_changed = TRUE;*/
            ADCMCTL0 &= ~ADCINCH_2; // Disable Last channel A2
            ADCMCTL0 |= ADCINCH_3; // Enable Next channel A3
            break;
        case 0x01: // Channel A3 Interrupt
            ADC_Right_Detect = ADCMEM0; // Move result into Global Values
            ADC_Right_Detect = (ADC_Right_Detect >> 2); // Move result into Global
            HEXtoBCD(ADC_Right_Detect);
            /*adc_line(3,6);
            display_changed = TRUE;*/
            ADCMCTL0 &= ~ADCINCH_3; // Disable Last channel A2
            ADCMCTL0 |= ADCINCH_5; // Enable Next channel ?
            break;
        case 0x02:
            ADC_thumb = ADCMEM0; // Move result into Global Values
            ADC_thumb = ADC_thumb >> 2;
            HEXtoBCD(ADC_thumb);
            /*adc_line(2,6);
            display_changed = TRUE;*/
            ADCMCTL0 &= ~ADCINCH_5; // Disable Last channel A?
            ADCMCTL0 |= ADCINCH_2; // Enable First channel 2
            ADC_Channel=0;
            break;
        default:
            break;
        }
        ADCCTL0 |= ADCENC; // Enable Conversions
        break;
        default:




            break;
    }

}

