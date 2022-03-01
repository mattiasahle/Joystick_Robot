/******************************************************************************
	USART0 HEADER FILE

	This file contains the interface to interact with and use usart0.c.

	Author: Mattias Ahle
			mattias.ahle@gmail.com
	Created: 2020-09-08
	Last update: 2020-10-27
******************************************************************************/

#ifndef USART0_H_
#define USART0_H_

void usart_init(void);
void usart_transmit_character(char c);
void usart_transmit_string (char *s);
uint8_t usart_receive(void);



#endif /* USART0_H_ */