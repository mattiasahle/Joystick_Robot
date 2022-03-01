/******************************************************************************
	GAME OF THRONES THEME SONG

	Run play_GoT() to play parts of GoT theme song on PWM controlled hardware.

	Created: 2020-10-28
	Author: Mattias Ahle, mattias.ahle@gmail.com
******************************************************************************/
#define F_CPU 16000000UL
#define TIME_1 240
#define TIME_2 480
#define TIME_4 960
#define TIME_6 1440

//Notes:
#define F2  88
#define G2  98
#define Ab 104
#define Bb 117
#define C  131
#define D  147
#define Eb 156
#define F3 175
#define G3 196

#include "motors/motors.h"
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

void play_GoT(void) {
	motors_set_left_forward();
	motors_set_right_reverse();

	motors_set_both_PWM(G3);
	_delay_ms(TIME_6);

	motors_set_both_PWM(C);
	_delay_ms(TIME_6);

	motors_set_both_PWM(Eb);
	_delay_ms(TIME_1);
	motors_set_both_PWM(F3);
	_delay_ms(TIME_1);
	motors_set_both_PWM(G3);
	_delay_ms(TIME_4);

	motors_set_both_PWM(C);
	_delay_ms(TIME_4);
	motors_set_both_PWM(Eb);
	_delay_ms(TIME_1);
	motors_set_both_PWM(F3);
	_delay_ms(TIME_1);

	motors_set_both_PWM(D);
	_delay_ms(TIME_2);
	motors_set_both_PWM(G2);
	_delay_ms(TIME_2);
	motors_set_both_PWM(Bb);
	_delay_ms(TIME_1);
	motors_set_both_PWM(C);
	_delay_ms(TIME_1);

	motors_set_both_PWM(D);
	_delay_ms(TIME_2);
	motors_set_both_PWM(G2);
	_delay_ms(TIME_2);
	motors_set_both_PWM(Bb);
	_delay_ms(TIME_1);
	motors_set_both_PWM(C);
	_delay_ms(TIME_1);

	motors_set_both_PWM(D);
	_delay_ms(TIME_2);
	motors_set_both_PWM(G2);
	_delay_ms(TIME_2);
	motors_set_both_PWM(Bb);
	_delay_ms(TIME_1);
	motors_set_both_PWM(C);
	_delay_ms(TIME_1);

	motors_set_both_PWM(D);
	_delay_ms(TIME_2);
	motors_set_both_PWM(G2);
	_delay_ms(TIME_2);
	motors_set_both_PWM(Bb);
	_delay_ms(TIME_2);

	motors_set_both_PWM(F3);
	_delay_ms(TIME_6);

	motors_set_both_PWM(Bb);
	_delay_ms(TIME_6);

	motors_set_both_PWM(Eb);
	_delay_ms(TIME_1);
	motors_set_both_PWM(D);
	_delay_ms(TIME_1);
	motors_set_both_PWM(F3);
	_delay_ms(TIME_4);

	motors_set_both_PWM(Bb);
	_delay_ms(TIME_6);

	motors_set_both_PWM(Eb);
	_delay_ms(TIME_1);
	motors_set_both_PWM(D);
	_delay_ms(TIME_1);
	motors_set_both_PWM(F2);
	_delay_ms(TIME_2);
	motors_set_both_PWM(Ab);
	_delay_ms(TIME_1);
	motors_set_both_PWM(Bb);
	_delay_ms(TIME_1);

	motors_set_both_PWM(C);
	_delay_ms(TIME_2);
	motors_set_both_PWM(F2);
	_delay_ms(TIME_2);
	motors_set_both_PWM(Ab);
	_delay_ms(TIME_1);
	motors_set_both_PWM(Bb);
	_delay_ms(TIME_1);

	motors_set_both_PWM(C);
	_delay_ms(TIME_2);
	motors_set_both_PWM(F2);
	_delay_ms(TIME_2);
	motors_set_both_PWM(Ab);
	_delay_ms(TIME_1);
	motors_set_both_PWM(Bb);
	_delay_ms(TIME_1);

	motors_set_both_PWM(C);
	_delay_ms(TIME_2);
	motors_set_both_PWM(F2);
	_delay_ms(TIME_2);
	motors_set_both_PWM(Ab);
	_delay_ms(TIME_2);
}