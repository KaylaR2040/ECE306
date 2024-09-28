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

unsigned int delay_start;
unsigned int left_motor_count;
unsigned int right_motor_count;
unsigned int segment_count;

unsigned int wheel_Counttime;
unsigned int right_Counttime;
unsigned int left_Counttime;
unsigned int travel_Distance;
extern volatile unsigned int triangleset = 0;


void Wheel_Move(void) {
// This is going to be moving the car
switch(event){
                case STRAIGHT: // Straight
                    wheel_Counttime = 10;
                    right_Counttime = 7;
                    left_Counttime = 8;
                    travel_Distance = 2;

                Run_Shape();
                break;
                case CIRCLE: // Circle
                    wheel_Counttime = 18;
                    right_Counttime = 1;
                    left_Counttime = 20;
                    travel_Distance = 75;

                    Run_Shape();
                break;
                case TRIANGLE:
                    Run_Shape();

                break;
                case FIGUREEIGHT:
                    wheel_Counttime = 10;
                    right_Counttime = 7;
                    left_Counttime = 8;
                    travel_Distance = 2;
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
                  if(event == TRIANGLE) {
                      run_triangle();
                  }
                  else {
                      run_case();
                  }
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
//    Forward_On();
    segment_count = 0;
    state = RUN;
}

//The third state “RUN” controls the movement.
void run_case(void){
    if(time_change){
        time_change = 0;
        if(segment_count <= travel_Distance){
            if(right_motor_count++ >= right_Counttime){
                P6OUT &= ~R_FORWARD; //stop right motor
            }
            if(left_motor_count++ >= left_Counttime){
                P6OUT &= ~L_FORWARD; //stop left motor
            }
            if(cycle_time >= wheel_Counttime){
                cycle_time = 0;
                right_motor_count = 0;
                left_motor_count = 0;
//                segment_count++;
                Forward_Move();  // Trigger forward movement
            }
        }else{
            state = END;
        }
    }
}

// This is for the triangle
void run_triangle(void){
    if(time_change){
        time_change = 0;
        if(triangleset % 2){
            wheel_Counttime = 10;
            right_Counttime = 0;
            left_Counttime = 8;
            travel_Distance = 10;
            if(segment_count <= travel_Distance){
                        if(right_motor_count++ >= right_Counttime){
                            P6OUT &= ~R_FORWARD; //stop right motor
                        }
                        if(left_motor_count++ >= left_Counttime){
                            P6OUT &= ~L_FORWARD; //stop left motor
                        }
                        if(cycle_time >= wheel_Counttime){
                            cycle_time = 0;
                            right_motor_count = 0;
                            left_motor_count = 0;
                            segment_count++;
                            Forward_Move();  // Trigger forward movement
                        }
            }else{
                triangleset++;
                Forward_Off();
                wait_case();
                state = RUN;
            }
        }
        else{
            wheel_Counttime = 18;
            right_Counttime = 10;
            left_Counttime = 0;
            travel_Distance = 8;
            if(segment_count <= travel_Distance){
                        if(right_motor_count++ >= right_Counttime){
                            P6OUT &= ~R_FORWARD; //stop right motor
                        }
                        if(left_motor_count++ >= left_Counttime){
                            P6OUT &= ~L_FORWARD; //stop left motor
                        }
                        if(cycle_time >= wheel_Counttime){
                            cycle_time = 0;
                            right_motor_count = 0;
                            left_motor_count = 0;
                            segment_count++;
                            Forward_Move();  // Trigger forward movement
                        }
            }else{
                triangleset++;
                Forward_Off();
                wait_case();
                state = RUN;
            }
        }
        if(triangleset == 6){
            state  = END;
        }
    }
}


//The last state “END” clears the state machine back to NONE so it ends and movement over.
void end_case(void){
    Forward_Off();
    state = WAIT;
    event = NONE;
    triangleset = 0;

}




