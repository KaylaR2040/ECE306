
//===========================================================================
// File Name : Display.c
//
// Description: This file contains the Initialization for display aspects
//
// Author: Kayla Radu
// Date: Sept 12, 2024
// Compiler: Built with Code Composer Studio Version: 12.8.0.00012
//===========================================================================

#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"
#include "ports.h"
#include "ADC.h"

//Variables
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;

extern volatile unsigned int backlight;
extern volatile unsigned char backlight_changed;
extern char adc_char[10];
extern char display_line[4][11];

void Display_Process(void){
  if(update_display){
    update_display = 0;
    if(display_changed){
      display_changed = 0;
      Display_Update(0,0,0,0);
    }
  }
}

void Backlight_Process(void){
    if(backlight_changed){
        backlight_changed = 0;
        if(backlight){
            P6OUT |= LCD_BACKLITE;
        }
        else {
            P6OUT &= ~LCD_BACKLITE;
        }
    }
}

//-------------------------------------------------------------
// ADC Line insert
// Take the HEX to BCD value in the array adc_char and place it
// in the desired location on the desired line of the display.
// char line => Specifies the line 1 thru 4
// char location => Is the location 0 thru 9
//
//-------------------------------------------------------------
void adc_line(int line, int location){
//-------------------------------------------------------------
int i;
unsigned int real_line;
real_line = line - 1;
for(i=0; i < 4; i++) {
display_line[real_line][i+location] = adc_char[i];
}
update_display = TRUE;
display_changed = TRUE;
}
//-------------------------------------------------------------

