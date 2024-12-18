/*
 * wheels.c
 *
 *  Created on: Sep 25, 2024
 *      Author: kaylaradu
 */


#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"
#include "switches.h"
#include "wheels.h"
#include  <stdio.h>






volatile unsigned int state;   // To manage the state machine
volatile unsigned char event;  // Event variable from switches.c (e.g., STRAIGHT, CIRCLE)
extern volatile unsigned int event_Counter; //picking the event based on increment

extern unsigned int Last_Time_Sequence;  // To track changes in Time_Sequence
extern unsigned int cycle_time;          // Controls shape timings
extern unsigned int time_change;         // Flag to detect time sequence change
 unsigned int Time_Sequence;

unsigned int delay_start;
unsigned int left_motor_count;
unsigned int right_motor_count;
unsigned int segment_count;


unsigned int waitingtostart;
extern volatile unsigned int rdirection = 0;
extern volatile unsigned int ldirection = 0;
extern volatile unsigned int instruction;
volatile unsigned char origevent;
extern volatile unsigned int elapsed_time = 0;
extern volatile unsigned int desiredtime= 0;
extern volatile unsigned int desired_cycles = 0;

extern char display_line[4][11];
extern char *display[4];
extern volatile unsigned char display_changed;
extern volatile unsigned int switchpressed;

void Wheel_Move(void) {
    if(switchpressed){
        switch(Time_Sequence){
        case ONESEC: // 1st second
                     strcpy(display_line[0], "          ");
                     strcpy(display_line[1], " FORWARDS ");
                     strcpy(display_line[2], "ONE SECOND");
                     strcpy(display_line[3], "          ");
                     display_changed = TRUE;
                     ldirection = FORWARD;
                     rdirection = FORWARD;
                     Off_Case();
                     Forward_Move();
                     break; //
                 case TWO_SECONDS*ONESEC:
                     strcpy(display_line[0], "          ");
                     strcpy(display_line[1], "  WAIT    ");
                     strcpy(display_line[2], "ONE SECOND");
                     strcpy(display_line[3], "          ");
                     display_changed = TRUE;
                     Off_Case();
                     break;
                 case THREE_SECONDS*ONESEC:
                     strcpy(display_line[0], "          ");
                     strcpy(display_line[1], "  REVERSE ");
                     strcpy(display_line[2], "TWOSECONDS");
                     strcpy(display_line[3], "          ");
                     display_changed = TRUE;
                     ldirection = REVERSE;
                     rdirection = REVERSE;
                     Off_Case();
                     Reverse_Move();
                     break;
                 case FIVE_SECONDS*ONESEC:
                     strcpy(display_line[0], "          ");
                     strcpy(display_line[1], "  WAIT    ");
                     strcpy(display_line[2], "ONE SECOND");
                     strcpy(display_line[3], "          ");
                     display_changed = TRUE;
                     Off_Case();
                     break; //
                 case SIX_SECONDS*ONESEC:
                     strcpy(display_line[0], "          ");
                     strcpy(display_line[1], " FORWARDS ");
                     strcpy(display_line[2], "ONE SECOND");
                     strcpy(display_line[3], "          ");
                     display_changed = TRUE;
                     ldirection = FORWARD;
                     rdirection = FORWARD;
                     Off_Case();
                     Forward_Move();
                     break;
                 case SEVEN_SECONDS*ONESEC:
                     strcpy(display_line[0], "          ");
                     strcpy(display_line[1], "  WAIT    ");
                     strcpy(display_line[2], "ONE SECOND");
                     strcpy(display_line[3], "          ");
                     display_changed = TRUE;
                     Off_Case();
                     break;
                 case EIGHT_SECONDS*ONESEC:
                     strcpy(display_line[0], "          ");
                     strcpy(display_line[1], " CLOCKWISE");
                     strcpy(display_line[2], " THREESECS");
                     strcpy(display_line[3], "          ");
                     display_changed = TRUE;
                     Off_Case();
                     rdirection = FORWARD;
                     ldirection = REVERSE;
                     Forward_Move();
                     Reverse_Move();
                     break;
                 case TEN_SECONDS*ONESEC:
                     strcpy(display_line[0], "          ");
                     strcpy(display_line[1], "  WAIT    ");
                     strcpy(display_line[2], "TWOSECONDS");
                     strcpy(display_line[3], "          ");
                     display_changed = TRUE;
                     Off_Case();
                     break;
                 case TWELVE_SECONDS*ONESEC:
                     strcpy(display_line[0], "          ");
                     strcpy(display_line[1], " CCLKWISE ");
                     strcpy(display_line[2], " THREESECS");
                     strcpy(display_line[3], "          ");
                     display_changed = TRUE;
                     ldirection = FORWARD;
                     rdirection = REVERSE;
                     Forward_Move();
                     Reverse_Move();
                     break;
                 case FOURTEEN_SECONDS*ONESEC:
                     strcpy(display_line[0], "          ");
                     strcpy(display_line[1], "  WAIT    ");
                     strcpy(display_line[2], "TWOSECONDS");
                     strcpy(display_line[3], "          ");
                     display_changed = TRUE;
                     Off_Case();
                     break;
                 default: break;
    }
    }
}



void LForward_On(void) {
    P6OUT |= L_FORWARD;   // Turn on the left motor (set pin high)
}
void RForward_On(void) {
    P6OUT |= R_FORWARD;
}

void LForward_Off(void){
    P6OUT &= ~L_FORWARD;  // Turn off the left motor (set pin low)
}
void RForward_Off(void){
    P6OUT &= ~R_FORWARD;  // Turn off the left motor (set pin low)
}
void LReverse_On(void) {
    P6OUT |= L_REVERSE;   // Turn on the left motor (set pin high)
}
void RReverse_On(void){
    P6OUT |= R_REVERSE;
}
void LReverse_Off(void){
    P6OUT &= ~L_REVERSE;  // Turn off the left motor (set pin low)
}
void RReverse_Off(void){
    P6OUT &= ~R_REVERSE;  // Turn off the left motor (set pin low)
}
void Forward_Off(void){
    P6OUT &= ~L_FORWARD;
    P6OUT &= ~R_FORWARD;

}
void Reverse_Off(void){
    P6OUT &= ~L_REVERSE;
    P6OUT &= ~R_REVERSE;

}

void Forward_Move(void) {
    if(ldirection == FORWARD){
        LForward_On();
    }
    if(rdirection == FORWARD){
        RForward_On();
    }
}
void Reverse_Move(void) {
    if(ldirection == REVERSE){
        LReverse_On();
    }
    if(rdirection == REVERSE){
        RReverse_On();
    }
}
void Off_Case(void){
    Forward_Off();
    Reverse_Off();
}



void motorDirec(void){
    if(((P6IN & L_FORWARD)&&(P6IN & L_REVERSE))||((P6IN & R_FORWARD)&&(P6IN & R_REVERSE))){
        P1OUT |= RED_LED;
        Forward_Off();
        Reverse_Off();
    }

}
