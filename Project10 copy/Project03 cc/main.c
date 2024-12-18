//------------------------------------------------------------------------------
//
//  Description: This file contains the Main Routine - "While" Operating System
//
// Kayla Radu Fall 2024
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
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


// Function Prototypes
void main(void);
void Init_Conditions(void);
void Display_Process(void);
void Init_LEDs(void);
void Init_Timer_B0(void);
void Carlson_StateMachine(void);
void Wheels_clkw(void);
void Run_Figure_Eight(void);
void Wheels_forward(void);


// Global Variables
extern char display_line[4][11];
extern char *display[4];
unsigned char display_mode;
extern volatile char one_time;
extern unsigned int test_value;
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
extern volatile unsigned int Time_Sequence;
unsigned int wheel_move;
unsigned int Last_Time_Sequence;
unsigned int cycle_time;
unsigned int time_change;
extern unsigned int okay_to_look_at_switch1;
extern unsigned int sw1_position;
char forward;
extern char transmit_state;
extern unsigned int baude_toggle;
volatile char Rx_display[SMALL_RING_SIZE];

extern unsigned int event;
extern unsigned int state;
extern unsigned int Circlecount;



//IOT COMMUNICATION SHIT
volatile char website1[] = "AT+PING=\"www.google.com\"\r\n";
volatile char website2[] = "AT+PING=\"www.ncsu.edu\"\r\n";
volatile char stop_save_sett[] = "AT+SYSSTORE=0\r\n";
volatile char update_conn_sett[] = "AT+CIPMUX=1\r\n";
volatile char server_config[] = "AT+CIPSERVER=1,8080\r\n";
volatile char SSID_comm[] = "AT+CWJAP?\r\n";
volatile char IP_access[] = "AT+CIFSR\r\n";
volatile char cmd_OK[] = "AT\r\n";
volatile char IOT_Ring_Rx[LARGE_RING_SIZE];
volatile char iot_TX_buf[LARGE_RING_SIZE];
volatile char ssid_display[SSID_SIZE];
volatile unsigned int tx_index;
unsigned int command4_flag;
volatile char ip_display1[SSID_SIZE];
volatile char ip_display2[SSID_SIZE];
extern unsigned int init_completed;
extern unsigned int running_timecount;
volatile char commanding_send;
extern unsigned int running_timecount_flag;

//ADC STUFF
extern unsigned int ADC_Detection_L;
extern unsigned int ADC_Detection_R;
extern unsigned int ADC_thumb;
unsigned int on_line;
extern unsigned int transmit_done;
unsigned int clear_display;

extern unsigned int IOT_on_timer;

unsigned int response_value;
unsigned int movement;
int increment;
unsigned int setTime;

char process_buffer[PROCESSBUFF_MAX];
unsigned int displayclr = 0;
unsigned int following = 0;
char sheet = '0';
extern unsigned int movementcount;






void main(void){


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

    Init_LCD();                          // Initialize LCD
    Init_ADC();
    Init_Serial_UCA0();
    Init_Serial_UCA1();
    Init_DAC();


    strcpy(display_line[0], "          ");
    strcpy(display_line[1], "          ");
    strcpy(display_line[2], "          ");
    strcpy(display_line[3], "          ");
    display_changed = TRUE;


    USCI_A0_transmit();


    transmit_state = WAIT;

    Wheels_off();
    wheel_move = 0;
    forward = TRUE;

    // okay_to_look_at_switch1 = 1;
    // sw1_position = 1;

    IOT_on_timer = 0;
    unsigned int initial_process = 0;
    init_completed = 0;
    commanding_send = WAIT;
    state = WAIT;

    P2OUT |= IR_LED;

    //------------------------------------------------------------------------------
    // Begining of the "While" Operating System
    //------------------------------------------------------------------------------
    while(ALWAYS) {                      // Can the Operating system run
        //    Carlson_StateMachine();            // Run a Time Based State Machine

        Display_Process();                 // Update Display
        motor_failure();
        P3OUT ^= TEST_PROBE;               // Change State of TEST_PROBE OFF



            if(displayclr == 1){
                Clr_Display();
            }

            switch (init_completed){
            case 0:
                if (IOT_on_timer < FIFTEENSEC){
                    if (IOT_on_timer <= TENSEC){
                        display_changed = TRUE;
                    }
                    else{
                        P3OUT |= IOT_EN;
                        display_changed = TRUE;
                        initial_process = 1;
                    }
                }

                else if (IOT_on_timer < HUNDRED_UNITS){
                    increment = 1;
                }


                else if (IOT_on_timer < ONEFIFTY_UNITS){
                    switch (initial_process){
                    case 1:
                        increment = 0;
                        strcpy(display_line[3], "    1     ");
                        display_changed = TRUE;
                        if (iot_TX_buf[response_value++] == '\r'){

                            if (iot_TX_buf[response_value-2] == 'P'){
                                strcpy(IOT_Ring_Rx, stop_save_sett);
                                UCA1IE |= UCTXIE;
                                initial_process = 2;
                                response_value = 0;

                                while (IOT_Ring_Rx[increment] == 0x00){
                                    IOT_Ring_Rx[increment++] = 0;
                                }
                            }
                        }
                        break;
                    case 2:
                        increment= 0;
                        strcpy(display_line[3], "    2     ");
                        display_changed = TRUE;
                        if (iot_TX_buf[response_value++] == '\r'){
                            if (iot_TX_buf[response_value-2] == 'K'){
                                tx_index = 0;
                                strcpy(IOT_Ring_Rx, update_conn_sett);
                                UCA1IE |= UCTXIE;
                                initial_process = 3;
                                response_value = 0;
                            }
                        }
                        if (response_value > RESPONSE_BUFFER_MAX){
                            response_value = 0;
                        }
                        if (tx_index > TX_BUFFER_MAX){
                            tx_index = 0;
                        }
                        break;
                    case 3:
                        increment= 0;
                        strcpy(display_line[3], "    3     ");
                        display_changed = TRUE;
                        if (iot_TX_buf[response_value++] == '\r'){
                            increment = 0;
                            if (iot_TX_buf[response_value-TENSEC] == 'X'){
                                tx_index = 0;
                                strcpy(IOT_Ring_Rx, server_config);
                                UCA1IE |= UCTXIE;
                                initial_process = 4;
                                response_value = 0;
                            }
                        }
                        if (response_value > RESPONSE_BUFFER_MAX){
                            response_value = 0;
                        }
                        if (tx_index > TX_BUFFER_MAX){
                            tx_index = 0;
                        }
                        break;
                    case 4:
                        increment= 0;
                        strcpy(display_line[3], "    4     ");
                        display_changed = TRUE;
                        if (iot_TX_buf[response_value++] == '\r'){
                            increment = 0;
                            if (iot_TX_buf[response_value + RESPONSE_8_INDEX] == '8'){
                                tx_index = 0;
                                strcpy(IOT_Ring_Rx, SSID_comm);
                                UCA1IE |= UCTXIE;
                                initial_process = 5;
                                response_value = 0;
                            }
                        }
                        if (response_value > RESPONSE_BUFFER_MAX){
                            response_value = 0;
                        }
                        if (tx_index > TX_BUFFER_MAX){
                            tx_index = 0;
                        }
                        break;
                    case 5:
                        strcpy(display_line[3], "    5     ");
                        display_changed = TRUE;
                        strcpy(display_line[1], "          ");
                        strcpy(display_line[0], "          ");
                        strcpy(display_line[2], "          ");
                        strcpy(display_line[3], "          ");
                        display_changed = TRUE;
                        initial_process = 6;
                        break;
                    case 6:
                        increment= 0;
                        strcpy(display_line[3], "    6     ");
                        display_changed = TRUE;

                        if (iot_TX_buf[response_value++] == '\r'){
                            increment = 0;
                            if (iot_TX_buf[response_value-RESPONSE_COMMA_INDEX] == ','){
                                tx_index = 0;
                                strcpy(IOT_Ring_Rx, IP_access);
                                UCA1IE |= UCTXIE;
                                initial_process = 7;
                                response_value = 0;
                                command4_flag = 0;

                                display_changed = TRUE;
                                strcpy(display_line[0], "ncsu      ");
                                display_changed = TRUE;
                            }
                        }
                        //            }
                        if (response_value > RESPONSE_BUFFER_MAX){
                            response_value = 0;
                        }
                        if (tx_index > TX_BUFFER_MAX){
                            tx_index = 0;
                        }
                        break;
                    case 7:
                        increment= 0;
                        strcpy(display_line[3], "    7     ");
                        display_changed = TRUE;
                        if (iot_TX_buf[response_value++] == '\r'){
                            increment = 0;
                            if (iot_TX_buf[response_value-RESPONSE_QUOTE_INDEX] == '"'){
                                tx_index = 0;
                                strcpy(IOT_Ring_Rx, cmd_OK);
                                UCA1IE |= UCTXIE;
                                initial_process = 8;
                                response_value = 0;
                                display_changed = TRUE;
                                strcpy(display_line[0], "ncsu      ");
                                strcpy(display_line[2], ip_display1);
                                strcpy(display_line[3], ip_display2);
                                display_changed = TRUE;
                            }
                        }
                        //            }
                        if (response_value > RESPONSE_BUFFER_MAX){
                            response_value = 0;
                        }
                        if (tx_index > TX_BUFFER_MAX){
                            tx_index = 0;
                        }
                        break;
                    case 8:
                        display_changed = TRUE;
                        init_completed = 1;
                        strcpy(display_line[4], "  READY!  ");
                        display_changed = TRUE;
                        break;

                    }
                }
                break;
            case 1:
//                if(Circlecount > (10*TWENTYSEC)){
//                    strcpy(display_line[0], " BL  STOP ");
//                    display_changed = TRUE;
//                }
                if(movementcount >= MOVEMENT_LIMIT){
                    commanding_send = WAIT;

                    running_timecount_flag = 0;
                    Wheels_off();
                    running_timecount = 0;
                    movement = 0;

                }

                pingpong();
                increment = 0;
                if (iot_TX_buf[response_value++] == '^'){
                    if(iot_TX_buf[response_value] == '3' && iot_TX_buf[response_value+1] == '0' && iot_TX_buf[response_value+2] == '6'&& iot_TX_buf[response_value+3] == '1'){
                        response_value = response_value +4;
                        switch (iot_TX_buf[response_value]){
                        case 'F':
                            if(!movement){

                            setTime = (int)iot_TX_buf[response_value+1]-'0';

                            //DUFFEREBT DISTNACES FOR THE STRIAGHT LINE TO GO
                            switch(setTime){
                            case 3:
                                setTime = 17; 
                                break;
                            case 4:
                                setTime = 18;
                            break;
                            case 5:
                                setTime = 20;
                            break;
                            case 6:
                                setTime = 25;
                                break;
                            case 7:
                                setTime = 30;
                                break;
                            case 8:
                                setTime = 40;
                                break;
                            case 9:
                                setTime = 80;
                                strcpy(display_line[0], " FINISHED ");
                                    display_changed = TRUE;
                                break;
                            default: break;

                            }
                            running_timecount_flag = 1;
                            commanding_send = FORWARDS;
                            running_timecount = 0;
                            for (increment = 0; increment < 32; increment++){
                                iot_TX_buf[increment] = 0;
                            }
                            }

                            break;
                        case 'B':
                            if(!movement){
                            setTime = (int)iot_TX_buf[response_value+1]-'0';
                            if(setTime == 9){
                                setTime = ((3*9)-2);
                            }
                            running_timecount_flag = 1;
                            commanding_send = BACK;
                            running_timecount = 0;
                            for (increment = 0; increment < LARGE_RING_SIZE; increment++){
                                iot_TX_buf[increment] = 0;
                            }
                            }
                            break;
                        case 'R':
                            if(!movement){
                            setTime = (int)iot_TX_buf[response_value+1]-'0';
                            running_timecount_flag = 1;
                            commanding_send = RIGHT;
                            running_timecount = 0;
                            for (increment = 0; increment < LARGE_RING_SIZE; increment++){
                                iot_TX_buf[increment] = 0;
                            }
                            }
                            break;
                        case 'L':
                            if(!movement){
                            setTime = (int)iot_TX_buf[response_value+1]-'0';
                            running_timecount_flag = 1;
                            commanding_send = LEFT;
                            running_timecount = 0;
                            for (increment = 0; increment < LARGE_RING_SIZE; increment++){
                                iot_TX_buf[increment] = 0;
                            }
                            }
                            break;

                        case 'S':
                            if(!movement){
                            setTime = (int)iot_TX_buf[response_value+1]-'0';
                            commanding_send = WAIT;
                            running_timecount = 0;

                            for (increment = 0; increment < LARGE_RING_SIZE; increment++){
                                iot_TX_buf[increment] = 0;
                            }
                            }
                            break;

                        case 'I':
                            if(!movement){
                            setTime = (int)iot_TX_buf[response_value+1]-'0';
                            commanding_send = INTERCEPT;
                            running_timecount = 0;

                            for (increment = 0; increment < LARGE_RING_SIZE; increment++){
                                iot_TX_buf[increment] = 0;
                            }
                            }
                            break;

                        case 'A':
                            if(!movement){
                            setTime = 10;
                            sheet = iot_TX_buf[response_value+1];
                            commanding_send = ARRIVED;
                            running_timecount = 0;

                            for (increment = 0; increment < LARGE_RING_SIZE; increment++){
                                iot_TX_buf[increment] = 0;
                            }
                            }
                            break;
                        case 'H':
                            if(!movement){
                                setTime = (int)iot_TX_buf[response_value+1]-'0';
                                commanding_send = EXIT;
                                running_timecount = 0;

                                for (increment = 0; increment < LARGE_RING_SIZE; increment++){
                                    iot_TX_buf[increment] = 0;
                                }
                            }
                            break;

                        }
                    }
                }

                if (response_value > RESPONSE_BUFFER_MAX) {
                    response_value = 0;
                }

                char tempStr[10];
                switch (commanding_send){
                case WAIT:
                    Wheels_off();
                    if(setTime == 80){
                        strcpy(display_line[3], " BL  STOP ");
                        display_changed = TRUE;
                    }
                    else{

                        if (running_timecount < 50){
                            strcpy(display_line[0], "          ");
                            display_changed = TRUE;
                            centeringdisplay("IP address", '2');
                            //                strcpy(display_line[1], " RUN CASE ");
                            strcpy(display_line[0], "          ");
                            display_changed = TRUE;
                            centeringdisplay(ssid_display, '1');
                            centeringdisplay(ip_display1, '3');
                            centeringdisplay(ip_display2, '4');

                            display_changed = TRUE;
                        }
                        else{
                            strcpy(display_line[1], " WAITDONE ");
                            strcpy(display_line[3], "         W");
                            display_changed = TRUE;
                        }
                    }

                    break;
                case FORWARDS:
                    Wheels_forward();
                    strcpy(tempStr,ip_display2);
                    strcat(tempStr,"F");
                    centeringdisplay(tempStr, '4');
                    display_changed = TRUE;
                    if (running_timecount > setTime){ // was originally 10
                        running_timecount_flag = 0;
                        Wheels_off();
                        running_timecount = 0;
                        commanding_send = WAIT;
                        movement = 0;
                    }
                    break;
                case BACK:
                    Wheels_backward();
                    strcpy(tempStr,ip_display2);
                    strcat(tempStr,"B");
                    centeringdisplay(tempStr, '4');
                    display_changed = TRUE;
                    if (running_timecount > setTime){
                        running_timecount_flag = 0;
                        Wheels_off();
                        running_timecount = 0;
                        commanding_send = WAIT;
                        movement = 0;
                    }
                    break;
                case RIGHT:
                    wheels_turn_right();
                    strcpy(tempStr,ip_display2);
                    strcat(tempStr,"R");
                    centeringdisplay(tempStr, '4');
                    display_changed = TRUE;
                    display_changed = TRUE;
                    if (running_timecount >= setTime){
                        running_timecount_flag = 0;
                        Wheels_off();
                        running_timecount = 0;
                        commanding_send = WAIT;
                        movement = 0;
                    }
                    break;
                case LEFT:
                    wheels_turn_left();
                    strcpy(tempStr,ip_display2);
                    strcat(tempStr,"L");
                    centeringdisplay(tempStr, '4');
                    display_changed = TRUE;
                    if (running_timecount >= setTime){
                        running_timecount_flag = 0;
                        Wheels_off();
                        running_timecount = 0;
                        commanding_send = WAIT;
                        movement = 0;
                    }

                    break;

                case INTERCEPT:
                    // make the display cleared
                    displayclr = displayclr +1;

                    // Have it transmit the IR VALUES
                    HexToBCD(ADC_Detection_L);
                    adc_line(2,2);

                    HexToBCD(ADC_Detection_R);
                    adc_line(3,2);
                    display_changed = TRUE;

                    // STATE MACHINE
                    blacklinemachine();


                    break;
                case ARRIVED:
                    // make the display cleared
                    strcpy(display_line[0], "          ");
                    display_changed = TRUE;
                    Display_complete();
                    if (running_timecount >= setTime){
                        running_timecount_flag = 0;
                        Wheels_off();
                        running_timecount = 0;
                        commanding_send = WAIT;
                        movement = 0;
                    }

                    break;
                case EXIT:

                    if(setTime == 0){
                        state = LEFT;
                    }
                    else{
                        state = RIGHT;
                    }
                    exit_state();
                    strcpy(display_line[0], " BL  EXIT ");
                    display_changed = TRUE;


                    break;

                default: break;
                }
                break;
            }


            //    HexToBCD(ADC_Detection_L);
            //    adc_line(2,2);
            //
            //    HexToBCD(ADC_Detection_R);
            //    adc_line(3,3);

    }

}
//------------------------------------------------------------------------------

// Aligns text to Middle of Display
void centeringdisplay(char *line, char linechange) {
    int lineloc = -1; // Index of the line

    // Determine which line to update 
    if (linechange == '1') {
        lineloc = 0;
    } else if (linechange == '2') {
        lineloc = 1;
    } else if (linechange == '3') {
        lineloc = 2;
    } else if (linechange == '4') {
        lineloc = 3;
    } else {
        // BAD INPUT
        strcpy(display_line[0], "   WRONG  ");
        display_changed = TRUE;
        return;
    }

    // check if NULL
    if (line != NULL) {
        int len = strlen(line);

        if (len > 10) {
            len = 10;
        }

        // Find the number of spaces needed to center the line
        int spaces = (10 - len) / 2;

        int i;
        for (i = 0; i < 10; i++) {
            display_line[lineloc][i] = ' ';
        }

        for (i = 0; i < len; i++) {
            display_line[lineloc][spaces + i] = line[i];
        }
        display_line[lineloc][10] = '\0';

        display_changed = TRUE;
    } else {
        strcpy(display_line[lineloc], " NULL Line ");
        display_changed = TRUE;
    }
}

void blacklinemachine(void){
    following = 1;
    switch(state){
    case WAIT:
        Off_Case();
        state = STRAIGHTLINE;
        break;
    case STRAIGHTLINE:
        straightline();
        break;
    case STRAIGHTWAIT:
        straightwait();
        break;
    case SEMICIRCLERT:
        semicirclert();
        break;
    case START:
        start_movement();
        break;
    case DETECTED:
        detect_movement();
        break;
    case SPIN:
        spinning_movement();
        break;
    case TRACKWAIT:
        track_wait();
        break;
    case TRACK:
        tracking_movement();
        break;
    case STRAIGHTW:
        straightsecond();
        break;
    case END:
        end_state();

        running_timecount_flag = 0;
        running_timecount = 0;
        commanding_send = WAIT;
        movement = 0;

        break;
    default: break;
    }
}

void exit_state(void){
    switch(state){
    case RIGHT:
        rightexit();
        break;
    case LEFT:
        leftexit();
        break;
    case WAITS:
        waits();
        break;
    case STRAIGHTEXIT:
        straight_exit();
        break;
    default: break;
    }
}




