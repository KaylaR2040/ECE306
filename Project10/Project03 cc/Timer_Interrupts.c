/*
 * Timer_Interrupts.c
 *
 *  Created on: Nov 21, 2024
 *  Author: Kayla Radu
 *
 */
//------------------------------------------------------------------------------

#include  "msp430.h"
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"
#include "strings.h"
#include "wheels.h"
#include "Timers.h"
#include  "DAC.h"
#include "switches.h"

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//variable declarations
unsigned int display_count;
volatile unsigned char display_changed;
extern char display_line[4][11];
volatile unsigned char update_display;
volatile char one_time;
volatile unsigned int Time_Sequence;
unsigned int CCR0_counter;
unsigned int iot_on_time;
char transmit_state;
unsigned int initialize_done;
unsigned int run_time;
unsigned int run_time_flag;
extern volatile unsigned int debounce_statesw1 = 0;
extern volatile unsigned int debounce_statesw2 = 0;
extern volatile unsigned int count_debounce_SW1;
extern volatile unsigned int count_debounce_SW2;
unsigned int DimFlag = TRUE;
unsigned int FlagSpinL = FALSE;
unsigned int FlagSpinR = FALSE;
unsigned int FlagWait = FALSE;
unsigned int Blink_counter;
unsigned int LostFlag = FALSE;
unsigned int Lostcount = 0;
unsigned int CircleFlag = FALSE;
unsigned int Circlecount = 0;
extern unsigned int MotorFlag;
extern unsigned int Motorcount;

extern unsigned int SpincountL;
extern unsigned int SpincountR;
extern unsigned int Waitcount;
extern unsigned int ping;
unsigned int pingcount = 0;
extern unsigned int movement;
unsigned int movementcount =0;

extern unsigned int commanding_send;

//------------------------------------------------------------------------------

#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){
    //------------------------------------------------------------------------------
    // TimerB0 0 Interrupt handler
    //----------------------------------------------------------------------------
    //...... Add What you need happen in the interrupt ......
    // LCD Backlight

    if(display_count++ == 4){
        display_count = 0;
        update_display = TRUE;

        CCR0_counter +=1;
        iot_on_time++;
//        if (transmit_state == TRANSMIT){
//            transmit_count++;
//        }

    }

    if(pingcount++ == 50){
//        P1OUT ^= RED_LED;
        ping = !ping;
        pingcount = 0;
    }

//    ADCCTL0 |= ADCSC; // Start next sample

    // Time Sequence
//    one_time = 1; //CARLSON CODE
//    if(Time_Sequence++ > 250){
//        Time_Sequence = 0;
//    }



    if(DimFlag == TRUE){
        if (Blink_counter++ >= 4){
            Blink_counter = 0;
            LCD_BACKLITE_DIMING = PERCENT_80; //Flips on

        }
    }

    if (run_time_flag){
        run_time++;
    }

//    if(FlagSpinL == TRUE){
//        SpincountL++;
//    }


    if(FlagSpinR == TRUE){
        SpincountR++;
    }

    if(FlagWait == TRUE){
        Waitcount++;
    }

    TB0CCR0 += TB0CCR0_INTERVAL; // Add Offset to TBCCR0

    //----------------------------------------------------------------------------
}

#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void){

    //----------------------------------------------------------------------------
    // TimerB0 1-2, Overflow Interrupt Vector (TBIV) handler
    //----------------------------------------------------------------------------
    switch(__even_in_range(TB0IV,14)){
    P1OUT ^= RED_LED;
    case 0: break; // No interrupt
    case 2:
        P1OUT ^= GRN_LED;
        TB0CCR1 += TB0CCR1_INTERVAL;
        TB0CCTL1 |= CCIE;//Enable TimerB0_1
        if(DAC_data > 900){
            DAC_data -= 50;
            SAC3DAT = DAC_data;
        }
        break;
    case 4: // CCR2 not used
//        P1OUT ^= RED_LED;
        TB0CCR2 += TB0CCR2_INTERVAL; // Add Offset to TBCCR1
        ADCCTL0 |= ADCSC; // Start next sample

        if(LostFlag){
            Lostcount++;
        }
        if(CircleFlag){
            Circlecount++;
        }
        if(MotorFlag){
            Motorcount++;
        }

        if(movement){
            if(commanding_send != INTERCEPT){
                movementcount++;
            }
        }

//        TB0CCTL0 |= CCIE;
        /*strcpy(display_line[0], "          ");
        strcpy(display_line[1], "          ");
        strcpy(display_line[2], "          ");
        strcpy(display_line[3], "          ");
        display_changed = TRUE;*/
        break;
    case 14: // overflow
        //...... Add What you need happen in the interrupt ......
        break;
    default: break;
    }
    //----------------------------------------------------------------------------
}

//#pragma vector=TIMER0_B1_VECTOR
//__interrupt void TIMER0_B1_ISR(void){
//    P1OUT ^= RED_LED;
//    //----------------------------------------------------------------------------
//    // TimerB0 1-2, Overflow Interrupt Vector (TBIV) handler
//    //----------------------------------------------------------------------------
//    switch(__even_in_range(TB0IV,14)){
//    case  0: break;                    // No interrupt
//    case  2:                           // CCR1 Used for SW1 Debounce
//        P6OUT ^= GRN_LED;
//        count_debounce_SW1++;
//        if (count_debounce_SW1 >= DEBOUNCE_TIME){
//            count_debounce_SW1 = 0;
//
//            TB0CCTL1 &= ~CCIE;
//            P4IFG &= ~SW1;
//            P4IE  |= SW1;
//
//            TB0CCTL0 |= CCIE;
//        }
//
//        if(DAC_data > 900){
//            DAC_data -= 50;
//            SAC3DAT = DAC_data;
//        }
//        break;
//
//    case  4:                           // CCR2 Used for SW2 Debounce
//        count_debounce_SW2++;
//        if (count_debounce_SW2 >= DEBOUNCE_TIME){
//            count_debounce_SW2 = 0;
//
//            TB0CCTL2 &= ~CCIE;
//            P2IFG &= ~SW2;
//            P2IE  |=  SW2;
//
//            TB0CCTL0 |= CCIE;
//        }
//
//        break;
//
//    case 14:                           // overflow available for greater than 1 second timer
//        break;
//    default: break;
//    }
    //----------------------------------------------------------------------------

