//===========================================================================
// File Name : Timers.c
//
// Description: This file contains the initialization for Timer B0 and Timer B3.
//              Timer B0 is set up for continuous counting with multiple interrupts.
//              Timer B3 is configured for PWM control of wheel movements and the LCD backlight.
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
unsigned int display_count;
volatile unsigned char display_changed;
extern char display_line[4][11];
volatile unsigned char update_display;
volatile char one_time;
volatile unsigned int Time_Sequence;

//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------
void Init_Timers(void);
void Init_Timer_B0(void);
void Init_Timer_B3(void);

//==============================================================================
// Function: Init_Timers
//
// Description: Initializes Timer B0 and Timer B3 by calling their respective
//              initialization functions.
//==============================================================================
void Init_Timers(void) {
    Init_Timer_B0();
    Init_Timer_B3();
}

//==============================================================================
// Function: Init_Timer_B0
//
// Description: Initializes Timer B0 for continuous counting with interrupts
//              on CCR0, CCR1, and CCR2. The timer uses SMCLK as the clock source,
//              divided by 8, with an additional divider of 8.
//==============================================================================
void Init_Timer_B0(void) {
    TB0CTL = TBSSEL__SMCLK;          // SMCLK source
    TB0CTL |= TBCLR;                 // Reset TB0R, clock divider, count direction
    TB0CTL |= MC__CONTINOUS;         // Continuous up mode
    TB0CTL |= ID__8;                 // Divide clock by 8
    TB0EX0 = TBIDEX__8;              // Additional divide by 8

    TB0CCR0 = TB0CCR0_INTERVAL;      // Set CCR0 interval
    TB0CCTL0 |= CCIE;                // Enable CCR0 interrupt

    TB0CCR1 = TB0CCR1_INTERVAL;      // Set CCR1 interval
    TB0CCTL1 |= CCIE;                // Enable CCR1 interrupt

    TB0CCR2 = TB0CCR2_INTERVAL;      // Set CCR2 interval
    TB0CCTL2 |= CCIE;                // Enable CCR2 interrupt

    TB0CTL &= ~TBIE;                 // Disable overflow interrupt
    TB0CTL &= ~TBIFG;                // Clear overflow interrupt flag
}

//==============================================================================
// Function: Init_Timer_B3
//
// Description: Initializes Timer B3 for PWM control. Timer B3 controls the LCD
//              backlight and the wheel motors (right and left, forward and reverse).
//==============================================================================
void Init_Timer_B3(void) {
    // Timer B3 Configuration
    TB3CTL = TBSSEL__SMCLK;          // SMCLK source
    TB3CTL |= MC__UP;                // Up mode
    TB3CTL |= TBCLR;                 // Clear Timer B3 register

    PWM_PERIOD = WHEEL_PERIOD;       // Set PWM period (e.g., 50005)

    // LCD Backlight Control (P6.0)
    TB3CCTL1 = OUTMOD_7;             // CCR1 reset/set
    LCD_BACKLITE_DIMING = PERCENT_80;// LCD backlight PWM duty cycle

    // Right Wheel Forward Control (P6.1)
    TB3CCTL2 = OUTMOD_7;             // CCR2 reset/set
    RIGHT_FORWARD_SPEED = WHEEL_OFF; // Right forward PWM duty cycle

    // Right Wheel Reverse Control (P6.2)
    TB3CCTL3 = OUTMOD_7;             // CCR3 reset/set
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // Right reverse PWM duty cycle

    // Left Wheel Forward Control (P6.3)
    TB3CCTL4 = OUTMOD_7;             // CCR4 reset/set
    LEFT_FORWARD_SPEED = WHEEL_OFF;  // Left forward PWM duty cycle

    // Left Wheel Reverse Control (P6.4)
    TB3CCTL5 = OUTMOD_7;             // CCR5 reset/set
    LEFT_REVERSE_SPEED = WHEEL_OFF;  // Left reverse PWM duty cycle
}
