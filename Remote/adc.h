/******************************************************************************
	ANALOGUE TO DIGITAL CONVERSION HEADER FILE

	This file contains definitions of macros and function prototypes for
	analogue to digital conversion.

	Author: Mattias Ahle
			mattias.ahle@gmail.com
	Created: 2020-06-07
	Last update: 2020-10-22
******************************************************************************/

#include <stdint.h>



/******************************************************************************
	Function name:	adc_init()

	This function initializes analog to digital conversion (ADC) registers.

	Inputs:		uint8_t resultBits
					Provide the number of bits the result of the ADC
					should be represented in. Select either 8 or 10 bits.
					 8 bits == low resolution
					10 bits == high resolution
	Outputs:	void
	Calls:		-
******************************************************************************/
void adc_init(void);
/*****************************************************************************/



/******************************************************************************
	Function name:	adc_do_conversion_8bit()

	This function starts a conversion of the analogue signal of a peripheral,
	waits until it is finished and then returns	the ADC value.

	Inputs:		uint8_t (unsigned char) channel
	Outputs:	uint8_t (unsigned char)
	Calls:		select_channel()

	Argument(s):
	uint8_t channel
		- Provide the analogue input channel number.

	Return value:
	uint8_t
		- Returns the the 8 most significant bits of the 10 bit digital value
		produced by the analogue to digital conversion of the peripheral's
		output signal.
		This value is also accessible at the address pointed by ADCH macro.
		Example:
		uint8_t my8bitADCResult = ADCH;
******************************************************************************/
uint8_t adc_do_conversion_8bit(uint8_t channel);
/*****************************************************************************/



/******************************************************************************
	Function name:	adc_do_conversion_10bit()

	This function starts a conversion of the analogue signal of a peripheral,
	waits until it is finished and then returns	the ADC value.

	Inputs:		uint8_t (unsigned char) channel
	Outputs:	uint16_t (unsigned short)
	Calls:		select_channel()

	Argument(s):
	uint8_t channel
		- Provide the analogue input channel number.

	Return value:
	uint16_t
		- Returns the 10 bit digital value produced by the analogue to
		  digital conversion of the peripheral's output signal.
		  This value is also accessible at the address pointed by ADC macro.
		  Example:
		  uint16_t my10bitADCResult = ADC;
******************************************************************************/
uint16_t adc_do_conversion_10bit(uint8_t channel);
/*****************************************************************************/