/*
 * wheels.c
 *
 *  Created on: Nov 21, 2024
 *      Author: Kayla Radu
 */


#include  "msp430.h"
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"
#include "strings.h"
#include "wheels.h"
#include "Timers.h"
//#include  "DAC.h"
#include "switches.h"



void Wheels_forward(void);
void Wheels_backward(void);
void wheels_turn_right(void);
void wheels_turn_left(void);



volatile unsigned char event;  // Event variable from switches.c (e.g., STRAIGHT, CIRCLE)
extern volatile unsigned int event_Counter; //picking the event based on increment

extern unsigned int Last_Time_Sequence;  // To track changes in Time_Sequence
extern unsigned int cycle_time;          // Controls shape timings
extern unsigned int time_change;         // Flag to detect time sequence change
unsigned int Time_Sequence;

unsigned int delay_start; // Delay start time
unsigned int left_motor_count;
unsigned int right_motor_count;
unsigned int segment_count;


extern volatile unsigned int rdirection = 0;
extern volatile unsigned int ldirection = 0;
extern volatile unsigned int instruction;
volatile unsigned char origevent;
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

extern volatile unsigned int ADC_Detection_L;
extern volatile unsigned int ADC_Detection_R;

unsigned int SpincountL = 0;
unsigned int SpincountR = 0;
unsigned int Waitcount = 0;
extern unsigned int FlagSpinL;
extern unsigned int FlagSpinR;
extern unsigned int FlagWait;
unsigned int DirectionTurn = 0;

volatile unsigned char state;
extern unsigned int LostFlag;
extern unsigned int Lostcount;

extern unsigned int CircleFlag;
extern unsigned int Circlecount;

unsigned int MotorFlag = FALSE;
unsigned int Motorcount = 0;

extern unsigned int setTime;





void Wheels_off (void) {
    MotorFlag = FALSE;
    Motorcount = 0;
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
}

void Wheels_forward (void) {
    MotorFlag = TRUE;
    LCD_BACKLITE_DIMING = PERCENT_80; // P6.0 Right Forward PWM OFF

    RIGHT_FORWARD_SPEED = FASTRIGHT; // P6.1 Right Forward PWM ON amount
 
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.2 Right Reverse PWM OFF

    if(Motorcount < MOTORAHEAD){
        LEFT_FORWARD_SPEED = WHEEL_OFF;

    }
    else{
        LEFT_FORWARD_SPEED = FASTLEFT; // P6.3 Left Forward PWM ON amount
    }

    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM ON amount
}

void Wheels_backward (void) {
    LCD_BACKLITE_DIMING = PERCENT_80; // P6.0 Right Forward PWM OFF

    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM OFF

    RIGHT_REVERSE_SPEED = FASTRIGHTB; // P6.2 Right Reverse PWM ON amount

    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.3 Left Forward PWM OFF

    LEFT_REVERSE_SPEED = FASTLEFTB; // P6.4 Left Reverse PWM ON amount
}

void wheels_turn_left (void) {
    LCD_BACKLITE_DIMING = PERCENT_80; // P6.0 Right Forward PWM OFF

    RIGHT_FORWARD_SPEED = FAST; // P6.1 Right Forward PWM ON amount

    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.2 Right Reverse PWM OFF

    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.3 Left Forward PWM OFF

    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM OFF

}

void wheels_turn_right (void) {
    LCD_BACKLITE_DIMING = PERCENT_80; // P6.0 Right Forward PWM OFF


    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM OFF


    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.2 Right Reverse PWM OFF


    LEFT_FORWARD_SPEED = FAST; // P6.3 Left Forward PWM ON amount

    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM OFF


}

// THIS IS FOR THE WORST CASE OF FW AND RVS BOTH BEING ON
void motor_failure (void) {
    if ((RIGHT_FORWARD_SPEED > 0) && (RIGHT_REVERSE_SPEED > 0)) {
        Wheels_off();
        P1OUT |= RED_LED;
    }
    if ((LEFT_FORWARD_SPEED > 0) && (LEFT_REVERSE_SPEED > 0)) {
        Wheels_off();
        P1OUT |= RED_LED;
    }
}



// This is the main state machine for the car
void straightline(void){
    strcpy(display_line[0], "STRAIGHT  ");
    display_changed = TRUE;
    FlagWait = TRUE;
    LEFT_FORWARD_SPEED = STRAIGHTLEFT;
    RIGHT_FORWARD_SPEED = STRAIGHTRIGHT;

    if(Waitcount >= STRAIGHTDIST){
        Off_Case();
        state = STRAIGHTWAIT;
        FlagWait = FALSE;
        Waitcount = 0;
    }

}

// Pause between all of the states
void straightwait(void){
    strcpy(display_line[0], "  WAITING ");
    display_changed = TRUE;
    FlagWait = TRUE;
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    if(Waitcount >= PAUSE){
        state = SEMICIRCLERT;
        FlagWait = FALSE;
        Waitcount=0;
    }

}

// This is the first circle that the car will make
void semicirclert(void){
    strcpy(display_line[0], "SEMICIRCLE");
    display_changed = TRUE;
    FlagWait = TRUE;

    RIGHT_FORWARD_SPEED = SEMICIRRRT;

    if(Waitcount >= SEMICIRCLEDIST){
        Off_Case();
        FlagWait = FALSE;
        Waitcount = 0;
        state = STRAIGHTW;
    }

}

// This is the second straight line that the car will make
void straightsecond(void){
    strcpy(display_line[0], "STRAIGHTW ");
       display_changed = TRUE;
       FlagWait = TRUE;
       LEFT_FORWARD_SPEED = STRAIGHTLEFT;
       RIGHT_FORWARD_SPEED = STRAIGHTRIGHT;

       if(Waitcount >= STRAIGHTDISTW){
           Off_Case();
           state = START;
           FlagWait = FALSE;
           Waitcount = 0;
       }
}

// This sets the car to start moving toward the line unitl it sees it
void start_movement(void){
    Off_Case();
    LEFT_FORWARD_SPEED = FASTLEFT;
    RIGHT_FORWARD_SPEED = FASTRIGHT;

    strcpy(display_line[0], "INTERCEPT ");
    display_changed = TRUE;

    //        if((ADC_Detection_L >= BLACK) || (ADC_Detection_R >= BLACK)){
    //            state = SPIN;
    //        }

    if((ADC_Detection_L >= 700 ) && (ADC_Detection_R >= 700)){
        Waitcount = 0;
        state = DETECTED;
    }


}

//  This is the state where the car will wait until it sees the line
void detect_movement(void){
    FlagWait = TRUE;
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    strcpy(display_line[0], "BLACK LINE ");
    display_changed = TRUE;
    if(Waitcount >= PAUSE){
        state = SPIN;
        FlagWait = FALSE;
        Waitcount=0;
    }
}

void spinning_movement(void){
    // now get it off course like a turn
    strcpy(display_line[0], "SPINNINGL");
    display_changed = TRUE;
    if(ADC_Detection_L >= BLACKLSP && ADC_Detection_R >= BLACKRSP){
        state = TRACKWAIT;
    }
    else {
        LEFT_REVERSE_SPEED = SPINSPEED;
        RIGHT_FORWARD_SPEED = SPINSPEEDRT;
    }

}
void track_wait(void){
    FlagWait = TRUE;
    strcpy(display_line[0], " TRACKWAIT");
    display_changed = TRUE;
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_FORWARD_SPEED = WHEEL_OFF;

    if(Waitcount >= PAUSE){
        state = TRACK;
        FlagWait = FALSE;
        Waitcount=0;
    }

}

void tracking_movement(void){
    CircleFlag = TRUE;
//    strcpy(display_line[0], " TRACKING ");
//    display_changed = TRUE;

    // It reached the first circle and it needs to stop before completing it
    if(Circlecount > (BEGINCIRCLE-100) && Circlecount < (BEGINCIRCLE + TWENTYSEC)){
        strcpy(display_line[0], "BL  CIRCLE");
            display_changed = TRUE;
        RIGHT_FORWARD_SPEED = WHEEL_OFF;
        RIGHT_REVERSE_SPEED = WHEEL_OFF;
        LEFT_FORWARD_SPEED = WHEEL_OFF;
        LEFT_REVERSE_SPEED = WHEEL_OFF;
    }
    else{
        strcpy(display_line[0], " TRACKING ");
        display_changed = TRUE;
        RIGHT_FORWARD_SPEED = WHEEL_OFF;
        RIGHT_REVERSE_SPEED = WHEEL_OFF;
        LEFT_FORWARD_SPEED = WHEEL_OFF;
        LEFT_REVERSE_SPEED = WHEEL_OFF;

        // IT IS ON THE LINE
        if(ADC_Detection_L >= BLACKDETECTL && ADC_Detection_R >= BLACKDETECTR){
            Lostcount = 0;
            LostFlag = FALSE;
            strcpy(display_line[0], " STRAIGHT ");
            display_changed = TRUE;
            RIGHT_FORWARD_SPEED = STRAIGHTMOVE;
            LEFT_FORWARD_SPEED = STRAIGHTMOVE;
            // THE LEFT SENSOR IS MISSING, BUT RIGHT SENSOR IS ON THE LINE
        }else if(ADC_Detection_L < BLACKDETECTL && ADC_Detection_R >= BLACKDETECTR){
            Lostcount = 0;
            LostFlag = FALSE;
            strcpy(display_line[0], "  RIGHT   ");
            display_changed = TRUE;
            RIGHT_FORWARD_SPEED = TURNSPEED;
            LEFT_FORWARD_SPEED = WHEEL_OFF;
            DirectionTurn = RIGHT;
            // THE RIGHT SENSOR IS MISSING I NEED TO TURN LEFT TOWARD THE LINE
        }else if(ADC_Detection_R < BLACKDETECTR && ADC_Detection_L >= BLACKDETECTL){
            Lostcount = 0;
            LostFlag = FALSE;
            strcpy(display_line[0], "   LEFT   ");
            display_changed = TRUE;
            LEFT_FORWARD_SPEED = TURNSPEED;
            RIGHT_FORWARD_SPEED = WHEEL_OFF;
            DirectionTurn = LEFT;
            // IT IS WAYYY OFF AND CANNOT FIND THE LINE
        }else
            //        if(ADC_Detection_L < BLACKL && ADC_Detection_R < BLACKR)
        {

            //-----------------------------------------------------------------------------
            //---------------------      LOST MODE   ------------------------------------
           //-----------------------------------------------------------------------------

            //        strcpy(display_line[3], "   LOST   ");
            //        display_changed = TRUE;
            LEFT_FORWARD_SPEED = WHEEL_OFF;
            RIGHT_FORWARD_SPEED = WHEEL_OFF;
            LEFT_REVERSE_SPEED = WHEEL_OFF;
            RIGHT_REVERSE_SPEED = WHEEL_OFF;
            LostFlag = TRUE;
            if(Lostcount>= 200){
                RIGHT_FORWARD_SPEED = SPEED1;
                LEFT_FORWARD_SPEED = SPEED1;

            }
            else{
                RIGHT_REVERSE_SPEED = SPEED1;
                LEFT_REVERSE_SPEED = SPEED1;
            }
        }
    }

        FlagSpinR = TRUE;
        if(SpincountR >= TWOMIN){
            state = END;
            SpincountR = 0;
            FlagSpinR = FALSE;
        }

}




void rightexit(void){
    FlagWait = TRUE;
    //RIGHT
    RIGHT_FORWARD_SPEED = TURNSPEED;
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    if(Waitcount >= SEMICIRCLEDIST){
        RIGHT_FORWARD_SPEED = WHEEL_OFF;
        LEFT_FORWARD_SPEED = WHEEL_OFF;
        FlagWait = FALSE;
        Waitcount = 0;
        state = WAITS;
    }
}

void leftexit(void){
    FlagWait = TRUE;
    //RIGHT
    LEFT_FORWARD_SPEED = TURNSPEED;
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    if(Waitcount >= SEMICIRCLEDIST){
        RIGHT_FORWARD_SPEED = WHEEL_OFF;
                LEFT_FORWARD_SPEED = WHEEL_OFF;
        FlagWait = FALSE;
        Waitcount = 0;
        state = WAITS;
    }
}




void waits(void){
    FlagWait = TRUE;
            strcpy(display_line[0], "  WAIT S  ");
            display_changed = TRUE;
            RIGHT_FORWARD_SPEED = WHEEL_OFF;
            LEFT_REVERSE_SPEED = WHEEL_OFF;
            RIGHT_REVERSE_SPEED = WHEEL_OFF;
            LEFT_FORWARD_SPEED = WHEEL_OFF;

            if(Waitcount >= PAUSE){
                state = STRAIGHTEXIT;
                FlagWait = FALSE;
                Waitcount=0;
            }

}

void straight_exit(void){
    strcpy(display_line[0], "STRAIGHTW ");
    display_changed = TRUE;
    FlagWait = TRUE;
    RIGHT_FORWARD_SPEED = STRAIGHTMOVE;
    LEFT_FORWARD_SPEED = STRAIGHTMOVE;

          if(Waitcount >= STRAIGHTDISTW){
              Off_Case();
              strcpy(display_line[0], "BL FINISH ");
              display_changed = TRUE;
              state = WAIT;
              FlagWait = FALSE;
              Waitcount = 0;
          }
}


void end_state(void){
    strcpy(display_line[0], " STOPPED ");
    update_display = TRUE;
    display_changed = 1;
    Off_Case();
    SpincountL = 0;
    FlagSpinR = FALSE;
    FlagWait = 0;
    Waitcount = 0;
    SpincountR = 0;
    state = WAIT;

}


void Off_Case(void){
    //    Forward_Off();
    //    Reverse_Off();
    RIGHT_FORWARD_SPEED = WHEEL_OFF;   // P6.1 Right Forward PWM duty cycle

    LEFT_FORWARD_SPEED = WHEEL_OFF;    // P6.2 Right Forward PWM duty cycl

    RIGHT_REVERSE_SPEED = WHEEL_OFF;   // P6.3 Left Forward PWM duty cycle

    LEFT_REVERSE_SPEED = WHEEL_OFF;
}
