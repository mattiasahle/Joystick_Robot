/*******************************************************************************

	KEYSTUDIO JOYSTICK IMPLEMENTATION FILE

	This file contains function implementations for using the KeyStudio
	Joystick.

	Created: 2020-10-22
	Author: Mattias Ahle, mattias.ahle@gmail.com

	Last update: 2020-10-28
    By: Mattias Ahle

*******************************************************************************/



/*******************************************************************************
	DEFINE
*******************************************************************************/
#define F_CPU 16000000UL



/*******************************************************************************
	INCLUDE
*******************************************************************************/
#include "joystick.h"
#include "adc.h"
#include "oled/lcd.h"
#include "oled/printout.h"
#include <stdio.h>
#include <util/delay.h>
#include <math.h>
#include <stdlib.h>



/*******************************************************************************
	PRIVATE FUNCTION PROTOTYPES
*******************************************************************************/
static void calc_x_range(void);
static void calc_y_range(void);



/*******************************************************************************
	GLOBAL VARIABLES
*******************************************************************************/
/*** Derived constants ***/
//Min, max and range in each axis' positive and negative region
static uint8_t X_NEGATIVE_MIN;
static uint8_t X_NEGATIVE_MAX;
static uint8_t X_NEGATIVE_RANGE;
static uint8_t X_POSITIVE_MIN;
static uint8_t X_POSITIVE_MAX;
static uint8_t X_POSITIVE_RANGE;
static uint8_t Y_NEGATIVE_MIN;
static uint8_t Y_NEGATIVE_MAX;
static uint8_t Y_NEGATIVE_RANGE;
static uint8_t Y_POSITIVE_MIN;
static uint8_t Y_POSITIVE_MAX;
static uint8_t Y_POSITIVE_RANGE;



/*******************************************************************************
	PUBLIC FUNCTIONS
*******************************************************************************/

/*******************************************************************************
	Function name:	joystick_init()
	Arguments:		none
	Returns:		none
	Calls:			adc_init()
					calc_x_range()
					calc_y_range()

	Initializes ADC to be used to get joystick position.
*******************************************************************************/
void joystick_init(void)
{
	adc_init();
	calc_x_range();
	calc_y_range();

	DDRC &= ~(1 << JOYSTICK_X);
	DDRC &= ~(1 << JOYSTICK_Y);
	DDRB &= ~(1 << JOYSTICK_B);

	//Internal pull-up resistor on joystick button input
	//PORTD |= (1 << JOYSTICK_B);
}

/*******************************************************************************
	Joystick button press
*******************************************************************************/
uint8_t joystick_button_is_pressed(void)
{
	return !(PINB & (1 << JOYSTICK_B));
}

void joystick_handle_button_press(void)
{
	lcd_clrscr();
	printout_lcd_pos_puts(6, 3, "DE E NAJS!");
	_delay_ms(500);
	lcd_clrscr();
}



/*******************************************************************************
	Function name:	joystick_get_position()
	Arguments:		char axis ('X' or 'Y')
	Returns:		An 8-bit value of the joystick axis position
	Calls:			adc_do_conversion_8bit()
*******************************************************************************/
uint8_t joystick_get_position(char axis)
{
	if (axis == 'X')
	{
		return adc_do_conversion_8bit(JOYSTICK_X);
	}
	else if (axis == 'Y')
	{
		return adc_do_conversion_8bit(JOYSTICK_Y);
	}
	else
	{
		return -1;	//argument error
	}
}



/*******************************************************************************
	Function name:	joystick_is_<axis>_<positive/negative>()
	Arguments:		Pointer to joystick x/y-value
	Returns:		1 if true, 0 if false
	Calls:			none

	Checks if the joystick's logical position is positive/negative relative to
	it's relaxed position, or it's origin.

	If the joystick orientation setup in joystick.h is changed, these four
	functions greater than/less than operators needs to be changed
	accordingly.
*******************************************************************************/
uint8_t joystick_is_x_positive(uint8_t *p_x)
{
	return *p_x < X_ZERO;
}

uint8_t joystick_is_x_negative(uint8_t *p_x)
{
	return *p_x > X_ZERO;
}

uint8_t joystick_is_y_positive(uint8_t *p_y)
{
	return *p_y < Y_ZERO - ZERO_OFFSET;
}

uint8_t joystick_is_y_negative(uint8_t *p_y)
{
	return *p_y > Y_ZERO + ZERO_OFFSET;
}



/*******************************************************************************
	Function name:	joystick_get_<axis>_<negative/positive>_range()
	Arguments:		none
	Returns:		The, upon initialization calculated,
					positive/negative range in an axis
	Calls:			none

	See header description in joystick.h for more information.
*******************************************************************************/
uint8_t joystick_get_x_negative_range(void)
{
	return X_NEGATIVE_RANGE;
}

uint8_t joystick_get_x_positive_range(void)
{
	return X_POSITIVE_RANGE;
}

uint8_t joystick_get_y_negative_range(void)
{
	return Y_NEGATIVE_RANGE;
}

uint8_t joystick_get_y_positive_range(void)
{
	return Y_POSITIVE_RANGE;
}



/*******************************************************************************
	Function name:	joystick_get_<axis>_<negative/positive>_min()
	Arguments:		none
	Returns:		The, upon initialization calculated, minimum value
					greater than the origin in an axis' range
	Calls:			none

	See header description in joystick.h for more information.
*******************************************************************************/
uint8_t joystick_get_x_negative_min(void)
{
	return X_NEGATIVE_MIN;
}

uint8_t joystick_get_x_positive_min(void)
{
	return X_POSITIVE_MIN;
}

uint8_t joystick_get_y_negative_min(void)
{
	return Y_NEGATIVE_MIN;
}

uint8_t joystick_get_y_positive_min(void)
{
	return Y_POSITIVE_MIN;
}



/*******************************************************************************
	PRIVATE FUNCTIONS
*******************************************************************************/

/*******************************************************************************
	Function name:	calc_<axis>_range()
	Arguments:		none
	Returns:		none
	Calls:			abs()

	Calculates values derived from the macro definition setup in joystick.h.
*******************************************************************************/
static void calc_x_range(void)
{
	if (X_VAL_LEFT > X_ZERO)
	{
		X_NEGATIVE_MIN = X_ZERO + 1;
		X_NEGATIVE_MAX = X_VAL_LEFT;
		X_NEGATIVE_RANGE = X_NEGATIVE_MAX - X_NEGATIVE_MIN;
		X_POSITIVE_MIN = X_ZERO - 1;
		X_POSITIVE_MAX = X_VAL_RIGHT;
		X_POSITIVE_RANGE = abs(X_POSITIVE_MAX - X_POSITIVE_MIN);
	}
	else
	{
		X_NEGATIVE_MIN = X_ZERO - 1;
		X_NEGATIVE_MAX = X_VAL_LEFT;
		X_NEGATIVE_RANGE = abs(X_NEGATIVE_MAX - X_NEGATIVE_MIN);
		X_POSITIVE_MIN = X_ZERO + 1;
		X_POSITIVE_MAX = X_VAL_RIGHT;
		X_POSITIVE_RANGE = X_POSITIVE_MAX - X_POSITIVE_MIN;
	}
}

static void calc_y_range(void)
{
	if (Y_VAL_DOWN > Y_ZERO)
	{
		Y_NEGATIVE_MIN = Y_ZERO + 1;
		Y_NEGATIVE_MAX = Y_VAL_DOWN;
		Y_NEGATIVE_RANGE = Y_NEGATIVE_MAX - Y_NEGATIVE_MIN;
		Y_POSITIVE_MIN = Y_ZERO - 1;
		Y_POSITIVE_MAX = Y_VAL_UP;
		Y_POSITIVE_RANGE = abs(Y_POSITIVE_MAX - Y_POSITIVE_MIN);
	}
	else
	{
		Y_NEGATIVE_MIN = Y_ZERO - 1;
		Y_NEGATIVE_MAX = Y_VAL_DOWN;
		Y_NEGATIVE_RANGE = abs(Y_NEGATIVE_MAX - Y_NEGATIVE_MIN);
		Y_POSITIVE_MIN = Y_ZERO + 1;
		Y_POSITIVE_MAX = Y_VAL_UP;
		Y_POSITIVE_RANGE = Y_POSITIVE_MAX - Y_POSITIVE_MIN;
	}
}