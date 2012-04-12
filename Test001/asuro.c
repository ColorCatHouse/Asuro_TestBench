/*******************************************************************************
*
* File Name:   asuro.c
* Project  :   ASURO
*
* Description: This file contains ASURO main features
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
* Copyright (c) 2003 DLR Robotics & Mechatronics
*****************************************************************************/
/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   any later version.                                                    *
***************************************************************************/

#include "asuro.h"
volatile unsigned char count72kHz;

/* uses timer2 (36kHz for IR communication */
/* counts falling and rising edge => 36kHz*2 = 72kHz */
SIGNAL (SIG_OUTPUT_COMPARE2)
{
	count72kHz ++;
}

/* Init function Processor will be initalized to work correctly */
void Init (void)
{
	//-------- seriell interface programmed in boot routine and already running -------
	//  prepare 36kHz for IR - Communication
	TCCR2 = (1 << WGM21) | (1 << COM20) | (1 << CS20);
	OCR2  = 0x6E; // 36kHz @8MHz
	TIMSK |= (1 << OCIE2); // 36kHz counter for sleep
	
	// prepare RS232 
	UCSRA = 0x00;
	UCSRB = 0x00;	
	UCSRC = 0x86; // No Parity | 1 Stop Bit | 8 Data Bit
	UBRRL = 0xCF; // 2400bps @ 8.00MHz
	
	// I/O Ports
	DDRB = IRTX | LEFT_DIR | PWM | GREEN_LED; 
	DDRD = RIGHT_DIR | FRONT_LED | ODOMETRIE_LED | RED_LED;
	
	// for PWM (8-Bit PWM) on OC1A & OC1B
	TCCR1A = (1 << WGM10) | (1 << COM1A1) | (1 << COM1B1);
	// tmr1 running on MCU clock/8 
	TCCR1B = (1 << CS11);
	
	// A/D Conversion
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1); // clk/64 
	ODOMETRIE_LED_OFF;

	FrontLED(OFF);
	BackLED(ON,ON);
	BackLED(OFF,OFF);
	StatusLED(GREEN);
	
	MotorDir(FWD,FWD);
	MotorSpeed(0,0);
	sei();
}

/* Set motor speed */
inline void MotorSpeed(unsigned char left_speed, unsigned char right_speed)
{
	OCR1A = left_speed;
	OCR1B = right_speed;
}

/* Set motor direction */
inline void MotorDir(unsigned char left_dir, unsigned char right_dir)
{
	PORTD = (PORTD &~ ((1 << PD4) | (1 << PD5))) | left_dir;
	PORTB = (PORTB &~ ((1 << PB4) | (1 << PB5))) | right_dir;
}

/* Status LED (OFF,GREEN,YELLOW,RED)*/
/* example code set StatusLED GREEN */
/* StatusLED(GREEN); */
inline void StatusLED(unsigned char color)
{
	if (color == OFF)    {GREEN_LED_OFF; RED_LED_OFF;}
	if (color == GREEN)  {GREEN_LED_ON; RED_LED_OFF;} 
	if (color == YELLOW) {GREEN_LED_ON; RED_LED_ON;}
	if (color == RED)    {GREEN_LED_OFF; RED_LED_ON;}
}

/* Front LED */
/* example code FrontLED ON */
/* FrontLED(ON); */
inline void FrontLED(unsigned char status)
{
	PORTD = (PORTD &~(1 << PD6)) | (status << PD6);
}

/* function for Break LEDs */
/* example code right LED On left LED Off */
/* BackLED(OFF,ON); */
void BackLED(unsigned char left, unsigned char right)
{
	if (left || right) {
		PORTD &= ~(1 << PD7); // Wheel LED OFF
		DDRC |= (1 << PC0) | (1 << PC1); // Output => no odometrie
		PORTC |= (1 << PC0) | (1 << PC1);
	}
	if (!left) PORTC &= ~(1 << PC1);
	if (!right) PORTC &= ~(1 << PC0);
}

/* function to read out line follow phototransistors (left,rigth) */
void LineData(unsigned int *data)
{
	ADMUX = (1 << REFS0) | IR_LEFT;	// AVCC reference with external capacitor
	ADCSRA |= (1 << ADSC);			// Start conversion
	while (!(ADCSRA & (1 << ADIF)));	// wait for conversion complete
	ADCSRA |= (1 << ADIF);			// clear ADCIF
	data[0] = ADCL + (ADCH << 8);
	
	ADMUX = (1 << REFS0) | IR_RIGHT;	// AVCC reference with external capacitor
	ADCSRA |= (1 << ADSC);			// Start conversion
	while (!(ADCSRA & (1 << ADIF)));	// wait for conversion complete
	ADCSRA |= (1 << ADIF);			// clear ADCIF
	data[1] = ADCL + (ADCH << 8);
}

/* function to read out odometrie phototransistors (left,rigth) */
void OdometrieData(unsigned int *data)
{
	DDRC &= ~((1 << PC0) | (1 << PC1)); // Input => no break LED
	ODOMETRIE_LED_ON;
	
	ADMUX = (1 << REFS0) | WHEEL_LEFT; // AVCC reference with external capacitor
	ADCSRA |= (1 << ADSC);			// Start conversion
	while (!(ADCSRA & (1 << ADIF)));	// wait for conversion complete
	ADCSRA |= (1 << ADIF);			// clear ADCIF
	data[0] = ADCL + (ADCH << 8);
	
	ADMUX = (1 << REFS0) | WHEEL_RIGHT; // AVCC reference with external capacitor
	ADCSRA |= (1 << ADSC);			// Start conversion
	while (!(ADCSRA & (1 << ADIF)));	// wait for conversion complete
	ADCSRA |= (1 << ADIF);			// clear ADCIF
	data[1] = ADCL + (ADCH << 8);
}

/* function for serial communication */
void SerWrite(unsigned char *data,unsigned char length)
{
	unsigned char i = 0;
	UCSRB = 0x08; // enable transmitter
	while (length > 0) {
		if (UCSRA & 0x20) { // wait for empty transmit buffer
			UDR = data[i++];
			length --;
		}
	}
	while (!(UCSRA & 0x40)); 
	for (i = 0; i < 0xFE; i++)
		for(length = 0; length < 0xFE; length++); 
}

void SerRead(unsigned char *data, unsigned char length,unsigned int timeout)
{
	unsigned char i = 0;
	unsigned int  time = 0;
	UCSRB = 0x10; // enable receiver
	/* non blocking */
	if (timeout != 0) {
		while (i < length && time++ < timeout) {
			if (UCSRA & 0x80) {
				data[i++] = UDR;
				time = 0;
			}
		}
		if (time > timeout) data[0] = 'T';
	}
	/* blocking */
	else {
		while (i < length) {
			if (UCSRA & 0x80) 
				data[i++] = UDR;
		}
	}	
}

/* function to read out switches */
unsigned char PollSwitch (void)
{
	unsigned int i;
	
	DDRD |= SWITCHES;				// Switches as Output
	SWITCH_ON;						// Output HIGH for measurement
	ADMUX = (1 << REFS0) | SWITCH;	// AVCC reference with external capacitor
	Sleep(10);
	
	ADCSRA |= (1 << ADSC);			// Start conversion
	while (!(ADCSRA & (1 << ADIF)));// wait for conversion complete
	ADCSRA |= (1 << ADIF);			// clear ADCIF
	i = ADCL + (ADCH << 8);
	
	SWITCH_OFF;

	return  ((unsigned char) ((( 1024.0/(float)i - 1.0)) * 63.0 + 0.5));
}

/* for working with Interrupt */
void StartSwitch(void)
{
	SWITCH_OFF;
	DDRD &= ~SWITCHES;					// Switches as Input => ext. Int 1
	MCUCR &= ~((1 << ISC11) | (1 << ISC10));// Low level generates interrupt
	GICR |= (1 << INT1);					// Enable external Interrupt 1 
}

void StopSwitch(void)
{
	GICR &= ~(1 << INT1);
}

/* uses 72kHz timer => Sleep(x) = x/72kHz [sec] */
void Sleep(unsigned char time72kHz)
{
	count72kHz = 0;
	while (count72kHz < time72kHz);
}
