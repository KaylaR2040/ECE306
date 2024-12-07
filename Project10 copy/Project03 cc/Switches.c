//===========================================================================
// File Name : Switches.c
//
// Description: This file contains functions to process switch inputs (Switch 1 and Switch 2),
//              including debouncing logic and updating display information.
//
// Author: Kayla Radu
// Date: November 21, 2024
// Compiler: Built with Code Composer Studio Version: 12.8.0.00012
//===========================================================================

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include "msp430.h"
#include "functions.h"
#include "LCD.h"
#include "ports.h"
#include "macros.h"
#include "strings.h"
#include "wheels.h"
#include "Timers.h"
#include "switches.h"

//------------------------------------------------------------------------------
// Global Variable Declarations
//------------------------------------------------------------------------------
unsigned int okay_to_look_at_switch1 = OKAY;
unsigned int sw1_position = RELEASED;
unsigned int count_debounce_SW1 = DEBOUNCE_RESTART;

unsigned int okay_to_look_at_switch2 = OKAY;
unsigned int sw2_position = RELEASED;
unsigned int count_debounce_SW2 = DEBOUNCE_RESTART;

unsigned int event;

extern char display_line[4][11];
extern char *display[4];
volatile unsigned char display_changed;

//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------
void Switches_Process(void);
void Switch1_Process(void);
void Switch2_Process(void);

//==============================================================================
// Function: Switches_Process
//
// Description: Calls the individual functions to process Switch 1 and Switch 2.
//==============================================================================
void Switches_Process(void) {
    Switch1_Process();
    Switch2_Process();
}

//==============================================================================
// Function: Switch1_Process
//
// Description: Handles Switch 1 input, including debouncing and setting baud rate.
//==============================================================================
void Switch1_Process(void) {
    // Check if it's okay to process Switch 1 and if it’s in a pressed state
    if (okay_to_look_at_switch1 && sw1_position) {
        if (!(P4IN & SW1)) {
            sw1_position = PRESSED;
            okay_to_look_at_switch1 = NOT_OKAY;
            count_debounce_SW1 = DEBOUNCE_RESTART;

            // Set UART to 115200 baud
            UCA0BRW = 4;            // 115,200 baud
            UCA0MCTLW = 0x5551;

            // Update display with baud rate information
            strcpy(display_line[3], " 115200 b ");
            display_changed = TRUE;
        }
    }

    // Debounce logic for Switch 1
    if (count_debounce_SW1 <= DEBOUNCE_TIME) {
        count_debounce_SW1++;
    } else {
        okay_to_look_at_switch1 = OKAY;
        if (P4IN & SW1) {
            sw1_position = RELEASED;
        }
    }
}

//==============================================================================
// Function: Switch2_Process
//
// Description: Handles Switch 2 input, including debouncing.
//==============================================================================
void Switch2_Process(void) {
    // Check if it's okay to process Switch 2 and if it’s in a pressed state
    if (okay_to_look_at_switch2 && sw2_position) {
        if (!(P2IN & SW2)) {
            sw2_position = PRESSED;
            okay_to_look_at_switch2 = NOT_OKAY;
            count_debounce_SW2 = DEBOUNCE_RESTART;
        }
    }

    // Debounce logic for Switch 2
    if (count_debounce_SW2 <= DEBOUNCE_TIME) {
        count_debounce_SW2++;
    } else {
        okay_to_look_at_switch2 = OKAY;
        if (P2IN & SW2) {
            sw2_position = RELEASED;
        }
    }
}
