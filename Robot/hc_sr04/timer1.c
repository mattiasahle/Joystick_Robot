/******************************************************************************
	TIMER1 IMPLEMENTATION FILE

	This file contains implementations to handle Timer1.

	For more information regarding the implementation, please refer to
	Atmel-8271J-AVR- ATmega-Datasheet_11/2015.

	Created: 2020-10-14
	Author: Mattias Ahle, mattias.ahle@gmail.com

	Last update: 2020-10-15
    Author: Mattias Ahle
******************************************************************************/

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>



/******************************************************************************
	PRESCALER SELECTION INFO - SELECT A PRESCALER
*******************************************************************************
	Select a prescaler which does not have a higher accuracy than necessary for
	the application. Otherwise unnecessary CPU resources will be used.

	Formula:
	Micros/TCNT1 count = (prescaler * micros/second) / F_CPU
	micros/second = 10^6
	F_CPU = 16MHz (in this example)

	Prescaler	| Micros/TCNT1	| -> value in prescaler variable below
	____________|_______________|______________________________________
	No presc	|  0.0625		| ->    1
	8 bit		|  0.5			| ->    8
	64 bit		|  4			| ->   64
	256 bit		| 16			| ->  256
	1024 bit	| 64			| -> 1024
******************************************************************************/
static const uint16_t PRESCALER = 64;
/*****************************************************************************/




/******************************************************************************
	GLOBAL VARIABLES
******************************************************************************/
//overflow_count holds number of TCNT1 overflows
static volatile uint64_t overflow_count = 0;

static const uint8_t CLOCKS_PER_MICRO = F_CPU / 1000000;

//8-bit register = 256, 16-bit register = 65536
static const uint32_t MAX_COUNTS_COUNTER_REGISTER = 65536;
/*****************************************************************************/



/******************************************************************************
	FUNCTION PROTOTYPES
******************************************************************************/
void timer1_init(void);
void timer1_start(void);
void timer1_stop(void);
void timer1_reset(void);
uint64_t timer1_get_micros(void);
/*****************************************************************************/



/******************************************************************************
	INTERRUPT SERVICE ROUTINE
******************************************************************************/
ISR(TIMER1_OVF_vect) {
	overflow_count++;
}
/*****************************************************************************/



/******************************************************************************
	This function initializes TIMER1.

	Inputs:		void
	Outputs:	void
	Calls:		sei()
******************************************************************************/
void timer1_init(void) {
	TCCR1B &= ~(1 << CS12);		//All Clock Select bits initialized to 0
	TCCR1B &= ~(1 << CS11);		//This means TIMER1 is not started yet
	TCCR1B &= ~(1 << CS10);

	TCNT1 = 0;					//Timer/Counter1 counting register init to 0

	TIMSK1 |= (1 << TOIE1);		//Timer/Counter1, Overflow Interrupt Enable

	TIFR1 |= (1 << TOV0);		//Clear Timer/Counter1, Overflow Flag

	sei();						//Enable interrupts globally
}
/*****************************************************************************/



/******************************************************************************
	This function starts TIMER1 counting by setting a prescaler.

	Inputs:		none
	Outputs:	none
	Calls:		none
******************************************************************************/
void timer1_start(void) {
	switch (PRESCALER) {
		case 1:
			TCCR1B &= ~(1 << CS12);
			TCCR1B &= ~(1 << CS11);
			TCCR1B |= (1 << CS10);
			break;
		case 8:
			TCCR1B &= ~(1 << CS12);
			TCCR1B |= (1 << CS11);
			TCCR1B &= ~(1 << CS10);
			break;
		case 64:
			TCCR1B &= ~(1 << CS12);
			TCCR1B |= (1 << CS11);
			TCCR1B |= (1 << CS10);
			break;
		case 256:
			TCCR1B |= (1 << CS12);
			TCCR1B &= ~(1 << CS11);
			TCCR1B &= ~(1 << CS10);
			break;
		case 1024:
			TCCR1B |= (1 << CS12);
			TCCR1B &= ~(1 << CS11);
			TCCR1B |= (1 << CS10);
			break;
		default:
			//Highest accuracy no prescaler as default
			TCCR1B &= ~(1 << CS12);
			TCCR1B &= ~(1 << CS11);
			TCCR1B |= (1 << CS10);
			break;
	}
}
/*****************************************************************************/



/******************************************************************************
	This function stops TIMER1 counting by setting all Clock Select (CS) bits
	to zero.

	Inputs:		none
	Outputs:	none
	Calls:		none
******************************************************************************/
void timer1_stop(void) {
	TCCR1B &= ~(1 << CS12);
	TCCR1B &= ~(1 << CS11);
	TCCR1B &= ~(1 << CS10);
}
/*****************************************************************************/



/******************************************************************************
	This function stops and resets TIMER1.

	Inputs:		none
	Outputs:	none
	Calls:		none
******************************************************************************/
void timer1_reset(void) {
	timer1_stop();
	overflow_count = 0;
	TCNT1 = 0;
}
/*****************************************************************************/



/******************************************************************************
	This function returns the total number of microseconds stored in the
	overflow count variable together with the TIMER1 counting register TCNT1.

	Inputs:		none
	Outputs:	uint64_t
	Calls:		none
******************************************************************************/
uint64_t timer1_get_micros(void) {
	static const uint8_t MICROS_PER_TCNT1 = PRESCALER / CLOCKS_PER_MICRO;
	static const uint32_t MICROS_PER_OVERFLOW = MICROS_PER_TCNT1 * MAX_COUNTS_COUNTER_REGISTER;

	return overflow_count * MICROS_PER_OVERFLOW + TCNT1 * MICROS_PER_TCNT1;
}
/*****************************************************************************/