/******************************************************************************
	MOTORS HEADER FILE

	This file contains the interface to interact with and use motors.c.

	Created: 2020-10-13
	Author: Mattias Ahle, mattias.ahle@gmail.com

	Last update: 2020-10-27
	Author: Mattias Ahle
******************************************************************************/


#ifndef MOTORS_H_
#define MOTORS_H_

#include <stdint.h>

void motors_init(void);

void motors_set_both_forward(void);
void motors_set_both_reverse(void);
void motors_set_right_forward(void);
void motors_set_right_reverse(void);
void motors_set_left_forward(void);
void motors_set_left_reverse(void);
void motors_set_both_neutral(void);
void motors_set_right_neutral(void);
void motors_set_left_neutral(void);

void motors_set_both_PWM(uint8_t PWM);
void motors_set_left_PWM(uint8_t PWM);
void motors_set_right_PWM(uint8_t PWM);
void motors_stop(void);



#endif /* MOTORS_H_ */