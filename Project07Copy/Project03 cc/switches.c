
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
#include "timersB0.h"

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
extern volatile unsigned char state;
volatile unsigned int pressed = 1;



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



void Debounce_State(void){
    if(debounce_statesw1){
        if(count_debounce_SW1 >= DEBOUNCE_TIME){
            debounce_statesw1 = OFF;
            enable_switches();
        }
    }
    if(debounce_statesw2){
        if(count_debounce_SW2 >= DEBOUNCE_TIME){
            debounce_statesw2 = OFF;
            enable_switches();
        }
    }
}
