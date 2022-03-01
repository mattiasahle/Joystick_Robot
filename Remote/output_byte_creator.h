/*
 * output_byte_creator.h
 *
 * Created: 2020-10-22 20:29:27
 *  Author: Mattias Ahle
 */


#ifndef OUTPUT_BYTE_CREATOR_H_
#define OUTPUT_BYTE_CREATOR_H_

#include <stdint.h>

void output_byte_creator_init(void);

/*******************************************************************************
	Arguments:		unsigned char, or unsigned 8-bit int, pointers
					(uint8_t *p_x, uint8_t *p_x) to ADC joystick values x and y
					and a char ('L' or 'R')	representing left or right motor.
	Returns:		The byte to output to a motor.

	Creates and returns a byte which holds information of which motor
	(left or right), the direction of the motor (forward or reverse)
	and a 6-bit PWM value to set the motor speed.

	Output byte:
	Bit	  7	   6    5    4    3    2       1	      0
	    [PWM][PWM][PWM][PWM][PWM][PWM][DIRECTION][LEFT_RIGHT]
*******************************************************************************/
uint8_t output_byte_creator_create(uint8_t *p_x, uint8_t *p_y, char motor);



#endif /* OUTPUT_BYTE_CREATOR_H_ */