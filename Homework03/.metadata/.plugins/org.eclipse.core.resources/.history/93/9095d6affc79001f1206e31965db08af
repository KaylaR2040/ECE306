
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

// Global Variables
extern volatile unsigned char event;  // (e.g., STRAIGHT, CIRCLE)
extern volatile unsigned int Time_Sequence; // Already existing
extern volatile unsigned int state;
unsigned int okay_to_look_at_switch1;
unsigned int sw1_position;
unsigned int count_debounce_SW1;
unsigned int okay_to_look_at_switch2;
unsigned int sw2_position;
unsigned int count_debounce_SW2;



//------------------------------------------------------------------------------
void Switches_Process(void){
//------------------------------------------------------------------------------
// This function calls the individual Switch Functions
//------------------------------------------------------------------------------
Switch1_Process();
Switch2_Process();
}
//------------------------------------------------------------------------------





//------------------------------------------------------------------------------
void Switch1_Process(void){
//------------------------------------------------------------------------------
// Switch 1 Configurations
//------------------------------------------------------------------------------
    if (okay_to_look_at_switch1 && (sw1_position == RELEASED)) {  // Check if we are allowed to process the switch and if it was previously released
        if (!(P4IN & SW1)) {  // Check if Switch 1 is pressed (active low)
            sw1_position = PRESSED;
            okay_to_look_at_switch1 = NOT_OKAY;  // Prevent further processing until debounce
            count_debounce_SW1 = DEBOUNCE_RESTART;  // Start debouncing
            // Perform actions for the button press here
            event = STRAIGHT;
        }
    }

    if (count_debounce_SW1 <= DEBOUNCE_TIME) {
        count_debounce_SW1++;  // Increment debounce counter
    } else {
        okay_to_look_at_switch1 = OKAY;  // Allow further checks on the switch
        if (P4IN & SW1) {  // Check if Switch 1 is released (active high)
            sw1_position = RELEASED;  // Set switch to RELEASED
        }
    }
}


//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void Switch2_Process(void){
//------------------------------------------------------------------------------
// Switch 2 Configurations
//------------------------------------------------------------------------------
    if (okay_to_look_at_switch2 && sw2_position){
        if (!(P2IN & SW2)){
            sw2_position = PRESSED;
            okay_to_look_at_switch2 = NOT_OKAY;
            count_debounce_SW2 = DEBOUNCE_RESTART;
            // do what you want with button press
            event = CIRCLE;
        }
    }
    if (count_debounce_SW2 <= DEBOUNCE_TIME){
        count_debounce_SW2++;
    }else{
        okay_to_look_at_switch2 = OKAY;
        if (P2IN & SW2){
            sw2_position = RELEASED;
        }
    }
    }
//------------------------------------------------------------------------------
