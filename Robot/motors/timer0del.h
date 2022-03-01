/******************************************************************************
	TIMER0 HEADER FILE

	This file contains the interface to interact with and use timer0.c.

	Created: 2020-10-15
	Author: Mattias Ahle, mattias.ahle@gmail.com

	Last update: 2020-10-15
	Author: Mattias Ahle
******************************************************************************/

#ifndef TIMER1_H_
#define TIMER1_H_

#include <stdint.h>

void timer1_init(void);
void timer1_start(void);
void timer1_stop(void);
void timer1_reset(void);
uint64_t timer1_get_micros(void);



#endif /* TIMER1_H_ */