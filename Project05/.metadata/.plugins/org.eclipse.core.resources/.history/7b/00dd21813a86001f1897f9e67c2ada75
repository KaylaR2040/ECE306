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

unsigned int wheel_Counttime;
unsigned int right_Counttime;
unsigned int left_Counttime;
unsigned int travel_Distance;
unsigned int waitingtostart;
extern volatile unsigned int triangleset = 0;
extern volatile unsigned int figureeightset = 0;
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

void Wheel_Move(void) {
// This is going to be moving the car

switch(origevent){
    case PROJECTFIVE:
        switch(instruction){
            case 1:
            travel_Distance = 16;
            waitingtostart = 20;
            event = STRAIGHTFW;
            break;
            case 2:
            travel_Distance = 32;
            waitingtostart = 140;
            event = STRAIGHTBW;
            break;
            case 3:
            travel_Distance = 16;
            waitingtostart = 140;
            event = STRAIGHTFW;
            break;
            case 4: 
            travel_Distance = 42;
            waitingtostart = 140;
            event = SPINCLK;
            break;
            case 5:
            travel_Distance = 42;
            waitingtostart = 280;
            event = SPINCOUNTERCLK;
            break;
            default: break;

        }
    break;
    default: break;
}
switch(event){
case STRAIGHT:
    rdirection = REVERSE;
    ldirection = REVERSE;
    wheel_Counttime = 10;
    right_Counttime = 7;
    left_Counttime = 8;
    travel_Distance = 5;
    waitingtostart = 10;
    Run_Shape();
    break;

    case STRAIGHTFW: // Straight
        rdirection = FORWARD;
        ldirection = FORWARD;
        wheel_Counttime = 10;
        right_Counttime = 7;
        left_Counttime = 8;
        if(!instruction){
            travel_Distance = 5;
        }

    Run_Shape();
    break;
    case STRAIGHTBW: //Backwards
    rdirection = REVERSE;
    ldirection = REVERSE;
    wheel_Counttime = 10;
    right_Counttime = 7;
    left_Counttime = 8;
    if(!instruction){
        travel_Distance = 5;
    }
    Run_Shape();
    break;
    case SPINCLK:
        rdirection = FORWARD;
        ldirection = REVERSE;
        wheel_Counttime = 10;
        right_Counttime = 7;
        left_Counttime = 8;
    Run_Shape();
    break;
    case SPINCOUNTERCLK:
        rdirection = REVERSE;
        ldirection = FORWARD;
        wheel_Counttime = 10;
        right_Counttime = 7;
        left_Counttime = 8;
    Run_Shape();
    break;
    default: break;
}
}

void Run_Shape(void) {
          switch(state){
              case WAIT: // Begin
                  wait_case();
                  break;
              case START: // Begin
                  start_case();
                  break;
              case RUN: // Run
                  run_case();
                  break;
              case END: // End
                  end_case();
                  break; //
              default: break;
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


//The first state “WAIT” allows for the button to be pressed and time to move out of the way.
void wait_case(void){
    if(time_change){
        time_change = 0;
        if(delay_start++ >= waitingtostart){
            delay_start = 0;
            state = START;
        }
    }
}

//The second state “START” sets the initial condition.
void start_case(void){
    Display_Changing();
    cycle_time = 0;
    right_motor_count = 0;
    left_motor_count = 0;
    segment_count = 0;
    state = RUN;
}

//The third state “RUN” controls the movement.
void run_case(void){
    if(time_change){
        time_change = 0;
        if(segment_count <= travel_Distance){
            if(right_motor_count++ >= right_Counttime){
                if(rdirection == FORWARD){
                    RForward_Off();
                }
                else {
                    RReverse_Off(); //stop right motor
                }
            }
            if(left_motor_count++ >= left_Counttime){
                if(ldirection == FORWARD){
                    LForward_Off(); //stop left motor
                }
                else {
                    RReverse_Off();
                }

            }
            if(cycle_time >= wheel_Counttime){
                cycle_time = 0;
                right_motor_count = 0;
                left_motor_count = 0;
                segment_count++;
                Forward_Move();
                Reverse_Move();
            }
        }else{
            state = END;
        }
    }
}









//The last state “END” clears the state machine back to NONE so it ends and movement over.
void end_case(void){
    Reverse_Off();
    Forward_Off();
    state = WAIT;
    event = NONE;
    figureeightset = 0;

    if(instruction == 5){
        instruction = 0;
        event = NONE;
        origevent = NONE;
        Display_Changing();
    }
    else{
        instruction++;
    }

}



void motorDirec(void){
    if(((P6IN & L_FORWARD)&&(P6IN & L_REVERSE))||((P6IN & R_FORWARD)&&(P6IN & R_REVERSE))){
        P1OUT |= RED_LED;
        Forward_Off();
        Reverse_Off();
    }

}


void Display_Changing(void){
        strcpy(display_line[0], "          ");
        if(origevent == PROJECTFIVE){
            strcpy(display_line[1], "PROJECT 5 ");
        }
        else{
            strcpy(display_line[1], "          ");
        }
        switch(instruction){
        case 0:
            strcpy(display_line[2], " ALL DONE ");
            break;
        case 1:
            strcpy(display_line[2], "STRAIGHTFW");
            break;
        case 2:
            strcpy(display_line[2], "STRAIGHTBW");
            break;
        case 3:
            strcpy(display_line[2], "STRAIGHTFW");
            break;
        case 4:
            strcpy(display_line[2], "SPINNINGCL");
            break;
        case 5:
            strcpy(display_line[2], "SPINNINGCC");
            break;
        default:break;
        }
        strcpy(display_line[3], "          ");
        display_changed = TRUE;

}
