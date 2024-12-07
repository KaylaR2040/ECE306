//===========================================================================
// File Name : Timer_Interrupts.c
//
// Description: This file contains the Timer B0 interrupt service routines (ISRs)
//              for handling timer-based events, such as display updates, 
//              motor control, and system timing.
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
//#include "DAC.h"
#include "switches.h"

//------------------------------------------------------------------------------
// Global Variable Declarations
//------------------------------------------------------------------------------
unsigned int display_count;
volatile unsigned char display_changed;
extern char display_line[4][11];
volatile unsigned char update_display;


unsigned int CCR0_counter = 0;
unsigned int IOT_on_timer = 0;

volatile unsigned int running_timecount = 0;
unsigned int running_timecount_flag = 0;

unsigned int pingcount = 0;
extern unsigned int ping;

unsigned int DimFlag = TRUE;
unsigned int Blink_counter = 0;

unsigned int FlagSpinR = FALSE;
extern unsigned int SpincountR;

unsigned int FlagWait = FALSE;
extern unsigned int Waitcount;

unsigned int LostFlag = FALSE;
unsigned int Lostcount = 0;

unsigned int CircleFlag = FALSE;
unsigned int Circlecount = 0;

extern unsigned int MotorFlag;
extern unsigned int Motorcount;

extern unsigned int commanding_send;
extern unsigned int movement;
unsigned int movementcount = 0;
unsigned int init_completed;
extern unsigned int DAC_data;
 

//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void);

#pragma vector = TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void);

//==============================================================================
// Function: Timer0_B0_ISR
//
// Description: Timer B0 CCR0 interrupt service routine. Handles periodic tasks,
//              such as display updates, ping operations, and backlight dimming.
//==============================================================================
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void) {
    // Display update every 4 counts
    if (display_count++ == 4) {
        display_count = 0;
        update_display = TRUE;
        CCR0_counter++;
        IOT_on_timer++;
    }

    // Ping the internet values every 50 counts
    if (pingcount++ == 50) {
        ping = !ping;
        pingcount = 0;
    }

    // LCD backlight dimming to save battery
    if (DimFlag == TRUE) {
        if (Blink_counter++ >= 4) {
            Blink_counter = 0;
            LCD_BACKLITE_DIMING = PERCENT_80; // Turn on backlight
        }
    }

    // Running time count flag
    if (running_timecount_flag) {
        running_timecount++;
    }

    // Spin right flag counter
    if (FlagSpinR == TRUE) {
        SpincountR++;
    }

    // Wait flag counter
    if (FlagWait == TRUE) {
        Waitcount++;
    }

    // Add offset to TB0CCR0 for the next interrupt
    TB0CCR0 += TB0CCR0_INTERVAL;
}

//==============================================================================
// Function: TIMER0_B1_ISR
//
// Description: Timer B0 CCR1, CCR2, and overflow interrupt service routine.
//              Handles tasks such as DAC updates, motor control, and tracking.
//==============================================================================
#pragma vector = TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void) {
    switch (__even_in_range(TB0IV, 14)) {
        case 0: 
            break; // No interrupt

        case 2: // CCR1 Interrupt
            TB0CCR1 += TB0CCR1_INTERVAL;
            TB0CCTL1 |= CCIE; // Enable Timer B0 CCR1 interrupt

            if (DAC_data > 900) {
                DAC_data -= 50;
                SAC3DAT = DAC_data;
            }
            break;

        case 4: // CCR2 Interrupt
            TB0CCR2 += TB0CCR2_INTERVAL; // Add offset to TB0CCR2
            ADCCTL0 |= ADCSC;            // Start next ADC sample

            // Lost tracking mode backup
            if (LostFlag) {
                Lostcount++;
            }

            // Circle movement timer
            if (CircleFlag) {
                Circlecount++;
            }

            // Motor running flag timer
            if (MotorFlag) {
                Motorcount++;
            }

            // Movement timer for non-intercept commands
            if (movement) {
                if (commanding_send != INTERCEPT) {
                    movementcount++;
                }
            }
            break;

        case 14: // Overflow Interrupt
            // Add any necessary overflow handling code here
            break;

        default:
            break;
    }
}
