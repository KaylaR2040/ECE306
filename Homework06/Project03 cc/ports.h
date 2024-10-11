#define FALSE                  (0x00) //
#define TRUE                   (0x01) //
#define MOTOR                  (0x00) //
#define SMCLK_OFF              (0x00) //
#define SMCLK_ON               (0x01) //
#define PORTS                  (0x00) // RED LED 0
#define PWM_MODE               (0x01) // GREEN LED 1
#define WHEEL_OFF              (0x00)
#define WHEEL_PERIOD          (10000)
#define RIGHT_FORWARD_SPEED (TB3CCR2)
#define RIGHT_REVERSE_SPEED (TB3CCR3)
#define LEFT_FORWARD_SPEED  (TB3CCR4)
#define LEFT_REVERSE_SPEED  (TB3CCR5)
#define STEP                   (2000)
#define FORWARD                (0x00) // FORWARD
#define REVERSE                (0x01) // REVERSE


// Port 1 Pins
#define RED_LED (0x01) // 0 RED LED 0
#define A1_SEEED (0x02) // 1 A1_SEEED
#define V_DETECT_L (0x04) // 2 V_DETECT_L
#define V_DETECT_R (0x08) // 3 V_DETECT_R
#define A4_SEEED (0x10) // 4 A4_SEEED
#define V_THUMB (0x20) // 5 V_THUMB
#define UCA0TXD (0x40) // 6 Back Channel UCA0TXD
#define UCA0RXD (0x80) // 7 Back Channel UCA0RXD



// Port 2 Pins
#define SLOW_CLK (0x01) // 0 SLOW_CLK
#define CHECK_BAT (0x02) // 1 Check ADC Voltages
#define IR_LED (0x04) // 2 IR_LED
#define SW2 (0x08) // 3 SW2
#define IOT_RUN_CPU (0x10) // 4 IOT_RUN_CPU
#define DAC_ENB (0x20) // 5 DAC_ENB
#define LFXOUT (0x40) // 6 XOUTR
#define LFXIN (0x80) // 7 XINR

// Port 3 Pins
#define TEST_PROBE (0x01) // 0 TEST PROBE
#define OA20 (0x02) // 1 OA2O
#define OA2N (0x04) // 2 OA2N
#define OA2P (0x08) // 3 OA2P
#define SMCLK_OUT (0x10) // 4 SMCLK_OUT
#define DAC_CTRL (0x20) // 5 DAC_CTRL signal from Processor
#define IOT_LINK_CPU (0x40) // 6 IOT_LINK_CPU
#define IOT_RN_CPU (0x80) // 7 IOT_RN_CPU

// Port 4 Pins
#define RESET_LCD (0x01) // 0 RESET_LCD
#define SW1 (0x02) // 1 SW1
#define UCA1RXD (0x04) // 2 Back Channel UCA1RXD
#define UCA1TXD (0x08) // 3 Back Channel UCA1TXD
#define UCB1_CS_LCD (0x10) // 4 Chip Select
#define UCB1CLK (0x20) // 5 SPI mode - clock output—UCB1CLK
#define UCB1SIMO (0x40) // 6 UCB1SIMO
#define UCB1SOMI (0x80) // 7 UCB1SOMI

// Port 5 Pins
#define V_BAT (0x01) // 0  V_BAT
#define V_5 (0x02) // 1 V_5
#define CV_DAC (0x04) // 2 CV_DAC
#define V3_3 (0x08) // 3 V3_3
#define IOT_BOOT_CPU (0x10) // 4 IOT_BOOT_CPU

// Port 6 Pins
#define LCD_BACKLITE (0x01) // 0 LCD_BACKLITE
#define R_FORWARD (0x02) // 1 R_FORWARD
#define L_FORWARD (0x04) // 2 L_FORWARD
#define R_REVERSE (0x08) // 3 R_REVERSE
#define L_REVERSE (0x10) // 4 L_REVERSE
#define P6_5 (0x20) // 5 P6_5
#define GRN_LED (0x40) // 6 GRN_LED
