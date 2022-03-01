/******************************************************************************
	USART0 MAIN FILE

	This file contains function implementations to handle USART0.

	Be aware of code blocks in this file market with header "EDIT IF
	NECESSARY". Make sure these blocks hold the code wanted.

	For more information regarding the implementation, please refer to
	Atmel-8271J-AVR- ATmega-Datasheet_11/2015.

	Author: Mattias Ahle
			mattias.ahle@gmail.com
	Created: 2020-09-08
	Last update: 2020-10-27
******************************************************************************/



/******************************************************************************
	EDIT IF NECESSARY
******************************************************************************/
#define F_CPU 16000000UL
#define BAUDRATE 9600
/*****************************************************************************/
#define UBRR_BAUDRATE ((F_CPU / (BAUDRATE * 16L)) - 1)

#include <avr/io.h>
#include <avr/interrupt.h>



/******************************************************************************
	FUNCTION PROTOTYPES
******************************************************************************/
void usart_init(void);
void usart_transmit_character(char c);
void usart_transmit_string(char *s);
/*****************************************************************************/



/******************************************************************************
	EDIT IF NECESSARY - INTERRUPT SERVICE ROUTINE
******************************************************************************/
ISR(USART_RX_vect) {
	//Put code here
}
/*****************************************************************************/



/******************************************************************************
	This function initializes USART0.

	Inputs:		none
	Outputs:	none
	Calls:		sei()
******************************************************************************/
void usart_init(void) {
	UBRR0H = (UBRR_BAUDRATE >> 8);
	UBRR0L = UBRR_BAUDRATE;

	UCSR0B |= (1 << TXEN0) | (1 << RXEN0); 		//Transmit/Receive enable

	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
	UCSR0C &= ~(1 << UMSEL00) & ~(1 << UPM00) & ~(1 << USBS0);

	sei();
}



/******************************************************************************
	This function transmits a character via USART-TX.

	Inputs:		uint8_t
	Outputs:	none
	Calls:		none
******************************************************************************/
void usart_transmit_character(char c) {
	while (!(UCSR0A & (1 << UDRE0))) {}		//wait until sending is possible
	UDR0 = c;
}



/******************************************************************************
	This function transmits a string via USART-TX.

	Inputs:		char *
	Outputs:	none
	Calls:		usart_transmit_character()
******************************************************************************/
void usart_transmit_string(char *s) {
	while (*s) {							//sending char until "/0"
		usart_transmit_character(*s);
		s++;
	}
}



/******************************************************************************
	This function returns 1 (true) if a transmission is received (USART-RX).

	Inputs:		none
	Outputs:	uint8_t
	Calls:		none
******************************************************************************/
uint8_t usart_receive(void) {
	return (UCSR0A & (1<<7)) != 0;
}