/*******************************************************************************
*
* File Name:   asuro.h
* Project  :   ASURO
*
*
* Ver.     Date         Author           Comments
* -------  ----------   --------------   ------------------------------
* 1.00	   14.08.2003   Jan Grewe		 build
* 2.00     14.10.2003   Jan Grewe        LEFT_VEL, RIGHT_VEL -> MotorSpeed(unsigned char left_speed, unsigned char right_speed);
*										 LeftRwd(),LeftFwd(),RightRwd(),RigthFwd() -> MotorDir(unsigned char left_dir, unsigned char right_dir);
*                                        GREEN_ON,GREEN_OFF,RED_ON,RED_OFF -> StatusLED(unsigned char color);
*                                        LED_RED_ON, LED_RED_OFF -> FrontLED(unsigned char color);
*                                        Blink(unsigned char left, unsigned char right) -> BackLED(unsigned char left, unsigned char right);
*                                        Alles in Funktionen gefasst => leichter verständlich ?!?!
* 2.10     17.10.2003   Jan Grewe        new Timer funktion void Sleep(unsigned char time36kHz)  
*
*
* Copyright (c) 2003 DLR Robotics & Mechatronics
*****************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   any later version.                                                    *
 ***************************************************************************/

#ifndef ASURO_H
#define ASURO_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>

#define  FALSE	0
#define  TRUE	1

#define  OFF    0
#define  ON     1

#define GREEN	1
#define RED		2
#define YELLOW  3

/* Init function Processor will be initalized to work correctly */
void Init(void);

/* Set Status LED (OFF,GREEN,RED,YELLOW)*/
/* example code set Status LED GREEN */
/* StatusLED(GREEN); */
inline void StatusLED(unsigned char color);
/* function for front LED */
/* example code front LED On */
/* FrontLED(ON); */
inline void FrontLED(unsigned char status);

/* function for Back LEDs */
/* example code right LED On left LED Off */
/* BackLED(OFF,ON); */
void BackLED(unsigned char left, unsigned char right);

/* Set motor direction (FWD,RWD,BREAK,FREE)*/
/* example code set Direction for right motor to rewind */
/* MotorDir(BREAK,RWD); */
inline void MotorDir(unsigned char left_dir, unsigned char right_dir);

/* range for motor speed 0..255 */
/* example code set speed for left motor to 150 */
/* MotorSpeed(150,0); */
inline void MotorSpeed(unsigned char left_speed, unsigned char right_speed);

/* function for serial communication */
void SerWrite(unsigned char *data,unsigned char length);
/* timeout = 0 => bloking mode !!! */
void SerRead(unsigned char *data, unsigned char length, unsigned int timeout);

/* function to read out line follow phototransistors (left,rigth) */
void LineData(unsigned int *data);

/* function to read out odometrie phototransistors (left,rigth) */
void OdometrieData(unsigned int *data);

/* function to read out switches */
unsigned char PollSwitch (void);
/* for working with Interrupt */
void StartSwitch(void);
void StopSwitch(void);

/* Sleep function using 36kHz counter */
void Sleep(unsigned char timer36kHz);
/* ----------- END ------------ */


/* --------------- INTERNAL ------------- */
#define GREEN_LED_ON  PORTB |=  GREEN_LED
#define GREEN_LED_OFF PORTB &= ~GREEN_LED
#define RED_LED_ON    PORTD |=  RED_LED
#define RED_LED_OFF   PORTD &= ~RED_LED

#define FWD		(1 << PB5) /* (1 << PD5) */
#define RWD		(1 << PB4) /* (1 << PD4) */
#define BREAK	0x00
#define FREE	(1 << PB4) | (1 << PB5) /* (1 << PD4) | (1 << PD5)*/

#define IRTX        (1 << PB3)
#define GREEN_LED   (1 << PB0)
#define RED_LED     (1 << PD2)

#define PWM 	  (1 << PB1) | (1 << PB2)
#define RIGHT_DIR (1 << PB4) | (1 << PB5)
#define LEFT_DIR  (1 << PD4) | (1 << PD5)

#define SWITCHES   (1 << PD3)
#define SWITCH_ON  PORTD |= SWITCHES
#define SWITCH_OFF PORTD &= ~SWITCHES

#define BATTERIE  (1 << MUX0) | (1 << MUX2) //ADC5
#define SWITCH    (1 << MUX2)							//ADC4
#define IR_LEFT	  (1 << MUX0) | (1 << MUX1)	//ADC3
#define IR_RIGHT  (1 << MUX1)							//ADC2
#define FRONT_LED (1 << PD6)

#define ODOMETRIE_LED 	  (1 << PD7)
#define ODOMETRIE_LED_ON  PORTD |= ODOMETRIE_LED
#define ODOMETRIE_LED_OFF PORTD &= ~ODOMETRIE_LED

#define WHEEL_LEFT   (1 << MUX0)			//ADC1
#define WHEEL_RIGHT  0					//ADC0

#endif /* ASURO_H */

