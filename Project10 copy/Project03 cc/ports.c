
//===========================================================================
// File Name : ports.c
//
// Description: This file contains the Initialization for all port pins
//
// Author: Kayla Radu
// Date: Sept 12, 2024
// Compiler: Built with Code Composer Studio Version: 12.8.0.00012
//===========================================================================

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


//Init_Ports
void Init_Ports(){
    Init_Port1();
    Init_Port2();
    Init_Port3();
    Init_Port4();
    Init_Port5();
    Init_Port6();
}
// Code from Analog IO Main Slides
void Init_Port1(void){
    P1OUT = 0x00;
    P1DIR = 0x00;

    P1SEL0 &= ~RED_LED; // Set RED_LED as GP I/O
    P1SEL1 &= ~RED_LED; // Set RED_LED as GP I/O
    P1OUT |= RED_LED; // Set Red LED On
    P1DIR |= RED_LED; // Set Red LED direction to output

    P1SELC |= V_A1_SEEED; // ADC input for A1_SEEED
    P1SELC |= V_DETECT_L; // ADC input for V_DETECT_L
    P1SELC |= V_DETECT_R; // ADC input for V_DETECT_R
    P1SELC |= V_A4_SEEED; // ADC input for V_A4_SEEED
    P1SELC |= V_THUMB; // ADC input for V_THUMB

    P1SEL0 |= UCA0TXD; // UCA0TXD pin
    P1SEL1 &= ~UCA0TXD; // UCA0TXD pin
    P1SEL0 |= UCA0RXD; // UCA0RXD pin
    P1SEL1 &= ~UCA0RXD; // UCA0RXD pin
}

void Init_Port2(void){  // Configure Port 2
    P2OUT = 0x00;   // P2 set Low
    P2DIR = 0x00;   // Set P2 direction to output

    P2SEL0 &= ~SLOW_CLK;    // SLOW_CLK GPIO operation
    P2SEL1 &= ~SLOW_CLK;    // SLOW_CLK GPIO operation
    P2OUT  &= ~SLOW_CLK;    // Initial Value = Low / Off
    P2DIR  |=  SLOW_CLK;    // Direction = output

    P2SEL0 &= ~CHECK_BAT;   // CHECK_BAT GPIO operation
    P2SEL1 &= ~CHECK_BAT;   // CHECK_BAT GPIO operation
    P2OUT  &= ~CHECK_BAT;   // Initial Value = Low/ Off
    P2DIR  |=  CHECK_BAT;   // Direction = output

    P2SEL0 &= ~IR_LED;  // P2_2 GPIO operation
    P2SEL1 &= ~IR_LED;  // P2_2 GPIO operation
    P2OUT  &= ~IR_LED;  // Initial Value = Low / Off
    P2DIR  |=  IR_LED;  // Direction = input

    P2SEL0 &= ~SW2; // SW1 GPIO operation
    P2SEL1 &= ~SW2; // SW1 GPIO operation
    P2DIR  &= ~SW2; // Direction = input
    P2REN  |=  SW2; // Enable pullup resistor
    P2OUT  |=  SW2; // Configure pull-up resistor SW1
    P2IES  |=  SW2; // SW1 Hi/Lo edge interrupt
    P2IFG  &= ~SW2; // IFG SW1 cleared
    P2IE   |=  SW2; // SW1 interrupt Enabled

    P2SEL0 &= ~IOT_RUN_RED; // IOT_RUN_CPU GPIO operation
    P2SEL1 &= ~IOT_RUN_RED; // IOT_RUN_CPU GPIO operation
    P2OUT  &= ~IOT_RUN_RED; // Initial Value = Low / Off
    P2DIR  |=  IOT_RUN_RED; // Direction = input

    P2SEL0 &= ~DAC_ENB; // DAC_ENB GPIO operation
    P2SEL1 &= ~DAC_ENB; // DAC_ENB GPIO operation
    P2OUT  |=  DAC_ENB; // Initial Value = High
    P2DIR  |=  DAC_ENB; // Direction = output

    P2SEL0 &= ~LFXOUT;  // LFXOUT Clock operation
    P2SEL1 |= LFXOUT;   // LFXOUT Clock operation

    P2SEL0 &= ~LFXIN;   // LFXIN Clock operation
    P2SEL1 |= LFXIN;    // LFXIN Clock operation
}

void Init_Port3(){ //Configure Port 3
    P3SEL0 &= ~TEST_PROBE; // TEST_PROBE GPIO operation
    P3SEL1 &= ~TEST_PROBE; // TEST_PROBE GPIO operation
    P3OUT &= ~TEST_PROBE; // Initial Value = Low / Off
    P3DIR &= ~TEST_PROBE; // Direction = input

    P3SEL0 &= ~OA2O; // OA2O GPIO operation
    P3SEL1 &= ~OA2O; // OA2O GPIO operation
    P3OUT &= ~OA2O; // Initial Value = Low / Off
    P3DIR &= ~OA2O; // Direction = input

    P3SEL0 &= ~OA2N; // OA2N GPIO operation
    P3SEL1 &= ~OA2N; // OA2N GPIO operation
    P3OUT &= ~OA2N; // Initial Value = Low / Off
    P3DIR &= ~OA2N; // Direction = input

    P3SEL0 &= ~OA2P; // OA2P GPIO operation
    P3SEL1 &= ~OA2P; // OA2P GPIO operation
    P3OUT &= ~OA2P; // Initial Value = Low / Off
    P3DIR &= ~OA2P; // Direction = input

    P3SEL0 &= ~SMCLK_OUT; // SMCLK GPIO operation
    P3SEL1 &= ~SMCLK_OUT; // SMCLK GPIO operation
    P3OUT &= ~SMCLK_OUT; // Initial Value = Low / Off
    P3DIR &= ~SMCLK_OUT; // Direction = input

    P3SEL0 &= ~DAC_CNTL; // DAC_CNTL GPIO operation
    P3SEL1 &= ~DAC_CNTL; // DAC_CNTL GPIO operation
    P3OUT &= ~DAC_CNTL; // Initial Value = Low / Off
    P3DIR &= ~DAC_CNTL; // Direction = input

    P3SEL0 &= ~IOT_LINK_GRN; // IOT_LINK_GRN GPIO operation
    P3SEL1 &= ~IOT_LINK_GRN; // IOT_LINK_GRN GPIO operation
    P3OUT &= ~IOT_LINK_GRN; // Initial Value = Low / Off
    P3DIR &= ~IOT_LINK_GRN; // Direction = input

    P3SEL0 &= ~IOT_EN; // IOT_EN GPIO operation
    P3SEL1 &= ~IOT_EN; // IOT_EN GPIO operation
    P3OUT &= ~IOT_EN; // Initial Value = Low / Off
    P3DIR |= IOT_EN; // Direction = input 
}

void Init_Port4(void){ // Configure PORT 4
    P4OUT = 0x00; // P4 set Low
    P4DIR = 0x00; // Set P4 direction to output

    P4SEL0 &= ~RESET_LCD; // RESET_LCD GPIO operation
    P4SEL1 &= ~RESET_LCD; // RESET_LCD GPIO operation
    P4OUT &= ~RESET_LCD; // Initial Value = Low / Off
    P4DIR |= RESET_LCD; // Direction = output

    P4SEL0 &= ~SW1; // SW1 GPIO operation
    P4SEL1 &= ~SW1; // SW1 GPIO operation
    P4DIR  &= ~SW1; // Direction = input
    P4REN  |=  SW1; // Enable pullup resistor
    P4OUT  |=  SW1; // Configure pull-up resistor SW1
    P4IES  |=  SW1; // SW1 Hi/Lo edge interrupt
    P4IFG  &= ~SW1; // IFG SW1 cleared
    P4IE   |=  SW1; // SW1 interrupt Enabled

    P4SEL0 |= UCA1RXD; // USCI_A1 UART operation
    P4SEL1 &= ~UCA1RXD; // USCI_A1 UART operation
    P4SEL0 |= UCA1TXD; // USCI_A1 UART operation
    P4SEL1 &= ~UCA1TXD; // USCI_A1 UART operation

    P4SEL0 &= ~UCB1_CS_LCD; // UCB1_CS_LCD GPIO operation
    P4SEL1 &= ~UCB1_CS_LCD; // UCB1_CS_LCD GPIO operation

    P4OUT |= UCB1_CS_LCD; // Set SPI_CS_LCD Off [High]
    P4DIR |= UCB1_CS_LCD; // Set SPI_CS_LCD direction to output

    P4SEL0 |= UCB1CLK; // UCB1CLK SPI BUS operation
    P4SEL1 &= ~UCB1CLK; // UCB1CLK SPI BUS operation

    P4SEL0 |= UCB1SIMO; // UCB1SIMO SPI BUS operation
    P4SEL1 &= ~UCB1SIMO; // UCB1SIMO SPI BUS operation

    P4SEL0 |= UCB1SOMI; // UCB1SOMI SPI BUS operation
    P4SEL1 &= ~UCB1SOMI; // UCB1SOMI SPI BUS operation
}





// Code from Analog IO Main Slides
void Init_Port5(void){
    // P5SEL0 &= ~V_BAT;
    // P5SEL1 &= ~V_BAT;
    // P5OUT  &= ~V_BAT;
    // P5DIR  |= V_BAT;

    // P5SEL0 &= ~V_5_0;
    // P5SEL1 &= ~V_5_0;
    // P5OUT  &= ~V_5_0;
    // P5DIR  |= V_5_0;

    // P5SEL0 &= ~V_DAC;
    // P5SEL1 &= ~V_DAC;
    // P5OUT  &= ~V_DAC;
    // P5DIR  |= V_DAC;

    // P5SEL0 &= ~V_3_3;
    // P5SEL1 &= ~V_3_3;
    // P5OUT  &= ~V_3_3;
    // P5DIR  |= V_3_3;

    // P5SEL0 &= ~IOT_BOOT_CPU;
    // P5SEL1 &= ~IOT_BOOT_CPU;
    // P5OUT  &= ~IOT_BOOT_CPU;
    // P5DIR  |= IOT_BOOT_CPU;

     P5SELC |= V_BAT; // ADC input for V_BAT
     P5SELC |= V_5_0; // ADC input for V_BAT
     P5SELC |= V_DAC; // ADC input for V_DAC
     P5SELC |= V_3_3; // ADC input for V_3_3
     P5SEL0 &= ~IOT_BOOT; // IOT_BOOT GPIO operation
     P5SEL1 &= ~IOT_BOOT; // IOT_BOOT GPIO operation

}

void Init_Port6(void){

    P6OUT = 0x00; // P6 set Low
    P6DIR = 0x00; // Set P6 direction to output

    P6SEL0 &= ~LCD_BACKLITE; // LCD_BACKLITE GPIO operation
    P6SEL1 &= ~LCD_BACKLITE; // LCD_BACKLITE GPIO operation
    P6OUT |= LCD_BACKLITE;
    P6DIR |= LCD_BACKLITE; // Direction = output

    // P6SEL0 &= ~R_FORWARD; // R_FORWARD GPIO operation
    // P6SEL1 &= ~R_FORWARD; // R_FORWARD GPIO operation
    // P6OUT  &= ~R_FORWARD; // Initial Value = Low / Off
    // P6DIR  |= R_FORWARD; // Direction = output

    // P6SEL0 &= ~L_FORWARD; // L_FORWARD GPIO operation
    // P6SEL1 &= ~L_FORWARD; // L_FORWARD GPIO operation
    // P6OUT  &= ~L_FORWARD; // Initial Value = Low / Off
    // P6DIR  |= L_FORWARD; // Direction = output

    // P6SEL0 &= ~R_REVERSE; // R_REVERSE GPIO operation
    // P6SEL1 &= ~R_REVERSE; //    R_REVERSE GPIO operation
    // P6OUT  &= ~R_REVERSE; // Initial Value = Low / Off
    // P6DIR  |= R_REVERSE; // Direction = output

    // P6SEL0 &= ~L_REVERSE; //    L_REVERSE GPIO operation
    // P6SEL1 &= ~L_REVERSE; //    L_REVERSE GPIO operation
    // P6OUT  &= ~L_REVERSE; //    Initial Value = Low / Off
    // P6DIR  |= L_REVERSE; //    Direction = output

    P6SEL0 |= R_FORWARD; // R_FORWARD GIO operation
    P6SEL1 &= ~R_FORWARD; // R_FORWARD GIO operation
    P6OUT &= ~R_FORWARD; // Initial Value = Low / Off
    P6DIR |= R_FORWARD; // Direction = output

    P6SEL0 |= L_FORWARD; // L_FORWARD GIO operation
    P6SEL1 &= ~L_FORWARD; // L_FORWARD GIO operation
    P6OUT &= ~L_FORWARD; // Initial Value = Low / Off
    P6DIR |= L_FORWARD; // Direction = output

    P6SEL0 |= R_REVERSE; // R_REVERSE GIO operation
    P6SEL1 &= ~R_REVERSE; // R_REVERSE GIO operation
    P6OUT &= ~R_REVERSE; // Initial Value = Low / Off
    P6DIR |= R_REVERSE; // Direction = output

    P6SEL0 |= L_REVERSE; // L_REVERSE GIO operation
    P6SEL1 &= ~L_REVERSE; // L_REVERSE GIO operation
    P6OUT &= ~L_REVERSE; // Initial Value = Low / Off
    P6DIR |= L_REVERSE; // Direction = output

    P6SEL0 &= ~P6_5; // P6_5 GIO operation
    P6SEL1 &= ~P6_5; // P6_5 GIO operation
    P6OUT &= ~P6_5; // Initial Value = Low / Off
    P6DIR &= ~P6_5; // Direction = input

    P6SEL0 &= ~GRN_LED; // GRN_LED GIO operation
    P6SEL1 &= ~GRN_LED; // GRN_LED GIO operation
    P6OUT &= ~GRN_LED; // Initial Value = Low / Off
    P6DIR |= GRN_LED; // Direction = output


}

