//===========================================================================
// File Name : wheels.h
//
// Description: This file contains macro definitions for wheel control, 
//              movement commands, sensor thresholds, timing constants, 
//              and speed settings.
//
// Author: Kayla Radu
// Date: September 25, 2024
// Compiler: Built with Code Composer Studio Version: 12.8.0.00012
//===========================================================================

#ifndef WHEELS_H_
#define WHEELS_H_

/* ---------------------------- Wheel Timing Constants ---------------------------- */

#define WHEEL_COUNT_TIME       (10)   // Time for wheel count
#define RIGHT_COUNT_TIME       (7)    // Time for right wheel count
#define LEFT_COUNT_TIME        (8)    // Time for left wheel count
#define ONESEC                 (5)    // 1-second timer constant

/* ---------------------------- Movement Commands ---------------------------- */

#define NONE                   ('N')  // No movement
#define FORWARD                ('F')  // Move forward
#define REVERSE                ('R')  // Move in reverse

/* ---------------------------- Detection and Tracking ---------------------------- */

#define DETECTED               ('D')  // Detection state
#define SPIN                   ('O')  // Spin movement
#define TRACK                  ('T')  // Track movement
#define TRACKWAIT              ('K')  // Track and wait
#define STRAIGHTLINE           ('U')  // Move in a straight line
#define STRAIGHTWAIT           ('V')  // Wait while straight
#define STRAIGHTEXIT           ('^')  // Exit straight movement
#define WAITS                  ('#')  // General wait command
#define STRAIGHTW              ('@')  // Straight with waiting condition

/* ---------------------------- Semi-Circle Commands ---------------------------- */

#define SEMICIRCLERT           (')')  // Semi-circle right turn
#define SEMICIRRRT             (30000) // Semi-circle right turn speed
#define SEMICIRRLT             (14000) // Semi-circle left turn speed
#define SEMICIRCLEDIST         (11)    // Distance for semi-circle movement

/* ---------------------------- Direction Indicators ---------------------------- */

#define RIGHT                  (1)    // Right direction
#define LEFT                   (0)    // Left direction

/* ---------------------------- Sensor Thresholds ---------------------------- */

#define BLACKRSP               (720)  // Right sensor threshold (spin)
#define BLACKLSP               (720)  // Left sensor threshold (spin)
#define BLACKR                 (500)  // Right sensor threshold
#define BLACKL                 (500)  // Left sensor threshold
#define BLACKDETECTR           (720)  // Right detection threshold
#define BLACKDETECTL           (720)  // Left detection threshold

/* ---------------------------- Speed Settings ---------------------------- */

#define FORWARDSPEED           (15000) // Speed for forward movement
#define SPINSPEED              (15000) // Speed for spin movement
#define SPINSPEEDRT            (15000) // Speed for right spin movement
#define TURNSPEED              (18000) // Speed for turning
#define SPEED1                 (15000) // General speed setting
#define STRAIGHTMOVE           (18000) // Speed for straight movement
#define STRAIGHTLEFT           (20000) // Speed for left straight movement
#define STRAIGHTRIGHT          (20000) // Speed for right straight movement

/* ---------------------------- Distance Settings ---------------------------- */

#define STRAIGHTDIST           (80)    // Distance for straight movement
#define STRAIGHTDISTW          (30)    // Distance for straight movement with wait
#define PAUSE                  (40)    // Pause duration
#define BEGINCIRCLE            (4000)  // Initial circle movement distance

/* ---------------------------- Time Settings ---------------------------- */

#define TWENTYSEC              (4000)  // 20-second duration
#define TWOMIN                 (2400)  // 2-minute duration

/* ---------------------------- Motor Control ---------------------------- */

#define MOTORAHEAD             (20)    // Motor ahead control value

/* ---------------------------- Boolean Values ---------------------------- */

#define FALSE                  (0)     // False value
#define OFF                    (0)     // Off state

#endif /* WHEELS_H_ */