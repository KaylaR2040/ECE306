//===========================================================================
// File Name : macros.h
//
// Description: This file contains macro definitions for movement commands, 
//              state commands, timing constants, travel distances, speed control, 
//              button states, timer intervals, and serial communication.
//
// Author: Kayla Radu
// Date: November 21, 2024
// Compiler: Built with Code Composer Studio Version: 12.8.0.00012
//===========================================================================


#ifndef MACROS_H_
#define MACROS_H_

#define ALWAYS                  (1)
#define RESET_STATE             (0)
#define RED_LED              (0x01) // RED LED 0
#define GRN_LED              (0x40) // GREEN LED 1
#define TEST_PROBE           (0x01) // 0 TEST PROBE
#define TRUE                 (0x01) //
#define MCLK_FREQ_MHZ           (8) // MCLK = 8MHz

/* ---------------------------- IOT Commands ---------------------------- */
//------------------------------------------------------------------------------
// Timing Constants (in seconds or units)
//------------------------------------------------------------------------------
#define FIFTEENSEC       (15)    // 15-second threshold to check IoT initialization
#define TENSEC           (10)   // 10-second threshold before enabling IoT
#define HUNDRED_UNITS    (100)   // Threshold for intermediate IoT timer check
#define ONEFIFTY_UNITS   (150)   // 150 units threshold to start initialization process
//------------------------------------------------------------------------------
// Buffer Size Limits
//------------------------------------------------------------------------------
#define RESPONSE_BUFFER_MAX  (31)    // Maximum buffer size for response parsing
#define TX_BUFFER_MAX        (31)    // Maximum buffer size for transmission index
#define PROCESSBUFF_MAX (25)
//------------------------------------------------------------------------------
// Character Indices for Parsing
//------------------------------------------------------------------------------
#define RESPONSE_CR_INDEX         (2)   // Index for checking '\r' in the response buffer
#define RESPONSE_X_INDEX          (10)  // Index for checking 'X' in the response buffer
#define RESPONSE_8_INDEX          (23)   // Offset for checking '8' in the response buffer
#define RESPONSE_COMMA_INDEX      (9)   // Index for checking ',' in the response buffer
#define RESPONSE_QUOTE_INDEX      (8)   // Index for checking '"' in the response buffer
#define MOVEMENT_LIMIT            (3000) // Threshold for stopping movement due to timeout


/* ---------------------------- Movement Commands ---------------------------- */

#define NONE ('N')             // No movement
#define LEFT ('L')             // Turn left
#define RIGHT ('R')            // Turn right
#define CIRCLE ('C')           // Circle movement
#define TRIANGLE ('T')         // Triangle movement
#define FORWARDS ('F')         // Move forward
#define BACK ('B')             // Move backward
#define CLKW ('K')             // Clockwise movement
#define COUNTERCLKW ('U')      // Counter-clockwise movement
#define INTERCEPT ('X')        // Intercept movement

/* ---------------------------- State Commands ------------------------------ */

#define START ('S')            // Start state
#define RUN ('R')              // Run state
#define WAIT ('W')             // Wait state
#define END ('E')              // End state
#define ARRIVED ('A')          // CMD for it to send the mat message
#define EXIT ('H')             // Exit state

/* --------------------------- Timing Constants ----------------------------- */

#define WHEEL_COUNT_TIME (10)                // Time for wheel count
#define RIGHT_COUNT_TIME (7)                 // Time for right wheel count
#define LEFT_COUNT_TIME (8)                  // Time for left wheel count
#define WAITING2START (50)                   // Waiting time for controls not this project 10
#define DEBOUNCE_TIME (100)                  // Debounce delay time in ms
#define DEBOUNCE_RESTART (0)                 // Restart debounce counter
#define ONESEC (5)                           // 1-second timer constant
#define MS_COUNT (20)                        // Millisecond count for timing

/* -------------------------- Travel Distances ----------------------------- */

#define TRAVEL_DISTANCE (18)                 // Standard travel distance
#define CIRCLE_TRAVEL_DISTANCE (200)         // Travel distance for circle
#define TRIANGLE_TURN_DISTANCE (17)          // Turn distance for triangle

/* -------------------------- Directional Timings -------------------------- */

#define COUNTERCLKW_RIGHT_COUNT_TIME (240)   // Right count time for counter-clockwise
#define CLKW_RIGHT_COUNT_TIME (1)            // Right count time for clockwise

/* --------------------------- Wheel Period Constants ----------------------- */

#define WHEEL_PERIOD (50005)                 // Wheel period for timing

/* -------------------------- Speed and Turn Values ------------------------- */

#define SMALL (15000)                        // Small angle turn
#define NINETY (37515)                       // 90-degree turn
#define FOURFIVE (28135)                     // 45-degree turn

/* -------------------------- Speed Control Constants ----------------------- */

#define SLOW (20000)                         // Slow speed
#define FAST (35000)                         // Fast speed
#define FASTRIGHT (21500)                    // Fast speed for right turns
#define FASTLEFT (20000)                     // Fast speed for left turns
#define FASTRIGHTB (35000)                   // Alternate fast speed for right turns
#define FASTLEFTB (35400)                    // Alternate fast speed for left turns
#define PERCENT_100 (50000)                  // 100% LED
#define PERCENT_80 (45000)                   // 80% LED

/* ---------------------------- Button States ------------------------------- */

#define PRESSED (0)                          // Button pressed state
#define RELEASED (1)                         // Button released state

/* --------------------------- Timer Intervals ------------------------------ */

#define TB0CCR0_INTERVAL (6250)              // Timer B0 CCR0 interval
#define TB0CCR1_INTERVAL (50000)             // Timer B0 CCR1 interval
#define TB0CCR2_INTERVAL (625)               // Timer B0 CCR2 interval

/* ---------------------------- Serial Communication ------------------------ */

#define BEGINNING (0)                        // Start position in ring buffer
#define SMALL_RING_SIZE (16)                 // Size of small ring buffer
#define LARGE_RING_SIZE (32)                 // Size of large ring buffer
#define SSID_SIZE (10)                       // SSID size for network communication

#endif /* MACROS_H_ */
