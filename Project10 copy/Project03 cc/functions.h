//******************************************************************************
//
//  Description: This file contains the Function prototypes
//
//  Jim Carlson
//  Aug 2013
//  Built with IAR Embedded Workbench Version: V4.10A/W32 (5.40.1)
//******************************************************************************
// Functions

// Main
void main(void);

// Initialization
void Init_Conditions(void);

// Interrupts
void enable_interrupts(void);
__interrupt void Timer0_B0_ISR(void);
__interrupt void eUSCI_A0_ISR(void);
__interrupt void eUSCI_A1_ISR(void);

// serial
void Init_Serial_UCA0(void);
void USCI_A0_transmit(void);
void transmit_receive(char line, char loc);

// Analog to Digital Converter
void Init_ADC(void);
// Clocks
void Init_Clocks(void);

// LED Configurations
void Init_LEDs(void);
void IR_LED_control(char selection);
void Backlite_control(char selection);

  // LCD
void Display_Process(void);

// Ports
void Init_Ports(void);
void Init_Port1(void);
void Init_Port2(void);
//void Init_Port3(char smclk);
void Init_Port3(void);
void Init_Port4(void);
void Init_Port5(void);
void Init_Port6(void);

// Switches
void Switch1_Process(void);
void Switch2_Process(void);

// Timers
void Init_Timers(void);
void Init_Timer_B0(void);
void Init_Timer_B3(void);





