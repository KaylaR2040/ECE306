/*
 * serial.c
 *
 *  Created on: Nov 5, 2024
 *      Author: kaylaradu
 */

#include  "msp430.h"
#include  <string.h>
#include "ADC.h"
#include  "functions.h"
#include  "LCD.h"
#include "macros.h"
#include "ports.h"
#include "switches.h"
#include "timersB0.h"
#include "wheels.h"

void Init_Serial_UCA0(unsigned int baud_rate) {
    UCA0CTLW0 = UCSWRST;                   // Put eUSCI in reset
    UCA0CTLW0 |= UCSSEL__SMCLK;            // SMCLK
    UCA0BRW = (baud_rate == 115200) ? 4 : 1; // Set baud rate divisor
    UCA0MCTLW = 0x5551;                    // Set modulation pattern
    UCA0CTLW0 &= ~UCSWRST;                 // Initialize eUSCI
    UCA0IE |= UCRXIE;                      // Enable USCI_A0 RX interrupt
}

void Set_Baud_Rate(unsigned int baud_rate) {
    if (baud_rate == 115200) {
        Init_Serial_UCA0(115200);
        Init_Serial_UCA1(115200);
    } else if (baud_rate == 460800) {
        Init_Serial_UCA0(460800);
        Init_Serial_UCA1(460800);
    }
}

void Transmit_Data(const char *data) {
    while (*data) {
        while (!(UCA1IFG & UCTXIFG));  // Wait until TX buffer is ready
        UCA1TXBUF = *data++;           // Transmit data
    }
}


void Init_UART(void) {
    UCA0CTLW0 = UCSSEL__SMCLK; // Use SMCLK for UCA0
    UCA1CTLW0 = UCSSEL__SMCLK; // Use SMCLK for UCA1

    // Set baud rate to 115200 for UCA0 and UCA1
    UCA0BRW = 8;
    UCA0MCTLW = 0xD600;

    UCA1BRW = 8;
    UCA1MCTLW = 0xD600;

    // Enable UART modules
    UCA0CTLW0 &= ~UCSWRST;
    UCA1CTLW0 &= ~UCSWRST;

    // Enable receive interrupts for UCA0 and UCA1
    UCA0IE |= UCRXIE;
    UCA1IE |= UCRXIE;
}

void Change_Baud_Rate(unsigned int new_baud_rate) {
    UCA0CTLW0 |= UCSWRST; // Enter reset state
    UCA1CTLW0 |= UCSWRST; // Enter reset state

    if (new_baud_rate == 115200) {
        UCA0BRW = 8;
        UCA0MCTLW = 0xD600;

        UCA1BRW = 8;
        UCA1MCTLW = 0xD600;
    } else if (new_baud_rate == 460800) {
        UCA0BRW = 2;
        UCA0MCTLW = 0xBB00;

        UCA1BRW = 2;
        UCA1MCTLW = 0xBB00;
    }

    UCA0CTLW0 &= ~UCSWRST; // Exit reset state
    UCA1CTLW0 &= ~UCSWRST;
}
