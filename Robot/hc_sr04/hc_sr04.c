/******************************************************************************

	HC-SR04 ULTRASONIC DISTANCE SENSOR IMPLEMENTATION FILE

	This file contains implementations to handle HC-SR04.

	Atmel-8271J-AVR- ATmega-Datasheet_11/2015 is referenced as "the datasheet"
	in some comments.

	Created: 2020-09-21
	Author: Mattias Ahle, mattias.ahle@gmail.com

	Last update: 2020-10-15
    Author: Mattias Ahle

******************************************************************************/

#define F_CPU 16000000UL
#define HC_SR04_TRIG_PIN PINB1
#define HC_SR04_ECHO_PIN PIND3

#include "hc_sr04.h"
#include "timer1.h"
#include "int1.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>



/******************************************************************************
	FUNCTION PROTOTYPES
******************************************************************************/
void hc_sr04_init(void);
uint16_t hc_sr04_get_distance(void);
static void send_trig_signal(void);
static uint8_t echo_is_detected(void);
/*****************************************************************************/



/******************************************************************************
	INTERRUPT SERVICE ROUTINE

	Depending on where (INT0 @ PORTD2 or INT1 @ PORTD3) the HC-SR04 echo pin
	is connected, use ISR(INT0_vect) or ISR(INT1_vect).

	The interrupt shall be set up to trigger on any logical change.
******************************************************************************/
ISR(INT1_vect) {
	if (echo_is_detected()) {
		timer1_start();
	} else {
		timer1_stop();
	}
}
/*****************************************************************************/



/******************************************************************************
	This function initializes HC-SR04.

	Inputs:		void
	Outputs:	void
	Calls:		timer1_init();
				int1_init();
******************************************************************************/
void hc_sr04_init(void) {
	DDRB |= (1 << HC_SR04_TRIG_PIN);	//Trig pin set as output
	DDRD &= ~(1 << HC_SR04_ECHO_PIN);	//Echo pin set as input

	timer1_init();
	int1_init();
}
/*****************************************************************************/



/******************************************************************************
	This function senses the distance to an object. It returns the distance
	in centimeters.

	Inputs:		void
	Outputs:	uint16_t
	Calls:		send_trig_signal()
				timer1_getMicros()
				timer1_reset()
******************************************************************************/
uint16_t hc_sr04_get_distance(void) {
	send_trig_signal();
	uint16_t distance = timer1_get_micros() * 0.017;
	timer1_reset();

	return distance;
}
/*****************************************************************************/



/******************************************************************************
	This function sends a trigger signal to the HC-SR04. The trigger signal
	triggers an ultrasonic output burst from the HC-SR04 used to measure the
	distance to an object.

	Inputs:			void
	Outputs:		void
	Called by:		hc_sr04_sense_distance()
	Calls:			none
******************************************************************************/
static void send_trig_signal(void) {
	PORTB &= ~(1 << HC_SR04_TRIG_PIN);	//Trig pin low
	_delay_us(2);						//2µs delay
	PORTB |= (1 << HC_SR04_TRIG_PIN);	//Trig pin high
	_delay_us(10);						//10µs delay
	PORTB &= ~(1 << HC_SR04_TRIG_PIN);	//Trig pin low
}
/*****************************************************************************/



/******************************************************************************
	This function returns if the HC-SR04 echo pin is high, i.e. the HC-SR04
	is returning an echo signal to be timed.

	Inputs:			void
	Outputs:		uint8_t
	Called by:		ISR(INT1_vect)
	Calls:			none
******************************************************************************/
static uint8_t echo_is_detected(void) {
	return PIND & (1 << HC_SR04_ECHO_PIN);
}
/*****************************************************************************/