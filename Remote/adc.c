/******************************************************************************
	ANALOG TO DIGITAL CONVERSION IMPLEMENTATION FILE

	This file contains function implementations to handle analogue to
	digital conversion.

	Atmel-8271J-AVR- ATmega-Datasheet_11/2015 is referenced via page numbers
	in some comments.

	Created: 2020-06-07
	Author: Mattias Ahle, mattias.ahle@gmail.com

	Last update: 2020-10-22
    Author: Mattias Ahle
******************************************************************************/

#define F_CPU 16000000UL

#include <avr/io.h>



/******************************************************************************
	Function name:	adc_init()

	This is a public function and is described in the header file, adc.h.
******************************************************************************/
void adc_init(void) {
/******************************************************************************
	ADMUX - ADC Multiplexer Selection Register
		bit    7      6      5      4      3      2      1      0
			[REFS1][REFS0][ADLAR][  -  ][MUX3 ][MUX2 ][MUX1 ][MUX0 ]

		Result adjustment bit ADLAR is set in respective conversion function
		call.

		Channel selection bits MUX[3:0] are set in respective conversion call
		to adc_doConversion().

		See page 248 for more information.
******************************************************************************/
	ADMUX &= ~(1 << REFS1);
	ADMUX |= (1 << REFS0);
/******************************************************************************
	ADCSRA - ADC Control and Status Register A
		bit    7      6      5      4      3      2      1      0
			[ADEN ][ADSC ][ADATE][ADIF ][ADIE ][ADPS2][ADPS1][ADPS0]

		See page 249 for more information.
******************************************************************************/
	ADCSRA |= (1 << ADEN);
	ADCSRA &= ~(1 << ADSC);
	ADCSRA &= ~(1 << ADATE);
	ADCSRA &= ~(1 << ADIF);
	ADCSRA &= ~(1 << ADIE);
	ADCSRA |= (1 << ADPS2);
	ADCSRA |= (1 << ADPS1);
	ADCSRA |= (1 << ADPS0);
}
/*****************************************************************************/



/******************************************************************************
	Function name:	select_channel()

	This function sets the selected analogue input channel selection bits
	MUX[3:0] in ADMUX register.

	The eight analogue input channels [7:0] of the Atmega 328P are selectable.

	Inputs:			uint8_t channel
	Outputs:		void
	Called by:		adc_do_conversion_8bit()
					adc_do_conversion_10bit()
	Calls:			none
******************************************************************************/
void select_channel(uint8_t channel) {
	if (channel == 0) {
		ADMUX &= ~(1 << MUX3);
		ADMUX &= ~(1 << MUX2);
		ADMUX &= ~(1 << MUX1);
		ADMUX &= ~(1 << MUX0);
	} else if (channel == 1) {
		ADMUX &= ~(1 << MUX3);
		ADMUX &= ~(1 << MUX2);
		ADMUX &= ~(1 << MUX1);
		ADMUX |= (1 << MUX0);
	} else if (channel == 2) {
		ADMUX &= ~(1 << MUX3);
		ADMUX &= ~(1 << MUX2);
		ADMUX |= (1 << MUX1);
		ADMUX &= ~(1 << MUX0);
	} else if (channel == 3) {
		ADMUX &= ~(1 << MUX3);
		ADMUX &= ~(1 << MUX2);
		ADMUX |= (1 << MUX1);
		ADMUX |= (1 << MUX0);
	} else if (channel == 4) {
		ADMUX &= ~(1 << MUX3);
		ADMUX |= (1 << MUX2);
		ADMUX &= ~(1 << MUX1);
		ADMUX &= ~(1 << MUX0);
	} else if (channel == 5) {
		ADMUX &= ~(1 << MUX3);
		ADMUX |= (1 << MUX2);
		ADMUX &= ~(1 << MUX1);
		ADMUX |= (1 << MUX0);
	} else if (channel == 6) {
		ADMUX &= ~(1 << MUX3);
		ADMUX |= (1 << MUX2);
		ADMUX |= (1 << MUX1);
		ADMUX &= ~(1 << MUX0);
	} else if (channel == 7) {
		ADMUX &= ~(1 << MUX3);
		ADMUX |= (1 << MUX2);
		ADMUX |= (1 << MUX1);
		ADMUX |= (1 << MUX0);
	}
}
/*****************************************************************************/



/******************************************************************************
	Function name:	adc_do_conversion_8bit()

	This is a public function and is described in the header file, adc.h.
******************************************************************************/
uint8_t adc_do_conversion_8bit(uint8_t channel) {
	select_channel(channel);
	ADMUX |= (1 << ADLAR);				//Left adjust result to store the eight
										//most significant bits in ADCH register

	ADCSRA |= (1 << ADSC);				//Start conversion
	while (ADCSRA & (1 << ADSC)) {}		//Wait until conversion is completed

	return ADCH;						//ADCH = 8/10 m.s.b. of ADC result
}
/*****************************************************************************/



/******************************************************************************
	Function name:	adc_do_conversion_10bit()

	This is a public function and is described in the header file, adc.h.
******************************************************************************/
uint16_t adc_do_conversion_10bit(uint8_t channel) {
	select_channel(channel);
	ADMUX &= ~(1 << ADLAR);				//Right adjust result

	ADCSRA |= (1 << ADSC);				//Start conversion
	while (ADCSRA & (1 << ADSC)) {}		//Wait until conversion is completed

	return ADC;							//10 bit conversion result
}
/*****************************************************************************/