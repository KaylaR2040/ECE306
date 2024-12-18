
//===========================================================================
// File Name : switches.c
//
// Description: This file contains the code for when a switch is pressed and
//
// Author: Kayla Radu
// Date: Sept 20, 2024
// Compiler: Built with Code Composer Studio Version: 12.8.0.00012


#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"
#include "switches.h"
#include <stdio.h>

// Global Variables
extern volatile unsigned int Time_Sequence; // Already existing
extern volatile unsigned int switchpressed = OFF;
extern volatile unsigned int count_debounce_SW1 = OFF;
extern volatile unsigned int count_debounce_SW2 = OFF;
extern volatile unsigned int debounce_statesw1;
extern volatile unsigned int debounce_statesw2;
extern volatile unsigned char display_changed;
extern char display_line[4][11];
extern char *display[4];
extern volatile unsigned int backlight;
extern volatile unsigned char backlight_changed;
extern volatile unsigned int enableleft = 1;
extern volatile unsigned int enableright = 1;




// ENABLE SWITCHES

// ENABLE SW1
void enable_switch_SW1(void){
    P5OUT |=  SW1;
    P5DIR &= ~SW1;
}
// ENABLE SW2
void enable_switch_SW2(void){
    P5OUT |=  SW2;
    P5DIR &= ~SW2;
}
// ENABLE BOTH
void enable_switches(void){
    enable_switch_SW1();
    enable_switch_SW2();
    enableright = 1;
    enableleft = 1;
}



// DISABLE SWITCHES

// DISABLE SW1
void disable_switch_SW1(void){
    P5OUT |=  SW1;
    P5DIR &= ~SW1;
}
// DISABLE SW2
void disable_switch_SW2(void){
    P5OUT |=  SW2;
    P5DIR &= ~SW2;
}
// DISABLE BOTH
void disable_switches(void){
    disable_switch_SW1();
    disable_switch_SW2();
}



#pragma vector = PORT4_VECTOR

__interrupt void switchP4_interrupt(void){
    // Switch 1
    if (P4IFG & SW1) {
        P4IFG &= ~SW1;          // IFG SW1 cleared
        switchpressed = TRUE;
        Time_Sequence = RESTART;
        count_debounce_SW1 = RESTART;
        debounce_statesw1 = ON;
        disable_switches();
        strcpy(display_line[0], "          ");
        strcpy(display_line[1], "          ");
        strcpy(display_line[2], "          ");
        strcpy(display_line[3], " SWITCH 1 ");
        display_changed = TRUE;
        enableleft = 0;
        backlight = 0;
        backlight_changed = 1;
        P4IE &= ~SW1;


    }
}
#pragma vector = PORT2_VECTOR

__interrupt void switchP2_interrupt(void){
    // Switch 2
    if (P2IFG & SW2) {
        P2IFG &= ~SW2;          // IFG SW2 cleared
        switchpressed = TRUE;
        Time_Sequence = RESTART;
        count_debounce_SW2 = RESTART;
        debounce_statesw2 = ON;
        disable_switches();
        strcpy(display_line[0], "          ");
        strcpy(display_line[1], "          ");
        strcpy(display_line[2], "          ");
        strcpy(display_line[3], " SWITCH 2 ");
        display_changed = TRUE;
        backlight = 0;
        backlight_changed = TRUE;
        enableright = 0;
        P2IE &= ~SW2;



    }
}


void Debounce_State(void){
    if(debounce_statesw1){
        if(count_debounce_SW1 >= DEBOUNCE_TIME){
            debounce_statesw1 = OFF;
            enable_switches();
            P4IE |= SW1;
        }
    }
    if(debounce_statesw2){
        if(count_debounce_SW2 >= DEBOUNCE_TIME){
            debounce_statesw2 = OFF;
            enable_switches();
            P2IE |= SW2;
        }
    }
}

