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




volatile unsigned int state;   // To manage the state machine
// This is the way that everything will be run



// This is going to be moving the car
switch(event){
                case STRAIGHT: // Straight
                Run_Shape();
                break;
                case CIRCLE: // Circle
                    Run_Shape();
                break;
                case TRIANGLE:
                    Run_Shape();
                break;
                case: FIGUREEIGHT:
                    Run_Shape();
                break;
                default: break;
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
void Forward_On(void) {
    P6OUT |= L_FORWARD;   // Turn on the left motor (set pin high)
    P6OUT |= R_FORWARD;
}

void Forward_Off(void){
    P6OUT &= ~L_FORWARD;  // Turn off the left motor (set pin low)
    P6OUT &= ~R_FORWARD;  // Turn off the right motor (set pin low)
}

void Forward_Move(void) {
    Forward_On();
}


//The first state “WAIT” allows for the button to be pressed and time to move out of the way.
void wait_case(void){
    if(time_change){
        time_change = 0;
        if(delay_start++ >= WAITING2START){
            delay_start = 0;
            state = START;
        }
    }
}

//The second state “START” sets the initial condition.
void start_case(void){
    cycle_time = 0;
    right_motor_count = 0;
    left_motor_count = 0;
    Forward_On();
    segment_count = 0;
    state = RUN;
}

//The third state “RUN” controls the movement.
void run_case(void){
    if(time_change){
        time_change = 0;
        if(segment_count <= TRAVEL_DISTANCE){
            if(right_motor_count++ >= RIGHT_COUNT_TIME){
                P6OUT &= ~R_FORWARD; //stop right motor
            }
            if(left_motor_count++ >= LEFT_COUNT_TIME){
                P6OUT &= ~L_FORWARD; //stop left motor
            }
            if(cycle_time >= WHEEL_COUNT_TIME){
                cycle_time = 0;
                right_motor_count = 0;
                left_motor_count = 0;
                segment_count++;
                Forward_Move();  // Trigger forward movement
            }
        }else{
            state = END;
        }
    }
}

//The last state “END” clears the state machine back to NONE so it ends and movement over.
void end_case(void){
    Forward_Off();
    state = WAIT;
    event = NONE;
    selected = NONE;
}



