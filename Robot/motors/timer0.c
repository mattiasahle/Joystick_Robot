/******************************************************************************
	TIMER0 IMPLEMENTATION FILE

	This file contains implementations to handle Timer0.

	For more information regarding the implementation, please refer to
	Atmel-8271J-AVR- ATmega-Datasheet_11/2015.

	Created: 2020-10-15
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

	Prescaler	| Micros/TCNT1	| -> value in
				|				| PRESCALER below
	____________|_______________|______________________________________
	No presc	|  0.0625		| ->    1
	8 bit		|  0.5			| ->    8
	64 bit		|  4			| ->   64
	256 bit		| 16			| ->  256
	1024 bit	| 64			| -> 1024
******************************************************************************/
static const uint16_t PRESCALER = 1024;
/*****************************************************************************/




/******************************************************************************
	GLOBAL VARIABLES
******************************************************************************/
//overflow_count holds number of TCNT0 overflows
static volatile uint64_t overflow_count = 0;

static const uint8_t CLOCKS_PER_MICRO = F_CPU / 1000000;

//8-bit register = 256, 16-bit register = 65536
static const uint32_t MAX_COUNTS_COUNTER_REGISTER = 256;
/*****************************************************************************/



/******************************************************************************
	FUNCTION PROTOTYPES
******************************************************************************/
void timer0_init(void);
void timer0_start(void);
void timer0_stop(void);
void timer0_reset(void);
uint64_t timer0_get_micros(void);
/*****************************************************************************/



/******************************************************************************
	INTERRUPT SERVICE ROUTINE
******************************************************************************/
ISR(TIMER0_OVF_vect) {
	overflow_count++;
}
/*****************************************************************************/



/******************************************************************************
	This function initializes TIMER0.

	Inputs:		void
	Outputs:	void
	Calls:		sei()
******************************************************************************/
void timer0_init(void) {
	//Clear OC0A on Compare Match, set OC0A at BOTTOM, (non-inverting mode)
	TCCR0A |= (1 << COM0A1);
	TCCR0A &= ~(1 << COM0A0);

	//Clear OC0B on Compare Match, set OC0B at BOTTOM, (non-inverting mode)
	TCCR0A |= (1 << COM0B1);
	TCCR0A &= ~(1 << COM0B0);

	//Timer/Counter Mode of Operation = Fast PWM
	//Source for maximum (TOP) counter value = 0xFF
	//Update of OCRx at BOTTOM
	//TOV Flag Set on MAX
	TCCR0B &= ~(1 << WGM02);
	TCCR0A |= (1 << WGM01);
	TCCR0A |= (1 << WGM00);

	//Timer/Counter0, Overflow Interrupt Enable
	TIMSK0 |= (1 << TOIE0);

	//Enable interrupts globally
	sei();

	timer0_start();
}
/*****************************************************************************/



/******************************************************************************
	This function starts TIMER0 counting by setting a prescaler.

	Inputs:		none
	Outputs:	none
	Calls:		none
******************************************************************************/
void timer0_start(void) {
	switch (PRESCALER) {
		case 1:
			TCCR0B &= ~(1 << CS02);
			TCCR0B &= ~(1 << CS01);
			TCCR0B |= (1 << CS00);
			break;
		case 8:
			TCCR0B &= ~(1 << CS02);
			TCCR0B |= (1 << CS01);
			TCCR0B &= ~(1 << CS00);
			break;
		case 64:
			TCCR0B &= ~(1 << CS02);
			TCCR0B |= (1 << CS01);
			TCCR0B |= (1 << CS00);
			break;
		case 256:
			TCCR0B |= (1 << CS02);
			TCCR0B &= ~(1 << CS01);
			TCCR0B &= ~(1 << CS00);
			break;
		case 1024:
			TCCR0B |= (1 << CS02);
			TCCR0B &= ~(1 << CS01);
			TCCR0B |= (1 << CS00);
			break;
		default:
			//No prescaler as default
			TCCR0B &= ~(1 << CS02);
			TCCR0B &= ~(1 << CS01);
			TCCR0B &= ~(1 << CS00);
			break;
	}
}
/*****************************************************************************/



/******************************************************************************
	This function stops TIMER0 counting by setting all Clock Select (CS) bits
	to zero.

	Inputs:		none
	Outputs:	none
	Calls:		none
******************************************************************************/
void timer0_stop(void) {
	TCCR0B &= ~(1 << CS02);
	TCCR0B &= ~(1 << CS01);
	TCCR0B &= ~(1 << CS00);
}
/*****************************************************************************/



/******************************************************************************
	This function stops and resets TIMER0.

	Inputs:		none
	Outputs:	none
	Calls:		none
******************************************************************************/
void timer0_reset(void) {
	timer0_stop();
	overflow_count = 0;
	TCNT0 = 0;
}
/*****************************************************************************/



/******************************************************************************
	This function returns the total number of microseconds stored in the
	overflow count variable together with the TIMER0 counting register TCNT0.

	Inputs:		none
	Outputs:	uint64_t
	Calls:		none
******************************************************************************/
uint64_t timer0_get_micros(void) {
	static const uint8_t MICROS_PER_TCNT0 = PRESCALER / CLOCKS_PER_MICRO;
	static const uint32_t MICROS_PER_OVERFLOW = MICROS_PER_TCNT0 * MAX_COUNTS_COUNTER_REGISTER;

	return overflow_count * MICROS_PER_OVERFLOW + TCNT0 * MICROS_PER_TCNT0;
}
/*****************************************************************************/