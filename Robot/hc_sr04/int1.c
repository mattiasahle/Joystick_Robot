/******************************************************************************
	INT1 IMPLEMENTATION FILE

	This file contains function implementations to handle INT1.

    Implement the interrupt service routine for INT1 in your
    execution file:

    ISR(INT1_vect) {
        //interrupt code
    }

	For more information regarding the implementation, please refer to
	Atmel-8271J-AVR- ATmega-Datasheet_11/2015.

	Created: 2020-09-03
	Author: Mattias Ahle, mattias.ahle@gmail.com

	Last update: 2020-10-15
******************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>



/******************************************************************************
	Function name:	int1_init()

	This function initializes External Interrupt 1 (INT1) settings.
	To select which pin change to trigger an interrupt, see description
	below.

	EICRA - External Interrupt Control Register A
		bit    7	  6      5      4      3      2      1      0
			[  -  ][  -  ][  -  ][  -  ][ISC11][ISC10][ISC01][ISC00]

		Bit 0 and 1 is used by INT0 (Interrupt 0 @ PORTD2)
		Bit 2 and 3 is used by INT1 (Interrupt 1 @ PORTD3)

		Bit patterns:
		00 - Low level on the input pin generates an interrupt request
		01 - Any logical change on the input pin generates an interrupt request
		10 - The falling edge of the input pin generates an interrupt request
		11 - The rising edge of the input pin generates an interrupt request
******************************************************************************/
void int1_init(void) {
	//Select trigger:
	EICRA &= ~(1 << ISC11);
	EICRA |= (1 << ISC10);

	EIMSK |= (1 << INT1);	//activate INT1

	sei();					//Enable interrupts globally
}
/*****************************************************************************/