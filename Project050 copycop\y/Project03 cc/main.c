//------------------------------------------------------------------------------
//
//  Description: This file contains the Main Routine - "While" Operating System
//
//  Jim Carlson
//  Jan 2023
//  Built with Code Composer Version: CCS12.4.0.00007_win64
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include  "msp430.h"
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"
#include "strings.h"

// Function Prototypes
void main(void);
void Init_Conditions(void);
void Display_Process(void);
void Init_LEDs(void);
void Init_Timer_B0(void);
void Carlson_StateMachine(void);
void Wheels_Stop(void);
void Wheels_Forward(void);
void Wheels_Reverse(void);
void Wheels_clkw(void);
void Wheels_counterclkw(void);
void Run_Straight(void);
void Run_Circle(void);
void Run_Figure_Eight(void);
void Run_Triangle(void);
void motor_run_forward(void);

  // Global Variables
volatile char slow_input_down;
extern char display_line[4][11];
extern char *display[4];
unsigned char display_mode;
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
extern volatile unsigned int Time_Sequence;
extern volatile char one_time;
extern unsigned int test_value;
extern unsigned int tmr_ms_count;
char chosen_direction;
char change;
unsigned int wheel_move;
unsigned int Last_Time_Sequence;
unsigned int cycle_time;
unsigned int time_change;
extern unsigned int cir_count;
extern unsigned int tri_count;
extern unsigned int okay_to_look_at_switch1;
extern unsigned int sw1_position;
char forward;
extern char transmit_state;
extern unsigned int baud_toggle;
volatile char Rx_display[SMALL_RING_SIZE];

extern unsigned int event;
extern unsigned int state;


extern unsigned int ADC_Left_Detect;
extern unsigned int ADC_Right_Detect;
extern unsigned int ADC_thumb;
unsigned int on_line;
extern unsigned int transmit_done;
unsigned int clear_display;

extern unsigned int transmit_count;

char process_buffer[25];
//extern char NCSU_str[10] = "NCSU  #1";

//void main(void){
void main(void){
//    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

//------------------------------------------------------------------------------
// Main Program
// This is the main routine for the program. Execution of code starts here.
// The operating system is Back Ground Fore Ground.
//
//------------------------------------------------------------------------------
  PM5CTL0 &= ~LOCKLPM5;
// Disable the GPIO power-on default high-impedance mode to activate
// previously configured port settings

  Init_Ports();                        // Initialize Ports
  Init_Clocks();                       // Initialize Clock System
  Init_Conditions();                   // Initialize Variables and Initial Conditions
  Init_Timers();
  Wheels_Stop();
  Init_LCD();                          // Initialize LCD
//  Init_ADC();
  Init_Serial_UCA0();
//Init_Serial_UCA1();
//P2OUT &= ~RESET_LCD;
  // Place the contents of what you want on the display, in between the quotes
// Limited to 10 characters per line
/*  strcpy(display_line[0], "   NCSU   ");
  strcpy(display_line[1], " WOLFPACK ");
  strcpy(display_line[2], "  ECE306  ");
  strcpy(display_line[3], "  GP I/O  ");
  display_changed = TRUE;*/
//  Display_Update(0,0,0,0);

  strcpy(display_line[0], "          ");
  strcpy(display_line[1], "          ");
  strcpy(display_line[2], "          ");
  strcpy(display_line[3], "          ");
  display_changed = TRUE;

  clear_display = 1;
  baud_toggle = 0;
  USCI_A0_transmit();


  transmit_state = WAIT;

  motor_off();
  wheel_move = 0;
  forward = TRUE;

  cir_count = 0;
  tri_count = 0;
  state = IDLE;
  event = IDLE;
  okay_to_look_at_switch1 = 1;
  sw1_position = 1;
//------------------------------------------------------------------------------
// Begining of the "While" Operating System
//------------------------------------------------------------------------------
  while(ALWAYS) {                      // Can the Operating system run
//    Carlson_StateMachine();            // Run a Time Based State Machine
//    Switches_Process();                // Check for switch state change
    Display_Process();                 // Update Display
    motor_check();
    P3OUT ^= TEST_PROBE;               // Change State of TEST_PROBE OFF

    switch(transmit_state){
        case WAIT:
            strcpy(display_line[0], "  WAITING ");
            strcpy(display_line[1], "          ");
            if (!baud_toggle){
                strcpy(display_line[2], "  115200  ");
            }
            else {
                strcpy(display_line[2], "  460800  ");
            }
            strcpy(display_line[3], "          ");
            display_changed = TRUE;
            break;
        case RECEIVE:
            strcpy(display_line[0], "  RECEIVE ");
//            strcpy(display_line[1], "          ");
//            strcpy(display_line[2], "          ");
            strcpy(display_line[3], Rx_display);
            display_changed = TRUE;
            break;
        case TRANSMIT:
            strcpy(display_line[0], " TRANSMIT ");
            strcpy(display_line[1], Rx_display);
//            strcpy(display_line[2], "          ");
//            strcpy(display_line[3], "          ");
            display_changed = TRUE;
            if (transmit_count >= 25){
                transmit_state = WAIT;
                transmit_count = 0;
                int ride = 0;
                while (Rx_display[ride] != 0x00){
                    Rx_display[ride++] = 0;
                }
            }
            ;
        default: break;
    }

  }
}
//------------------------------------------------------------------------------






