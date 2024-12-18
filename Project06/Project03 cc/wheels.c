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
extern volatile unsigned char update_display;
extern volatile unsigned int switchpressed;
extern volatile unsigned int instruction;

extern volatile unsigned int blackleft;
extern volatile unsigned int blackright;
extern volatile unsigned int blacklinefound;

extern volatile unsigned int ADC_Left_Detect;
extern volatile unsigned int ADC_Right_Detect;

unsigned int SpincountL;
unsigned int SpincountR;
extern unsigned int FlagSpinL;
extern unsigned int FlagSpinR;

volatile unsigned char state;




void start_movement(void){
    Off_Case();
    LEFT_FORWARD_SPEED = 12000;
        RIGHT_FORWARD_SPEED = 12000;

        if((ADC_Left_Detect >= BLACK) || (ADC_Right_Detect >= BLACK)){
            state = SPIN;
        }
}

void detected_movement(void){

    if((ADC_Left_Detect >= BLACK) || (ADC_Right_Detect >= BLACK)){
        state = END;
    }
    else if(ADC_Left_Detect >= BLACK){
        FlagSpinL = TRUE;
        if(SpincountL >= 5){
            RIGHT_FORWARD_SPEED = SPEED1;
            LEFT_REVERSE_SPEED = SPEED1;
            FlagSpinL = FALSE;
        }
    }else{
        FlagSpinR = TRUE;
        if(SpincountR >= 5){
            RIGHT_REVERSE_SPEED = SPEED1;
            LEFT_FORWARD_SPEED = SPEED1;
            FlagSpinR = FALSE;
        }
    }

}

void spinning_movement(void){
    FlagSpinL = TRUE;
    RIGHT_FORWARD_SPEED = SPEED1;
    LEFT_REVERSE_SPEED = SPEED1;
    if(SpincountL >= 5){
        state = DETECTED;
        SpincountL = 0;
    }

}

void end_state(void){
    strcpy(display_line[0], "LINE INTER");
    update_display = TRUE;
    display_changed = 1;
    Off_Case();
    SpincountL = 0;
    SpincountR = 0;
    state = NONE;

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
    //    Forward_Off();
    //    Reverse_Off();
    RIGHT_FORWARD_SPEED = WHEEL_OFF;   // P6.1 Right Forward PWM duty cycle


    LEFT_FORWARD_SPEED = WHEEL_OFF;    // P6.2 Right Forward PWM duty cycl


    RIGHT_REVERSE_SPEED = WHEEL_OFF;   // P6.3 Left Forward PWM duty cycle


    LEFT_REVERSE_SPEED = WHEEL_OFF;
}



void motorDirec(void){
    if(((P6IN & L_FORWARD)&&(P6IN & L_REVERSE))||((P6IN & R_FORWARD)&&(P6IN & R_REVERSE))){
        P1OUT |= RED_LED;
        Forward_Off();
        Reverse_Off();
    }

}
