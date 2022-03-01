/*
 * output_byte_creator.c
 *
 * Created: 2020-10-22 20:29:16
 *  Author: Mattias Ahle
 */

/*******************************************************************************
	Define
*******************************************************************************/
#define STEERING_SENSITIVITY 0.3 //A larger value increases sensitivity
#define THROTTLE_SENSITIVITY 0.7 //A larger value increases the power given
                                 //to the motors at a given throttle position

#define F_CPU 16000000UL
#define PWM_OUT_MIN 1
#define PWM_OUT_MAX 255
#define PWM_OUT_RANGE (PWM_OUT_MAX - PWM_OUT_MIN)
#define GEAR_SEL_BIT 1
#define MOTOR_SEL_BIT 0



/*******************************************************************************
	Include
*******************************************************************************/
#include "joystick.h"
#include "usart0.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>



/*******************************************************************************
	Private function prototypes
*******************************************************************************/
void calc_x_negative_factor(void);
void calc_x_positive_factor(void);
void calc_y_negative_factor(void);
void calc_y_positive_factor(void);
static void set_left_right_bit(char motor);
static void set_fwd_rev_bit(uint8_t *p_y);
static void set_PWM_bits(uint8_t *p_x, uint8_t *p_y, char motor);
static uint8_t map_x(uint8_t *p_x);
static uint8_t map_y(uint8_t *p_y);
static uint8_t max(int16_t value1, uint8_t value2);



/*******************************************************************************
	Global variables
*******************************************************************************/
/* static uint8_t output_byte
Motor control byte to be constructed and outputted from this unit
6 msb = PWM
bit 1 = Forward/reverse (1/0) motor direction
bit 0 = Left/Right (1/0) motor to control

Output byte:
Bit	  7	   6    5    4    3    2       1	      0
	[PWM][PWM][PWM][PWM][PWM][PWM][DIRECTION][LEFT_RIGHT]
*/
static uint8_t output_byte;

//Scaling factors for each axis and direction
static float X_NEGATIVE_FACTOR;
static float X_POSITIVE_FACTOR;
static float Y_NEGATIVE_FACTOR;
static float Y_POSITIVE_FACTOR;



/*******************************************************************************
	Public functions
*******************************************************************************/
void output_byte_creator_init(void) {
	calc_x_negative_factor();
	calc_x_positive_factor();
	calc_y_negative_factor();
	calc_y_positive_factor();
}



/*******************************************************************************
	See header file for description
*******************************************************************************/
uint8_t output_byte_creator_create(uint8_t *p_x, uint8_t *p_y, char motor)
{
	set_left_right_bit(motor);
	set_fwd_rev_bit(p_y);
	set_PWM_bits(p_x, p_y, motor);

	return output_byte;
} /* output_byte_creator_create() */



/*******************************************************************************
	Private functions
*******************************************************************************/

/*******************************************************************************
	Set left/right motor select bit
*******************************************************************************/
static void set_left_right_bit(char motor)
{
	if (motor == 'L')
	{
		output_byte |= (1 << MOTOR_SEL_BIT);
	}
	else if (motor == 'R')
	{
		output_byte &= ~(1 << MOTOR_SEL_BIT);
	}
	else //wrong input
	{
		output_byte |= (1 << MOTOR_SEL_BIT);	//default as left motor
	}
} /* set_left_right_bit() */



/*******************************************************************************
	Set forward/reverse motor direction bit
*******************************************************************************/
static void set_fwd_rev_bit(uint8_t *p_y)
{
	if (joystick_is_y_positive(p_y))
	{
		output_byte |= (1 << GEAR_SEL_BIT);
	}
	else if (joystick_is_y_negative(p_y))
	{
		output_byte &= ~(1 << GEAR_SEL_BIT);
	}
	else
	{
		//Joystick in neutral y position
	}
} /* set_fwd_rev_bit() */


/*******************************************************************************
	Function name:	set_PWM_bits()
	Arguments:		Pointers to 8-bit ADC joystick values x and y and motor char
	Returns:		none
	Calls:

	Sets the 6 PWM bits in output byte.
*******************************************************************************/
static void set_PWM_bits(uint8_t *p_x, uint8_t *p_y, char motor)
{
    output_byte &= 0b00000011;            //clear old PWM bits

    uint8_t mapped_x = map_x(p_x);
    uint8_t mapped_y = map_y(p_y);

    if (!(joystick_is_y_positive(p_y) || joystick_is_y_negative(p_y)))
	{
	    //if y is neutral (neither positive or negative)
	    output_byte &= 0b00000011;
    }
	else if ((motor == 'L' && joystick_is_x_negative(p_x)) ||
    (motor == 'R' && joystick_is_x_positive(p_x)))
	{
	    //else if this output_byte is aimed for left motor and the joystick
	    //points left
	    //OR
	    //if this output_byte is aimed for right motor and the joystick
	    //points right
	    uint8_t x_adjust = mapped_x * STEERING_SENSITIVITY; //reduce X adjustment impact
	    x_adjust &= 0b11111100;                             //mask out 2 lsb.
	    output_byte |= max(mapped_y - x_adjust, 0);	        //set new PWM bits
	}
	else
	{
	    output_byte |= mapped_y;
    }
} /* set_PWM_bits() */



/*******************************************************************************
	Function name:	map_x()
	Arguments:		Pointer to 8-bit ADC joystick value X
	Returns:		Raw joystick X-value mapped in range 0 - 255
	Calls:			joystick_is_x_negative()
					abs()
					joystick_get_x_negative_min()
					joystick_get_x_positive_min()
*******************************************************************************/
static uint8_t map_x(uint8_t *p_x)
{
	uint8_t mapped_x;

	if (joystick_is_x_negative(p_x))
	{
		mapped_x = PWM_OUT_MIN + X_NEGATIVE_FACTOR *
				   (abs(*p_x - joystick_get_x_negative_min()));
	}
	else if (joystick_is_x_positive(p_x))
	{
		mapped_x = PWM_OUT_MIN + X_POSITIVE_FACTOR *
				   (abs(*p_x - joystick_get_x_positive_min()));
	}
	else
	{
		//joystick x is in origin and is 0
		mapped_x = 0;
	}

	return mapped_x & 0b11111100;    //mask out 2 lsb.
} /* map_x() */



/*******************************************************************************
	Function name:	map_y()
	Arguments:		Pointer to 8-bit ADC joystick value Y
	Returns:		Raw joystick Y-value mapped in range 0 - 255
	Calls:			joystick_is_y_negative()
					abs()
					joystick_get_y_negative_min()
					joystick_get_y_positive_min()
*******************************************************************************/
static uint8_t map_y(uint8_t *p_y)
{
    uint8_t mapped_y;

    if (joystick_is_y_negative(p_y))
	{
	    mapped_y = PWM_OUT_MIN + Y_NEGATIVE_FACTOR *
				   (abs(*p_y - joystick_get_y_negative_min()));
	}
	else if (joystick_is_y_positive(p_y))
	{
	    mapped_y = PWM_OUT_MIN + Y_POSITIVE_FACTOR *
				   (abs(*p_y - joystick_get_y_positive_min()));
	}
	else
	{
	    //joystick y is in origin and is 0
	    mapped_y = 0;
    }

	mapped_y *= THROTTLE_SENSITIVITY;

    return mapped_y & 0b11111100;    //mask out 2 lsb.
} /* map_y() */



void calc_x_negative_factor(void)
{
	X_NEGATIVE_FACTOR = (PWM_OUT_RANGE) / (float) joystick_get_x_negative_range();
}

void calc_x_positive_factor(void)
{
	X_POSITIVE_FACTOR = (PWM_OUT_RANGE) / (float) joystick_get_x_positive_range();
}

void calc_y_negative_factor(void)
{
	Y_NEGATIVE_FACTOR = (PWM_OUT_RANGE) / (float) joystick_get_y_negative_range();
}

void calc_y_positive_factor(void)
{
	Y_POSITIVE_FACTOR = (PWM_OUT_RANGE) / (float) joystick_get_y_positive_range();
}



static uint8_t max(int16_t value1, uint8_t value2)
{
	if (value1 > value2)
	{
		return value1;
	}
	else
	{
		return value2;
	}
}