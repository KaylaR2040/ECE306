/*
 * Ports_Interrupts.c
 *
 *  Created on: Nov 21, 2024
 *      Author: Kayla Radu
 */

//------------------------------------------------------------------------------

#include  "msp430.h"
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"
#include  "strings.h"
#include "switches.h"
#include "wheels.h"
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//variable declarations
unsigned int display_count;
volatile unsigned char display_changed;
extern char display_line[4][11];
volatile unsigned char update_display;
volatile char one_time;
volatile unsigned int Time_Sequence;

unsigned int event;
unsigned int state;
unsigned int t=0;
extern unsigned int clear_display;
extern char NCSU_str[10] = "NCSU  #1";
char display_line[4][11];
unsigned int baude_toggle;
char transmit_state;
extern volatile unsigned int switchpressed = OFF;
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Port 4 interrupt for switch 1, it is disabled for the duration
// of the debounce time. Debounce time is set for 1 second
#pragma vector=PORT4_VECTOR
__interrupt void switch1_interrupt(void) {
// Switch 1
    if (P4IFG & SW1) {
        P4IE &= ~SW1;//Disable Switch 1
        P4IFG &= ~SW1;//Clear Switch 1 Flag
        TB0CCTL1 &= ~CCIFG; //Clear TimerB0 Interrupt Flag for Capture Compare Register 1
        TB0CCR1 = TB0R + TB0CCR1_INTERVAL;//Add Interval to TB0R for TB0CCR1
        TB0CCTL1 |= CCIE;//Enable TimerB0_1
        //P6OUT &= ~LCD_BACKLITE;
        TB0CCTL0 &= ~CCIE;
//        P2OUT &= ~IR_LED; // Initial Value = Low / Off
//        P2DIR |= IR_LED; // Direction = input

//        transmit_state = TRANSMIT;
//        switchpressed = ON;
//                state = WAIT;
        // allow = 1;


        clear_display = 1;
        USCI_A0_transmit();
//        while(NCSU_str[t] != '\0'){
//            UCA0_transmit(NCSU_str[t]);
//            t++;
//        }
        //            UCA0_transmit(NCSU_str[t]);
//        t = 0;


    }
//------------------------------------------------------------------------------
}
//------------------------------------------------------------------------------
// Port 2 interrupt for switch 2, it is disabled for the duration
// of the debounce time. Debounce time is set for 1 second
#pragma vector=PORT2_VECTOR
__interrupt void switch2_interrupt(void) {
// Switch 2
    if (P2IFG & SW2) {
        P2IE &= ~SW2;//Disable Switch 1


        clear_display = 1;
//      if(pressed){
//             strcpy(display_line[0], "BLACK CAL ");
//                         HexToBCD(Left);
//                         adc_line(2,2);
//                         HexToBCD(Right);
//                         adc_line(3,3);
// //            strcpy(display_line[1], "SPINNING ");
//         }
//         if(pressed == 2){
//             strcpy(display_line[0], " WHITE DET");
//             HexToBCD(Left+10);
//             adc_line(2,2);
//             HexToBCD(Right+20);
//             adc_line(3,3);
//         }
    }
//------------------------------------------------------------------------------
}


// void Debounce_State(void){
//     if(debounce_statesw1){
//         if(count_debounce_SW1 >= DEBOUNCE_TIME){
//             debounce_statesw1 = OFF;
//             enable_switches();
//         }
//     }
//     if(debounce_statesw2){
//         if(count_debounce_SW2 >= DEBOUNCE_TIME){
//             debounce_statesw2 = OFF;
//             enable_switches();
//         }
//     }
// }
