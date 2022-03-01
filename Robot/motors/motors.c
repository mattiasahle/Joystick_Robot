/******************************************************************************
	MOTORS IMPLEMENTATION FILE

	This file contains implementations to handle two servo motors.

	Created: 2020-10-13
	Author: Mattias Ahle, mattias.ahle@gmail.com

	Last update: 2020-10-27
    Author: Mattias Ahle
******************************************************************************/

#define F_CPU 16000000UL

//PIN connections
#define L_CTRL_1 PIND2
#define L_CTRL_2 PIND4
#define PWML	 PIND5
#define PWMR	 PIND6
#define R_CTRL_1 PIND7
#define R_CTRL_2 PINB0

//PWM output registers
#define PWML_SET OCR0B
#define PWMR_SET OCR0A

#include "timer0.h"
#include <avr/io.h>
#include <util/delay.h>



/******************************************************************************
	FUNCTION PROTOTYPES
******************************************************************************/
void motors_init(void);

//Gear control
void motors_set_both_forward(void);
void motors_set_both_reverse(void);
void motors_set_right_forward(void);
void motors_set_right_reverse(void);
void motors_set_left_forward(void);
void motors_set_left_reverse(void);
void motors_set_both_neutral(void);
void motors_set_right_neutral(void);
void motors_set_left_neutral(void);

//Speed control
void motors_set_speeds(int16_t left_motor_speed, int16_t right_motor_speed);
void motors_stop(void);
/*****************************************************************************/



/******************************************************************************
	This function initializes motor connections to the microcontroller.

	The microcontroller pins which have the motor control pins connected to
	them is set to output.

	The motors use TIMER0 for pulse width modulation (PWM) speed control.
	TIMER0 initialization is called in this function.

	Inputs:		void
	Outputs:	void
	Calls:		none
******************************************************************************/
void motors_init(void) {
	timer0_init();

	DDRB |= (1 << R_CTRL_2);
	DDRD |= (1 << L_CTRL_1) |
			(1 << L_CTRL_2) |
			(1 << PWML) |
			(1 << PWMR) |
			(1 << R_CTRL_1);
}
/*****************************************************************************/



/******************************************************************************
	MOTOR DIRECTION CONTROL

	The functions below sets the direction of movement of the motors.
	Observe that these functions not put any load, i.e. speed, on the
	motors, they are barely the "gearbox".
******************************************************************************/
void motors_set_both_forward(void) {
	motors_set_left_forward();
	motors_set_right_forward();
}

void motors_set_both_reverse(void) {
	motors_set_left_reverse();
	motors_set_right_reverse();
}

void motors_set_both_neutral(void) {
	motors_set_left_neutral();
	motors_set_right_neutral();
}

void motors_set_right_forward(void) {
	PORTB &= ~(1 << R_CTRL_2);
	PORTD |= (1 << R_CTRL_1);
}

void motors_set_right_reverse(void) {
	PORTD &= ~(1 << R_CTRL_1);
	PORTB |= (1 << R_CTRL_2);
}

void motors_set_right_neutral(void) {
	PORTD &= ~(1 << R_CTRL_1);
	PORTB &= ~(1 << R_CTRL_2);
}

void motors_set_left_forward(void) {
	PORTD &= ~(1 << L_CTRL_1);
	PORTD |= (1 << L_CTRL_2);
}

void motors_set_left_reverse(void) {
	PORTD &= ~(1 << L_CTRL_2);
	PORTD |= (1 << L_CTRL_1);
}

void motors_set_left_neutral(void) {
	PORTD &= ~(1 << L_CTRL_2);
	PORTD &= ~(1 << L_CTRL_1);
}
/*****************************************************************************/



/******************************************************************************
	MOTOR SPEED CONTROL

	The functions below sets the speed of the motors. Observe that the motor
	directions has to be set prior to use of any speed function.
******************************************************************************/
void motors_set_both_PWM(uint8_t PWM) {
	PWML_SET = PWM;
	PWMR_SET = PWM;
}

void motors_set_left_PWM(uint8_t PWM) {
	PWML_SET = PWM;
}

void motors_set_right_PWM(uint8_t PWM) {
	PWMR_SET = PWM;
}

void motors_stop(void) {
	PWML_SET = 0;
	PWMR_SET = 0;
	motors_set_both_neutral();
}
/*****************************************************************************/